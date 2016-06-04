/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef SERVERMODULE_H
#define SERVERMODULE_H

#include <QThread>
#include <QDebug>

#include "TServerSocket.h"
#include "TSocket.h"
#include "TThread.h"
#include "TMessage.h"
#include "TMonitor.h"
#include "TList.h"

#include "RequestHandler.h"

/*
 * Server Thread Class
 * Do the handling of the client and answer plot request from the client
*/

class ServerModule : public QThread
{
    Q_OBJECT
public:
    //constructor
    ServerModule();
    //destructor
    ~ServerModule();

    //Overloaded Thread function
    void run();

    //Start Server
    void StartServer();
    //Stop Server
    void StopServer();

    //Check status
    bool isRunning() {return Running;}
    //Give number of connected clients
    int GetNclients() {return clientConnections->GetSize();}
    //Handleling of the client function
    void CheckClient(TSocket *s);

public slots:
    void RedirectLog(QString type, QString message);

private:
    //Server socket
    TServerSocket *server_sock;
    //Monitor
    TMonitor *monitor;
    //List of clients
    TList* clientConnections;
    bool Running, stop;

signals:
    //Signal to logger
    void log(QString type, QString message);

};

#endif // SERVERMODULE_H
