/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef REQUESTHANDLER_H
#define REQUESTHANDLER_H

#include "TList.h"
#include "TObject.h"

#include <QObject>

#include "DMAHCALStorage.h"

/*
 * Request Handler Class
 * Do the handling of the client request and get Plots from TFile to put into TList
*/

class RequestHandler : public QObject
{
    Q_OBJECT
public:
    //Constructor
    RequestHandler();
    //Destructor
    ~RequestHandler();

    //Get Objects from request
    TList* GetObjects(std::string m_Archive, std::string RunDir, std::string SubDir);

signals:
    //Signal to Logger
    void log(QString type, QString message);

public slots:
    //Redirect message to Logger
    void RedirectLog(QString type, QString message);

private:
    //TFile containing Plots (see DMAHCAL Class)
    DMAHCALStorage *pArchive;
};

#endif // REQUESTHANDLER_H
