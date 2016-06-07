#include "RequestHandler.h"

RequestHandler::RequestHandler(std::string Archive)
{
    //init pointer to Archive TFile
    pArchive = new DMAHCALStorage("Archive");
    m_Archive = Archive;
    connect(pArchive, SIGNAL(log(QString,QString)), this, SLOT(RedirectLog(QString, QString)));
}

//-----------------------------------------------------------------------------------------------

RequestHandler::~RequestHandler()
{
    pArchive->deleteLater();
}

//-----------------------------------------------------------------------------------------------

//Get Objects from TFile, return TList
TList *RequestHandler::GetObjects(std::string RunDir, std::string SubDir)
{
  
    pArchive->OpenTFile(m_Archive, "OPEN");
    TList *m_allList = pArchive->GetObjects(RunDir, SubDir);
    return m_allList;
}

//-----------------------------------------------------------------------------------------------

void RequestHandler::RedirectLog(QString type, QString message)
{
    emit log(type, message);
}
