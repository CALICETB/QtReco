#include "ApplicationModuleApi.h"

#include <QTimer>
#include <QEventLoop>
#include <QTime>
#include <QCoreApplication>

//Function for delay
void ApplicationModuleApi::delay(int secs)
{
    QTime dieTime= QTime::currentTime().addSecs(secs);
    while( QTime::currentTime() < dieTime )
    {
        if(stop)
            break;
        else
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
    return;
}

//-----------------------------------------------------------------------------------------------

ApplicationModuleApi::ApplicationModuleApi()
{
    //Parser Class pointer
    m_parser = new XmlParser();
    //Server Class pointer
    m_server = new ServerModule();
    //EventDisplay Main window pointer
    //m_event = new EventDisplayWindow(m_parser);
    m_event = NULL;
    //Module Sed Class pointer
    m_sed = new SedModule(m_parser);

    running = false;
    stop = false;
    isConfigured = false;
    test = m_parser->parser->GetMode();

    //connect
    connect(m_parser, SIGNAL(log(QString, QString)), &m_logger, SLOT(Log(QString, QString)));
    connect(m_server, SIGNAL(log(QString,QString)), &m_logger, SLOT(Log(QString, QString)));
    connect(m_sed, SIGNAL(log(QString,QString)), &m_logger, SLOT(Log(QString,QString)));
    connect(this, SIGNAL(log(QString,QString)), &m_logger, SLOT(Log(QString, QString)));

    if(!m_server->isRunning())
    {
        //Start Server
        m_server->start();
    }

    //Timer for update
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), &m_logger, SLOT(show()));
    connect(timer, SIGNAL(timeout()), this, SLOT(DoUpdate()));
    timer->start(100);
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::init()
{
    //Analysis Module Class pointer
    m_analysis = new AnalysisModule(m_parser);
    //Marlin Module Class pointer
    m_marlin = new MarlinModule(m_parser);

    //Signals to connect
    connect(m_analysis, SIGNAL(finished()), this, SLOT(Stop()));
    connect(m_analysis, SIGNAL(log(QString,QString)), &m_logger, SLOT(Log(QString,QString)));
    connect(m_marlin, SIGNAL(log(QString,QString)), &m_logger, SLOT(Log(QString,QString)));
    connect(m_marlin, SIGNAL(finished()), this, SLOT(DoAnalysis()));
}

//-----------------------------------------------------------------------------------------------

ApplicationModuleApi::~ApplicationModuleApi()
{
    //Destruction of instance
    emit log("DEBUG", "ApplicationModuleApi : destroyed");
    m_sed->deleteLater();
    m_server->deleteLater();
    if(m_event != NULL)
        m_event->deleteLater();
    m_parser->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::run()
{
    //Overload of run function, infinite loop
    running = true;

    /*
    while(1)
    {
        if(stop)
            break;
    }
    */
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::DoUpdate()
{
    //Update Parameters in UI, stored into map
    p_map.clear();
    //Run Number
    p_map["Run"] = QString::fromStdString(m_parser->parser->GetRunNumber());
    //Input file
    p_map["Input"] = QString::fromStdString(m_parser->parser->GetInputFile());
    //Output file
    p_map["Output"] = QString::fromStdString(m_parser->parser->GetRootfile());
    //Ped flag
    p_map["Ped"] = QString::number(m_parser->parser->HasPedSubstraction());
    //MIP Conversion flag
    p_map["MIP"] = QString::number(m_parser->parser->HasADCtoMIP());
    //Mapping file
    p_map["MapIJK"] = QString::fromStdString(m_parser->parser->GetMapFile());

    //Number of clients
    p_map["ClientNumber"] = QString::number(m_server->GetNclients());
    //Number of layers
    p_map["NLayer"] = QString::number(m_parser->parser->GetNLayer());
    //EBU flag
    p_map["EBU"] = QString::number(m_parser->parser->HasEBU());
    //MIP cut value
    p_map["MIPCut"] = QString::number(m_parser->parser->GetMIPCut());
    //Running mode
    p_map["AnalysisMode"] = QString::number(m_parser->parser->GetMode());
    //Minimum hits value
    p_map["NMinHits"] = QString::number(m_parser->parser->GetMinHits());
    //Maximum hits value
    p_map["NMaxHits"] = QString::number(m_parser->parser->GetMaxHits());
    //number of T0s
    p_map["nT0s"] = QString::number(m_parser->parser->GetnT0s());

    emit update(p_map);
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::DoAnalysis()
{
    //Start analysis Module after Marlin
    m_analysis->start();
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::Start()
{
    running = true;

    //if Run configured start Marlin Module
    if(isConfigured)
    {
        this->init();
        m_marlin->start();
    }
    else
    {
        emit log("ERROR", "ApplicationModuleApi : Can't start Marlin, Problem in configuration!");
    }

    emit started();
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::Stop()
{
    //Clean and Stop dispatcher
    emit log("DEBUG", "ApplicationModuleApi : Stop");
    m_marlin->deleteLater();
    m_analysis->deleteLater();

    running = false;
    emit stopped();
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::Quit()
{
    //Clean and Quit dispatcher
    stop = true;
    running = false;

    timer->deleteLater();

    emit log("DEBUG", "ApplicationModuleApi : Exit");

    if(m_server->isRunning())
    {
        //Stop Server
        emit log("DEBUG", "TCPServer : Stopping");
        m_server->StopServer();
    }

    if(m_event != NULL)
        m_event->close();

    //Timer to stop correctly
    QTimer timer_close;
    QEventLoop loop;
    connect(&timer_close, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer_close.start(1000);
    loop.exec();

    m_logger.close();
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::Configure()
{
    //Configure run parameters & prepare Marlin steering file
    m_parser->configure();
    m_sed->init();
    m_sed->start();
    //Cleaning
    m_sed->clean();

    isConfigured = true;
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::OpenDEH()
{
    if(m_event != NULL)
        m_event->show();
}

//-----------------------------------------------------------------------------------------------

void ApplicationModuleApi::RedirectMessage(QString type, QString message)
{
    emit log(type, message);
}
