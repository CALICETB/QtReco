#include "ServerModule.h"

#include "DMAHCAL.h"

ServerModule::ServerModule()
{
    //Number of client list
    clientConnections = new TList();
    Running = false;
    stop = false;
    m_Archive = "";
}

//-----------------------------------------------------------------------------------------------

ServerModule::~ServerModule()
{
    emit log("DEBUG","TCP Server : destroyed");
    delete clientConnections;
    delete server_sock;
    delete monitor;
}

//-----------------------------------------------------------------------------------------------

//Thread starting server
void ServerModule::run()
{
    this->StartServer();
    return;
}

//-----------------------------------------------------------------------------------------------

void ServerModule::StartServer()
{
    //create server socket
    server_sock = new TServerSocket(5622, kTRUE);
    //create monitor of sockets
    monitor = new TMonitor();
    //add socket to monitor
    monitor->Add(server_sock);

    //case socket valid
    if(server_sock->IsValid())
    {
        Running = true;
        emit log("DEBUG","TCP Server : Listening on port 5622");
        while(1)
        {
            if(stop) //if stopping server
            {
                break;
            }

            //Check Client every 20 secs
            TSocket *s;
            if ((s = monitor->Select(20)) != (TSocket*)-1)
                CheckClient(s);

            //sleep of 1 secs
            sleep(1);
        }

	return;
    }
    else //Error socket invalid
    {
        emit log("ERROR", QString("TCP Server : Can't start TSocketServer, error code : %1").arg(QString::number(server_sock->GetErrorCode())));
	return;
    }
}

//-----------------------------------------------------------------------------------------------

void ServerModule::StopServer()
{
    stop = true;
    //Stopping server & clearing list
    if(Running && stop)
    {
        clientConnections->Clear();
        server_sock->Close();
    }
}

//-----------------------------------------------------------------------------------------------

void ServerModule::CheckClient(TSocket *s)
{
    //Accept connection from client
    if (s->IsA() == TServerSocket::Class())
    {
        TSocket *sock = ((TServerSocket*)s)->Accept();
        //add client socket to monitor
        monitor->Add(sock);
        //add socket to client list
        clientConnections->Add(sock);
        emit log("DEBUG",QString("TCP Server : Incoming connection from %1").arg(QString::fromLocal8Bit(sock->GetInetAddress().GetHostAddress())));
    }
    else
    {
        //Check Message received
        TMessage *mess;
        if(s->Recv(mess) <= 0) //case empty message == disconnected
        {
            clientConnections->Remove(s);
            monitor->Remove(s);
            emit log("DEBUG","TCP Server : Client disconnected");
            delete s;
            return;
        }
        else
        {
            //Read message if string
            if(mess->What() == kMESS_STRING)
            {
                std::string request;
                mess->ReadStdString(request);
                emit log("DEBUG",QString("TCP Server : %1").arg(QString::fromStdString(request)));

                std::vector<std::string> tokens;
                std::string delimiter = ",";
                //Tokenize incoming string (Run, Plot)
                DMAHCAL::tokenize(request, tokens, delimiter);

                std::string m_RunDir = tokens.at(0);
                std::string m_SubDir = tokens.at(1);

                //Handle the request
                TList *m_list = new TList();
                RequestHandler *handler = new RequestHandler(m_Archive);
                //Get Plots into a TList
                m_list = handler->GetObjects(m_RunDir, m_SubDir);
                m_list->SetName(m_SubDir.c_str());

                if(!m_list->IsEmpty())
                {
                    //Sending the List to client
                    TMessage answer(kMESS_OBJECT);
                    answer.WriteObjectAny(m_list, m_list->Class());
                    s->Send(answer);
                    delete m_list;
		    delete handler;
                    return;
                }
                else
                {
                    //Error case Run does not exist
                    emit log("ERROR","TCP Server : Error getting Histograms - Run asked does not exist!");
                    TMessage answer(kMESS_STRING);
                    answer.WriteStdString("Run selected not available!");
                    s->Send(answer);
		    delete handler;
                    return;
                }
            }
            else //case wrong message from client
            {
                emit log("ERROR","TCP Server : Wrong format of TMessage");
                return;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------

void ServerModule::RedirectLog(QString type, QString message)
{
    emit log(type, message);
}
