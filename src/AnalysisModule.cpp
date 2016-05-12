#include "AnalysisModule.h"

#include <QDebug>

AnalysisModule::AnalysisModule(XmlParser *settings)
{
    running = false;
    //copy Parser Class pointer
    p_xml = settings;
    stop = false;
}

//-----------------------------------------------------------------------------------------------

AnalysisModule::~AnalysisModule()
{
    //Destruction of instance
    emit log("DEBUG", "Analysis Module : destroyed");
    //p_xml->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void AnalysisModule::init()
{
    //Pointer to Analysis rootfile
    pAnalysis = new DMAHCALStorage("Analysis");
    //Pointer to Storage file
    pStorage = new DMAHCALStorage("Archive");

    connect(pAnalysis, SIGNAL(log(QString,QString)), this, SLOT(RedirectMessage(QString,QString)));
    connect(pStorage, SIGNAL(log(QString,QString)), this, SLOT(RedirectMessage(QString,QString)));

    //instanciation of Analysis
    m_Analysis = new AnalysisThread(p_xml, pAnalysis, pStorage);

    connect(m_Analysis, SIGNAL(log(QString,QString)), this, SLOT(RedirectMessage(QString, QString)));
    connect(m_Analysis, SIGNAL(finished()), this, SLOT(Stop()));

    emit log("DEBUG", "Analysis Module : Init Modules done");
}

//-----------------------------------------------------------------------------------------------

void AnalysisModule::run()
{
    //Run analysis
    running = true;
    init();
    Start();
}

//-----------------------------------------------------------------------------------------------

void AnalysisModule::Start()
{
    emit log("DEBUG", "Analysis Module : Started");
    running = true;
    m_Analysis->start();
}

//-----------------------------------------------------------------------------------------------

void AnalysisModule::Stop()
{
    //Stop when Analysis finished & Clean up
    stop = true;
    running = false;
    pStorage->close();
    pAnalysis->close();

    m_Analysis->deleteLater();
    pStorage->deleteLater();
    pAnalysis->deleteLater();

    emit log("DEBUG", "Analysis Module : Stopped");
    emit finished();
}

//-----------------------------------------------------------------------------------------------

void AnalysisModule::RedirectMessage(QString type, QString message)
{
    emit log(type, message);
}
