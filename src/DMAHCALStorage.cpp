#include "DMAHCALStorage.h"
#include "TKey.h"
#include "TList.h"

#include <QDebug>

DMAHCALStorage::DMAHCALStorage() :
    pFile(NULL),
    m_isOpened(false),
    m_currentPublicationId(0),
    m_fileName(""),
    m_name(""),
    pCurrentDir(NULL),
    pRootDir(NULL)
{
    m_directoryList.clear();
}

//-----------------------------------------------------------------------------------------------

DMAHCALStorage::DMAHCALStorage(std::string name) :
    pFile(NULL),
    m_isOpened(false),
    m_currentPublicationId(0),
    m_fileName(""),
    m_name(name),
    pCurrentDir(NULL),
    pRootDir(NULL)
{
    m_directoryList.clear();
}

//-----------------------------------------------------------------------------------------------

DMAHCALStorage::~DMAHCALStorage()
{
    emit log("DEBUG", "Storage : destroyed");
    delete pFile;
    delete pRootDir;
    delete pCurrentDir;
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::OpenTFile(std::string filename, std::string openingMode)
{
    pFile = new TFile(filename.c_str(), openingMode.c_str());

    if(NULL == pFile)
        emit log("ERROR", QString("Storage: Could not open TFile : %1").arg(QString::fromStdString(filename)));

    m_fileName = filename;
    m_currentPublicationId = 0;
    m_hasBeenWritten = false;
    m_isOpened = true;

    if(openingMode == "READ")
        emit log("DEBUG", QString("Storage: TFile %1 opened").arg(QString::fromStdString(filename)));
    if(openingMode == "RECREATE" || openingMode == "UPDATE")
        emit log("DEBUG", QString("Storage: TFile %1 created/updated").arg(QString::fromStdString(filename)));
}

//-----------------------------------------------------------------------------------------------

TTree* DMAHCALStorage::GetTree(std::string treeName)
{
    return (TTree*)pFile->Get(treeName.c_str());
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::close()
{
    if(!isOpened())
        return;

    pFile->Close();
    delete pFile;
    m_hasBeenWritten = true;
    m_isOpened = false;
}

//-----------------------------------------------------------------------------------------------

std::string DMAHCALStorage::getFilename()
{
    return m_fileName;
}

//-----------------------------------------------------------------------------------------------

int DMAHCALStorage::getPublicationID()
{
    return m_currentPublicationId;
}

//-----------------------------------------------------------------------------------------------

bool DMAHCALStorage::isOpened()
{
    return m_isOpened;
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::WriteElement(TList *m_list)
{
    pCurrentDir->cd();

    emit log("DEBUG", QString("Storage: Writing %1 elements from TList '%2' in dir %3").arg(QString::number(m_list->GetSize()), QString::fromLocal8Bit(m_list->GetName()), QString::fromLocal8Bit(pCurrentDir->GetName())));

    TIter next(m_list);
    TObject *obj;

    while((obj = next()))
    {
        obj->Write();
    }
    m_currentPublicationId++;
}

//-----------------------------------------------------------------------------------------------

bool DMAHCALStorage::RootExist(std::string dirName)
{
    bool m_value = false;

    if(pFile->GetDirectory(dirName.c_str()) != NULL)
        m_value = true;

    return m_value;
}

//-----------------------------------------------------------------------------------------------

bool DMAHCALStorage::dirExist(std::string dirName)
{
    bool m_value = false;

    if(pCurrentDir->GetName() == dirName.c_str())
        m_value = true;

    return m_value;
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::MakeRoot(std::string dirName)
{
    if(this->RootExist(dirName))
    {
        TDirectory *pDirectory = pFile->GetDirectory(dirName.c_str());
        pRootDir = pDirectory;
        return;
    }
    else
    {
        TDirectory *pDirectory = pFile->mkdir(dirName.c_str());
        pRootDir = pDirectory;
        m_directoryList.push_back(pRootDir);
        emit log("DEBUG", QString("Storage: Created dir %1 in %2").arg(QString::fromStdString(dirName), QString::fromLocal8Bit(pFile->GetName())));
    }
    return;
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::mkdir(std::string dirName)
{
    pRootDir->cd();
    if(pRootDir->cd(dirName.c_str()))
    {
        rmDir(dirName);
        TDirectory *pDirectory = pRootDir->mkdir(dirName.c_str());
        pCurrentDir = pDirectory;
        return;
    }
    else
    {
        TDirectory *pDirectory = pRootDir->mkdir(dirName.c_str());
        pCurrentDir = pDirectory;
        m_directoryList.push_back(pCurrentDir);
        emit log("DEBUG", QString("Storage: Created dir %1 in %2").arg(QString::fromLocal8Bit(pCurrentDir->GetName()), QString::fromLocal8Bit(pFile->GetName())));
    }
    return;
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::cd()
{
    pCurrentDir->cd();
}

//-----------------------------------------------------------------------------------------------

TList* DMAHCALStorage::GetObjects(std::string RunDir, std::string SubDir)
{
    TList *m_list = new TList();
    m_list->Clear();

    if(pFile->cd(RunDir.c_str()))
    {
        TDirectory *pDirectory = pFile->GetDirectory(RunDir.c_str());
        if(pDirectory->cd(SubDir.c_str()))
        {
            TDirectory *pTemp = pDirectory->GetDirectory(SubDir.c_str());
            pTemp->cd();
            TIter next(pTemp->GetListOfKeys());
            TKey *key;
            while((key = (TKey*)next()))
            {
                TObject* pObject = key->ReadObj();
                m_list->Add(pObject);
            }

            emit log("DEBUG", QString("Storage: Number of Objects in %2: %1").arg(QString::number(m_list->GetSize()), QString::fromLocal8Bit(pTemp->GetName())));
            return m_list;
        }
        else
        {
            return m_list;
        }
    }
    else
    {
        return m_list;
    }
}

//-----------------------------------------------------------------------------------------------

void DMAHCALStorage::rmDir(std::string dirName)
{
    pRootDir->rmdir(dirName.c_str());
    emit log("DEBUG", QString("Storage: Cleaning dir %1").arg(QString::fromStdString(pRootDir->GetName())));
    return;
}

//-----------------------------------------------------------------------------------------------

TDirectory* DMAHCALStorage::GetRunDir()
{
    return pRootDir;
}

//-----------------------------------------------------------------------------------------------

TDirectory* DMAHCALStorage::GetRunDir(std::string RunDir)
{
    if(pFile->GetDirectory(RunDir.c_str()))
        return pFile->GetDirectory(RunDir.c_str());
    else
    {
        emit log("DEBUG", QString("Storage : No %1 Directory in %2").arg(QString::fromStdString(RunDir), QString::fromLocal8Bit(pFile->GetName())));
        return NULL;
    }
}
