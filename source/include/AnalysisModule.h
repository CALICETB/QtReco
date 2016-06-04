/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef ANALYSISMODULE_H
#define ANALYSISMODULE_H

#include <QThread>
#include "DMAHCALStorage.h"
#include "AnalysisThread.h"
#include "XmlParser.h"
#include <map>

/*
 * Analysis Module Class
 * Module handling the call to do the analysis
*/

class AnalysisModule : public QThread
{
    Q_OBJECT
public:
    //Constructor
    AnalysisModule(XmlParser *settings);
    //Destructor
    ~AnalysisModule();
    //Initialization function
    void init();
    //Start Module function
    void Start();
    //QThread overloaded function
    void run();

    //Check Status of Module
    bool isRunning() {return running;}

public slots:
    //Slot to redirect message to Logger
    void RedirectMessage(QString type, QString message);
    //Slot to Stop Module
    void Stop();

signals:
    //Signal to Logger
    void log(QString type, QString message);
    //Signal Module finished
    void finished();

private:
    //Class variables

    //Storage Class pointer -> analysis rootfile & Plot rootfile
    DMAHCALStorage *pAnalysis, *pStorage;
    //Parser pointer
    XmlParser *p_xml;
    //Pointer to Analysis
    AnalysisThread *m_Analysis;
    bool running, stop;
};

#endif // ANALYSISMODULE_H
