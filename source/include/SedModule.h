/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef SEDMODULE_H
#define SEDMODULE_H

#include <QObject>
#include <QProcess>
#include <string>

#include "XmlParser.h"

/*
 * Sed Module Class
 * Configure Reco xml file for Marlin
*/

class SedModule : public QObject
{
    Q_OBJECT
public:
    //Constructor
    SedModule(XmlParser *settings);
    //Destructor
    ~SedModule();

    //initialization function
    void init();
    //Start sed function
    void start();
    //Cleaning function
    void clean();

signals:
    void log(QString type, QString message);
    //Signal finished
    void finished();

private:
    //Parser pointer
    XmlParser *xml;
    //QProcess to call bash script
    QProcess *process_sed;
    //Parameters variables
    std::string Input_PATH, Run_number, Input_file, Output_PATH;
    bool PedSubs, ADC_Conversion;
};

#endif // SEDMODULE_H
