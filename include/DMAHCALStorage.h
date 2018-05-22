/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef DMAHCALSTORAGE_H
#define DMAHCALSTORAGE_H

#include <QObject>

#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TList.h"
#include "TDirectory.h"

/*
 * Data Monitoring AHCAL Storage Class
 * Handles all related to TFiles
 * Creates new TFiles/TDirectory for Plots and open rootfiles created by Marlin
*/

class DMAHCALStorage : public QObject
{
    Q_OBJECT
public:
    //Constructor
    DMAHCALStorage();
    //Constructor with name
    DMAHCALStorage(std::string name);
    //Destructor
    ~DMAHCALStorage();

    //Open TFile with mode
    void OpenTFile(std::string filename, std::string openingMode);
    //Return TTree of TFile
    TTree *GetTree(std::string treeName);
    //Close TFile
    void close();
    //Return filename
    std::string getFilename();
    //Return ID
    int getPublicationID();
    //Return status of TFile
    bool isOpened();

    //Create Archive with folders
    //Make Root folder (Run number)
    void MakeRoot(std::string dirName);
    //Create folder
    void mkdir(std::string dirName);
    //Go to folder
    void cd();

    //Check if root folder exist
    bool RootExist(std::string dirName);
    //Check if folder exist
    bool dirExist(std::string dirName);

    //Remove folder
    void rmDir(std::string dirName);
    //Get any Run folder
    TDirectory* GetRunDir();
    //Get Specific Run folder
    TDirectory* GetRunDir(std::string RunDir);
    //Write TObjects in TFile
    void WriteElement(TList *m_list);

    //Get TObjects from TFile in Run folder from a specific sub-directory
    TList *GetObjects(std::string RunDir, std::string SubDir);

signals:
    //Signal to Logger
    void log(QString type, QString message);

public slots:

private:
    //Pointer to TFile
    TFile *pFile;
    //Status of TFile
    bool m_isOpened, m_hasBeenWritten;
    int m_currentPublicationId;
    //Class variables
    std::string m_fileName;
    std::string m_name;
    TDirectory *pCurrentDir, *pRootDir;
    std::vector<TDirectory*> m_directoryList;
};

#endif // DMAHCALSTORAGE_H
