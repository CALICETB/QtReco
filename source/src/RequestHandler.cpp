#include "RequestHandler.h"

RequestHandler::RequestHandler()
{
    //init pointer to Archive TFile
    pArchive = new DMAHCALStorage("Archive");
    connect(pArchive, SIGNAL(log(QString,QString)), this, SLOT(RedirectLog(QString, QString)));
}

//-----------------------------------------------------------------------------------------------

RequestHandler::~RequestHandler()
{
    pArchive->deleteLater();
}

//-----------------------------------------------------------------------------------------------

//Get Objects from TFile, return TList
TList *RequestHandler::GetObjects(std::string m_Archive, std::string RunDir, std::string SubDir)
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
