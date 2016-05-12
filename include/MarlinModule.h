/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef MARLINMODULE_H
#define MARLINMODULE_H

#include <QObject>
#include <QProcess>
#include <QStringList>

#include "XmlParser.h"

/*
 * MarlinModule Class
 * Calls Marlin (ilcsoft) within a QProcess
*/

class MarlinModule : public QObject
{
    Q_OBJECT
public:
    //Constructor
    MarlinModule(XmlParser *settings);
    //Destructor
    ~MarlinModule();

    //Initialisation function
    void init();
    //Start function
    void start();
    //Stop function
    void stop();
    //Check status function
    bool isRunning() {return running;}

    //QThread overloaded function
    void run();

    //Return State of QProcess
    QProcess::ProcessState GetState() {return processMarlin->state();}
    //Return PID of QProcess
    Q_PID Getpid() {return processMarlin->pid();}

signals:
    //Signal to Logger
    void log(QString type, QString message);
    //Signal that Marlin started
    void started();
    //Signal that Marlin stopped
    void stopped();
    //Signal that Marlin finished
    void finished();

public slots:
    //Handle function end of QProcess
    void Finished(int exit_status);
    //Handle stdout of QProcess
    void StdOut();
    //Cleaning function
    void clean();

private:
    //Steering parameter
    XmlParser *xml;
    //QProcess Marlin
    QProcess *processMarlin;
    //QProcess variables
    std::string Run_number, marlin_script;
    bool running, test;
};

#endif // MARLINMODULE_H
