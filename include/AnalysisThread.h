/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef ANALYSISTHREAD_H
#define ANALYSISTHREAD_H

#include <QThread>
#include <QObject>
#include <QElapsedTimer>
#include <QMutex>
#include <map>
#include "DMAHCALStorage.h"
#include "XmlParser.h"

/*
 * Analysis Thread Class
 * Thread that do each type of analysis
 * Any function can be added here to make any kind of plots
*/

class AnalysisThread : public QThread
{
    Q_OBJECT
public:
    //Constructor with all parameter needed
    AnalysisThread(XmlParser *xml, DMAHCALStorage *Analysis, DMAHCALStorage *Archive);
    //Destructor
    ~AnalysisThread();
    //QThread overloaded function
    void run();
    //Return time of execution
    qint64 GetElapsedTime() {return timer.elapsed();}

signals:
    //Signal to Logger
    void log(QString type, QString message);
    //Signal end of module
    void finished();

protected:
    //Each Method is here for analysis
    //T0 analysis
    // void T0_Correlation();
    // void T0_Difference();
    // void T0_Sum();
    //Hit Time analysis
    void HitTime();
    //Energy analysis
    void Esum();
    //void EnergyCell();
    void EnergyChannel();
    //Hit/noise analysis
    void Hits();
    //Shower analysis
    void Shower();
    //EnergyPerLayer
    void EnergyLayer();
    //MemoryCells
    void MemoryCells();
    //HitMap analysis
    void HitMap();
    //nHitscogZ analysis
    void nHitscogZ();
    //Temperature analysis
    //void Temperature();

    //Store Mapping of detector from txt file
    bool MakeMapping();

    int GetIJK(int Chip, int Chn);
    int GetChipChn(int I, int J, int K);

    //Veto functions
    //Return number of T0s
    int NumberOfT0(int ahc_hitI[], int ahc_hitJ[], int ahc_hitK[], float ahc_hitEnergy[], float ahc_hitTime[], Int_t ahc_nHits);
    //Return if hit is T0
    int isT0(int I, int J, int K);

    //General parameter for MIP if no calibration
    int EstimateMIP(int layer);

private:
    //Class parameters
    bool EBU, MIP;
    const int nLayer;
    int nMinHits, nMaxHits, nT0s;
    float m_MIPcut;
    std::string m_runNumber, m_ArchiveName, m_Rootfile, m_IJKMapName, m_absorber;

    //Storage pointer -> analysis rootfile
    DMAHCALStorage *pAnalysis;
    //Storage pointer -> Rootfile containing created plots
    DMAHCALStorage *pArchive;
    //Timer variable
    QElapsedTimer timer;
    //Mutex when writting to TFile
    QMutex mutex;
    //Mapping detector
    std::map<int, int> m_map_IJK;
    std::map<int, int> m_map_ChipChn;
    //Map of T0s
    std::map<int, std::vector<int> > m_map_T0s;
};

#endif // ANALYSISTHREAD_H
