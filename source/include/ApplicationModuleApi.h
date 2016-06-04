/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef APPLICATIONMODULEAPI_H
#define APPLICATIONMODULEAPI_H

#include <QThread>
#include <map>
#include <QStringList>

#include "MarlinModule.h"
#include "SedModule.h"
#include "ServerModule.h"
#include "XmlParser.h"
#include "AnalysisModule.h"
#include "Logger.h"
#include "EventDisplayWindow.h"

typedef std::map<QString, QString> ParameterMap;

/*
 * ApplicationModuleApi Class
 * Main Module that handles actions between all the modules
 * Thread to call other classes (SedModule, MarlinModule, AnalysisModule...)
*/

class ApplicationModuleApi : public QThread
{
    Q_OBJECT
public:
    //Constructor
    ApplicationModuleApi();
    //Destructor
    ~ApplicationModuleApi();
    //Check status of module
    bool isRunning() {return running;}
    //Initialization function
    void init();
    //QThread overloaded function
    void run();
    //Delay function
    void delay(int secs);

signals:
    //Signal to Logger
    void log(QString type, QString message);
    //Signal to Main Window for parameters
    void update(ParameterMap map);
    //Signal module started
    void started();
    //Signal module stopped
    void stopped();

public slots:
    //Slot function Start Module
    void Start();
    //Slot function Stop Module
    void Stop();
    //Slot function exit Module
    void Quit();
    //Slot function calling SedModule
    void Configure();
    //Slot function to send parameter to main window
    void DoUpdate();
    //Slot function calling AnalysisModule
    void DoAnalysis();
    //Slot function to open EventDisplay
    void OpenDEH();
    //Slot function to redirect messages to Logger
    void RedirectMessage(QString type, QString message);

private:
    //Server Class Pointer
    ServerModule *m_server;
    //Sed Class Pointer
    SedModule *m_sed;
    //MarlinModule Class Pointer
    MarlinModule *m_marlin;
    //AnalysisModule Class Pointer
    AnalysisModule *m_analysis;
    //Parser Class Pointer
    XmlParser *m_parser;
    //Logger Class Pointer
    Logger m_logger;
    //EventDisplay Class Pointer
    EventDisplayWindow *m_event;

    //Class variables
    bool running, isConfigured, stop, test;
    ParameterMap p_map;
    QTimer *timer;
};

#endif // APPLICATIONMODULEAPI_H
