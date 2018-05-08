#include "MarlinModule.h"

#include <QDebug>

MarlinModule::MarlinModule(XmlParser *settings)
{
    //copy of parser pointer
    xml = settings;
    Run_number = "";
    running = false;
}

//-----------------------------------------------------------------------------------------------

MarlinModule::~MarlinModule()
{
    //Destruction of the instance
    emit log("DEBUG", "Marlin Module : destroyed");
    //xml->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::init()
{
    //Fetching parameters
    Run_number = xml->parser->GetRunNumber();
    test = xml->parser->GetMode();
    //init QProcess for Marlin
    processMarlin = new QProcess();
    processMarlin->setProcessChannelMode(QProcess::ForwardedChannels);
    connect(processMarlin, SIGNAL(readyRead()), this, SLOT(StdOut()));
    connect(processMarlin, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(Finished(int)));
    connect(processMarlin, SIGNAL(finished(int)), this, SLOT(clean()));
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::start()
{
    //init Module
    this->init();
    //Start Thread module
    this->run();
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::run()
{
    emit log("DEBUG", "MarlinModule : Starting");

    QStringList args;
    std::string steering = "/tmp/saivahu/Reco_Run_";
    steering += Run_number;
    steering += ".xml";
    std::string script = "../scripts/myMarlin.sh";

    emit log("DEBUG", QString("MarlinModule : Running Marlin on %1").arg(QString::fromStdString(steering)));

    //Argument for QProcess : script, steering file
    args << QString::fromStdString(script) << QString::fromStdString(steering);

    QStringList args_analysis;
    args_analysis << QString::fromStdString("Running in Analysis Mode");

    //case testing don't run Marlin
    if(test)
        processMarlin->start("/bin/echo", args_analysis);
    //Start Marlin
    else
        processMarlin->start("/bin/bash", args);

    //Case Marlin failed to start
    if(!processMarlin->waitForStarted())
    {
        emit log("ERROR" ,"MarlinModule : Can't Start Reco!");
        running = false;
        return;
    }
    else
    {
        emit started();
        running = true;
    }
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::stop()
{
    //Wait for Marlin to finish
    if(processMarlin->state() == QProcess::Running)
    {
        running = false;
        processMarlin->waitForFinished(-1);
        //Cleaning
        clean();
        emit stopped();
    }
    else
    {
        running = false;
        //Cleaning
        clean();
        emit stopped();
    }
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::Finished(int exit_status)
{
    running = false;
    emit log("DEBUG", QString("MarlinModule : Exit Status = %1").arg(QString::number(exit_status)));

    //Case to handle when end of QProcess
    switch(exit_status)
    {
    case 127:
        emit log("ERROR", "MarlinModule : Error 127 - Marlin not found!");
        return;
    case 128:
        emit log("ERROR", "MarlinModule : Error 128 - Seg fault!");
        return;
    case 0:
        emit log("VERBOSE", "MarlinModule : finished");
        emit finished();
    }
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::clean()
{
    //delete QProcess
    processMarlin->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void MarlinModule::StdOut()
{
    //Redirect Marlin Output to stdout in terminal
    emit log("VERBOSE", "-------------------------------- MarlinModule : Output ----------------------------------------");
    emit log("VERBOSE", processMarlin->readAll());
}
