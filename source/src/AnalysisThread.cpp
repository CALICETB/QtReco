#include "AnalysisThread.h"
#include "DMAHCALBooker.h"

#include <QDebug>

#include <cmath>
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TMultiGraph.h"
#include "TObject.h"
#include "TMath.h"
#include "TSystem.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <tuple>
#include <map>
#include <algorithm>

#define MAXCELL 10000

AnalysisThread::AnalysisThread(XmlParser *xml, DMAHCALStorage *Analysis, DMAHCALStorage *Archive) :
  m_IJKMapName(xml->parser->GetMapFile()),
  m_runNumber(xml->parser->GetRunNumber()),
  m_ArchiveName(xml->parser->GetArchive()),
  m_Rootfile(xml->parser->GetRootfile()),
  pAnalysis(Analysis),
  pArchive(Archive),
  EBU(xml->parser->HasEBU()),
  MIP(xml->parser->HasADCtoMIP()),
  nLayer(xml->parser->GetNLayer()),
  nMinHits(xml->parser->GetMinHits()),
  nMaxHits(xml->parser->GetMaxHits()),
  m_MIPcut(xml->parser->GetMIPCut()),
  m_absorber(xml->parser->GetAbsorber()),
  m_map_T0s(xml->parser->GetT0Map()),
  nT0s(xml->parser->GetnT0s())
{
  gSystem->Load("libPhysics.so");
}

//-----------------------------------------------------------------------------------------------

AnalysisThread::~AnalysisThread()
{
  //Destruction of instance
  pAnalysis->deleteLater();
  pArchive->deleteLater();
  emit log("DEBUG", "AnalysisThread : destroyed");
}

//-----------------------------------------------------------------------------------------------

void AnalysisThread::run()
{
  //Make Mapping
  bool map_ok = MakeMapping();
  //Case not mapping file
  if(!map_ok)
    {
      emit log("ERROR", "AnalysisThread : Not Running because of no IJK Map");
      return;
    }

  //Do analysis
  /*if(nT0s > 0)
    {
      T0_Correlation();
      T0_Difference();
      T0_Sum();
      }*/
  Esum();
  //EnergyCell();
  EnergyChannel();
  HitTime();
  Hits();
  Shower();
  HitMap();
  //Temperature();
  emit finished();
}

//-----------------------------------------------------------------------------------------------

//Mapping
bool AnalysisThread::MakeMapping()
{
  std::string FILE = m_IJKMapName;

  std::ifstream fIn;
  std::string line;
  fIn.open(FILE.c_str());

  //Open mapping file
  if(fIn.is_open())
    {
      while(getline(fIn, line))
	{
	  if(line[0] == '#') continue;

	  std::istringstream iss(line);
	  int layer, ChipID, chn, I, J, K;

	  iss >> layer >> ChipID >> chn >> I >> J >> K;
	  //Combine I, J, K to a single int
	  int index = I*10000 + J*100 + K;

	  //Fill the map (Chip, Chn)->(I, J, K)
	  m_map_IJK.insert(std::make_pair(ChipID*100+chn, index));
	  //Fill the map (I, J, K)->(Chip, Chn)
	  m_map_ChipChn.insert(std::make_pair(index, ChipID*100+chn));
	}

      fIn.close();

      emit log("DEBUG", "IJK Map done!!");
      return true;
    }
  //Case no mapping file found
  else
    {
      emit log("ERROR", QString("Can't find IJK map in %1!!").arg(QString::fromStdString(FILE)));
      return false;
    }
}

//-----------------------------------------------------------------------------------------------

int AnalysisThread::GetIJK(int Chip, int Chn)
{
  std::map<int, int>::iterator itMap;
  int index = Chip*100+Chn;

  int IJK = -1;

  if(m_map_IJK.find(index) != m_map_IJK.end())
    {
      itMap = m_map_IJK.find(index);
      IJK = itMap->second;
    }

  return IJK;
}

//-----------------------------------------------------------------------------------------------

int AnalysisThread::GetChipChn(int I, int J, int K)
{
  std::map<int, int>::iterator itMap;
  int index = I*10000+J*100+K;

  int ChipChn= -1;

  if(m_map_ChipChn.find(index) != m_map_ChipChn.end())
    {
      itMap = m_map_ChipChn.find(index);
      ChipChn = itMap->second;
    }

  return ChipChn;
}

//-----------------------------------------------------------------------------------------------

//Number of T0 in an event
int AnalysisThread::NumberOfT0(int ahc_hitI[MAXCELL],  int ahc_hitJ[MAXCELL],  int ahc_hitK[MAXCELL], float ahc_hitEnergy[MAXCELL], float ahc_hitTime[MAXCELL], Int_t ahc_nHits)
{
  int nT0 = 0;
  for (int i = 0; i < ahc_nHits; i++)
    {
      int I = ahc_hitI[i];
      int J = ahc_hitJ[i];
      int K = ahc_hitK[i];

      float ampl = ahc_hitEnergy[i];
      float time = ahc_hitTime[i];

      //Find T0s in the map and assign the values to variables
      if(m_map_T0s.find(K) != m_map_T0s.end())
	{
	  std::map<int, std::vector<int> >::iterator it = m_map_T0s.find(K);
	  std::vector<int> vec = it->second;

	  int ChipChn = GetChipChn(I,J,K);
	  //Find ChipChn in the vector
	  if(find(vec.begin(), vec.end(), ChipChn) != vec.end())
	    {
	      if(ampl > 0 && time > 0)
		{
		  nT0++;
		}
	    }
	}
    }

  return nT0;
}

//-----------------------------------------------------------------------------------------------

//Check if Cell is T0 or Cherenkov
int AnalysisThread::isT0(int I, int J, int K)
{
  bool isT0channel = false;

  //Find T0s in the map and assign the values to variables
  if(m_map_T0s.find(K) != m_map_T0s.end())
    {
      std::map<int, std::vector<int> >::iterator it = m_map_T0s.find(K);
      std::vector<int> vec = it->second;

      int ChipChn = GetChipChn(I,J,K);
      //Find ChipChn in the vector
      if(find(vec.begin(), vec.end(), ChipChn) != vec.end())
	{
	  isT0channel = true;
	}
    }

  return isT0channel;
}

//-----------------------------------------------------------------------------------------------

int AnalysisThread::EstimateMIP(int layer)
{
  float MIP_Estimate = 0;

  if(layer == 1)//12
    MIP_Estimate = 13*25;
  if(layer == 2)//13
    MIP_Estimate = 13*25;
  if(layer == 3)//14
    MIP_Estimate = 13*20;
  if(layer == 4)//15
    MIP_Estimate = 13*20;

  return MIP_Estimate;
}

//-----------------------------------------------------------------------------------------------

/*
 * T0 Correlation
 * Represent the correlation of T0s/Cherenkov within the same layer
 */

/*void AnalysisThread::T0_Correlation()
{
  emit log("MESSAGE", "T0 Correlation started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //Open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
	
  //Get TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare tree variables
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  //Set Branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  //Create TList for holding the histograms
  TList *m_histoList = new TList();
  m_histoList->SetName("T0 Correlation List");
  
  //Create histograms
  std::vector<TH2F*> hCorrelationT0(nLayer);
  for(std::map<int, std::vector<int> >::iterator it = m_map_T0s.begin(); it != m_map_T0s.end(); ++it)
    {
      int Layer = it->first;
      hCorrelationT0[Layer-1] = new TH2F(TString::Format("hCorrelationT0s_Layer%i", Layer), TString::Format("hCorrelationT0s_Layer%i", Layer), 4096, 0, 4096, 4096, 0, 4096);
      m_histoList->Add(hCorrelationT0[Layer-1]);
    }

  //map with T0s time
  std::map<int, std::vector<float> > time_T0;

  //Loop over the roofile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      //case no hits
      if(ahc_nHits == 0) continue;
      //count number of events
      m_maxevents = std::max(m_maxevents, eventNumber);

      time_T0.clear();

      for (int i = 0; i < ahc_nHits; i++)
	{
	  int I = ahc_hitI[i];
	  int J = ahc_hitJ[i];
	  int K = ahc_hitK[i];

	  if(isT0(I, J, K))
	    {
	      float ampl = ahc_hitEnergy[i];
	      float time = ahc_hitTime[i];

	      if(time_T0.count(K) == 0)
		{
		  std::vector<float> vec;
		  vec.resize(2);
		  vec.push_back(time);
		  time_T0.insert(make_pair(K, vec));
		}
	      else
		time_T0[K].push_back(time);
	    }
	}

      //Filling histograms
      for(std::map<int, std::vector<float> >::iterator it = time_T0.begin(); it != time_T0.end(); ++it)
	{
	  int Layer = it->first;
	  std::vector<float> time_vec = it->second;
	  hCorrelationT0[Layer-1]->Fill(time_vec.at(0), time_vec.at(1));
	}
    }

  //Lock Thread for writting to rootfile
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("T0_Correlation");
  pArchive->WriteElement(m_histoList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("T0 Correlation done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));

  //Clean the memory
  TIter next(m_histoList);
  TObject *obj;
  while (obj = next())
    obj->Delete();

  delete m_histoList;
  delete tree;
}*/

//-----------------------------------------------------------------------------------------------

/*
 * T0 Difference
 * Represent the difference of T0s/Cherenkov within the same layer
 */

 /*void AnalysisThread::T0_Difference()
{
  emit log("MESSAGE", "T0 Difference started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //Open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
	
  //Get TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare tree variables
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  //Set Branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  //Create TList for holding the histograms
  TList *m_histoList = new TList();
  m_histoList->SetName("T0 Difference List");

  //Create histograms
  std::vector<TH1F*> hdT0(nLayer);
  for(std::map<int, std::vector<int> >::iterator it = m_map_T0s.begin(); it != m_map_T0s.end(); ++it)
    {
      int Layer = it->first;
      hdT0[Layer-1] = new TH1F(TString::Format("hDiffT0s_Layer%i", Layer), TString::Format("hDiffT0s_Layer%i", Layer), 4000, -2000, 2000);
      m_histoList->Add(hdT0[Layer-1]);
    }

  //map with T0s time
  std::map<int, std::vector<float> > time_T0;

  //Loop over the roofile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      //case no hits
      if(ahc_nHits == 0) continue;
      //count number of events
      m_maxevents = std::max(m_maxevents, eventNumber);

      time_T0.clear();

      for (int i = 0; i < ahc_nHits; i++)
	{
	  int I = ahc_hitI[i];
	  int J = ahc_hitJ[i];
	  int K = ahc_hitK[i];

	  if(isT0(I, J, K))
	    {
	      float ampl = ahc_hitEnergy[i];
	      float time = ahc_hitTime[i];

	      if(time_T0.count(K) == 0)
		{
		  std::vector<float> vec;
		  vec.resize(2);
		  vec.push_back(time);
		  time_T0.insert(make_pair(K, vec));
		}
	      else
		time_T0[K].push_back(time);
	    }
	}

      for(std::map<int, std::vector<float> >::iterator it = time_T0.begin(); it != time_T0.end(); ++it)
	{
	  int Layer = it->first;
	  std::vector<float> time_vec = it->second;
	  hdT0[Layer-1]->Fill(time_vec.at(0) - time_vec.at(1));
	}
    }

  //Lock Thread for writting to rootfile
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("T0_Difference");
  pArchive->WriteElement(m_histoList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("T0 Difference done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));

  //Clean memory
  TIter next(m_histoList);
  TObject *obj;
  while(obj = next())
    obj->Delete();

  delete m_histoList;
  delete tree;
}*/

//-----------------------------------------------------------------------------------------------

/*
 * T0 Sum
 * Represent the correlation of all the T0s against one of them
 */

  /*void AnalysisThread::T0_Sum()
{
  emit log("MESSAGE", "T0 Sum started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //Open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
	
  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare variables
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  //Create TList for histograms
  TList *m_histoList = new TList();
  m_histoList->SetName("T0 Sum List");

  //Number of T0s
  int nT0s = 0;
  for(std::map<int, std::vector<int> >::iterator it = m_map_T0s.begin(); it != m_map_T0s.end(); ++it)
    {
      nT0s += it->second.size();
    }

  //Create histograms
  std::vector<TH2F*> hSumT0(nT0s);
  int i = 0;
  for(std::map<int, std::vector<int> >::iterator it = m_map_T0s.begin(); it != m_map_T0s.end(); ++it)
    {
      int Layer = it->first;
      int iT0 = 1;
      for(std::vector<int>::iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	{
	  hSumT0[i] = new TH2F(TString::Format("hSumT0s_Layer%i_T0%i", Layer, iT0), TString::Format("hSumT0s_Layer%i_T0%i", Layer, iT0), 4096, 0, 4096, 4096, 0, 4096);
	  m_histoList->Add(hSumT0[i]);
	  i++;
	  iT0++;
	}
    }

  //map with T0s time
  std::map<int, std::vector<float> > time_T0;

  //Loop over the roofile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      //case no hits
      if(ahc_nHits == 0) continue;
      //count number of events
      m_maxevents = std::max(m_maxevents, eventNumber);

      time_T0.clear();

      for (int i = 0; i < ahc_nHits; i++)
	{
	  int I = ahc_hitI[i];
	  int J = ahc_hitJ[i];
	  int K = ahc_hitK[i];

	  if(isT0(I, J, K))
	    {
	      float ampl = ahc_hitEnergy[i];
	      float time = ahc_hitTime[i];

	      if(time_T0.count(K) == 0)
		{
		  std::vector<float> vec;
		  vec.resize(2);
		  vec.push_back(time);
		  time_T0.insert(make_pair(K, vec));
		}
	      else
		time_T0[K].push_back(time);
	    }
	}
    }

  //Locking of the roofile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("T0_Sum");
  pArchive->WriteElement(m_histoList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("T0 Sum done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));

  //Clean memory
  TIter next(m_histoList);
  TObject *obj;
  while(obj = next())
    obj->Delete();

  delete m_histoList;
  delete tree;
}*/

//-----------------------------------------------------------------------------------------------

/*
 * HitTime
 * Creates histogram of hit times for all hits
 */

void AnalysisThread::HitTime()
{
  emit log("MESSAGE", "HitTime started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0; 

  //Open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }

  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree"); 

  //Declare variables
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  /*EBU
  int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in one event
  Int_t emc_nHits;*/
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  //Booking of histogram
  DMAHCALBooker *booker = new DMAHCALBooker("HitTime");
  booker->Book1DHistograms("Hit_Time", 10000, -5000, 5000);
  booker->SetAxis("1D", "Hit_Time [ns]", "# Entries");

  //Create TList
  TList *m_histoList = new TList();
  m_histoList = booker->GetObjects("1D");
  m_histoList->SetName("HitTime List");

  float HitTime = 0;
  //Loop over rootfile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      //Reset Hit Time  at the beginning of event
      HitTime = 0;

      //case no hits
      if(ahc_nHits == 0) continue;
      m_maxevents = std::max(m_maxevents, eventNumber);

      //Count number of T0s
      int nT0 = NumberOfT0(ahc_hitI, ahc_hitJ,  ahc_hitK, ahc_hitEnergy, ahc_hitTime, ahc_nHits);
      if (nT0 < nT0s) continue;

      //Cut on number of Hits
      if(ahc_nHits < nMinHits) continue;
      if(ahc_nHits > nMaxHits) continue;

      //Loop over the hits
      for(int i = 0; i < ahc_nHits; i++)
	{
	  int layer = ahc_hitK[i];
	  float time = ahc_hitTime[i];
	  //if(layer > nLayer) continue;

	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;

	  HitTime = time;

	  //Fill Hit Time histogram
	  TIter next(m_histoList);
	  TObject *obj;

	  while ((obj = next()))
	  {
	    if(obj->InheritsFrom("TH1"))
	    {
	      TH1F* pHisto = static_cast<TH1F*>(obj);
	      pHisto->Fill(HitTime);
	    }
	  }
	}
    }


  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("Hit_Time");
  pArchive->WriteElement(m_histoList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("HitTime done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));
  booker->deleteLater();
  delete m_histoList;
  delete tree;
 
}

//-----------------------------------------------------------------------------------------------

/*
 * Energy Sum
 * Calculates the energy sum within one event and fill a histogram
 */

void AnalysisThread::Esum()
{
  emit log("MESSAGE", "Esum started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //Open roofile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
  
  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  emit log("DEBUG", "After TTree creation");
  
  //Declare variables
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  /*EBU
  int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in one event
  Int_t emc_nHits;*/
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  /*if(EBU)
    {
      tree->SetBranchStatus("emc_nHits", 1);
      tree->SetBranchStatus("emc_hitI", 1);
      tree->SetBranchStatus("emc_hitJ", 1);
      tree->SetBranchStatus("emc_hitK", 1);
      tree->SetBranchStatus("emc_hitEnergy", 1);
      tree->SetBranchStatus("emc_hitTime", 1);
      }*/

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  /*if(EBU)
    {
      tree->SetBranchAddress("emc_nHits", &emc_nHits);
      tree->SetBranchAddress("emc_hitI", &emc_hitI);
      tree->SetBranchAddress("emc_hitJ", &emc_hitJ);
      tree->SetBranchAddress("emc_hitK", &emc_hitK);
      tree->SetBranchAddress("emc_hitEnergy", &emc_hitEnergy);
      tree->SetBranchAddress("emc_hitTime", &emc_hitTime);
      }*/

  //Booking of histogram
  DMAHCALBooker *booker = new DMAHCALBooker("Esum");

  if(MIP)
    {
      booker->Book1DHistograms("Energy_Sum", 300, 0, 150);
      booker->SetAxis("1D", "Energy [MIP]", "# Entries");
    }
  else
    {
      booker->Book1DHistograms("Energy_Sum", 1000, 0, 10000);
      booker->SetAxis("1D", "Energy [ADC]", "# Entries");
    }

  //Create TList
  TList *m_histoList = new TList();
  m_histoList = booker->GetObjects("1D");
  m_histoList->SetName("Esum List");

  float SumEnergy = 0;
  //Loop over rootfile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      //Reset Energy Sum at the beginning of event
      SumEnergy = 0;

      //case no hits
      if(ahc_nHits == 0) continue;
      m_maxevents = std::max(m_maxevents, eventNumber);

      //Count number of T0s
      int nT0 = NumberOfT0(ahc_hitI, ahc_hitJ,  ahc_hitK, ahc_hitEnergy, ahc_hitTime, ahc_nHits);
      if (nT0 < nT0s) continue;

      //Cut on number of Hits
      if(ahc_nHits < nMinHits) continue;
      if(ahc_nHits > nMaxHits) continue;

      //Loop over the hits
      for(int i = 0; i < ahc_nHits; i++)
	{
	  int layer = ahc_hitK[i];
	  float ampl = ahc_hitEnergy[i];
	  //if(layer > nLayer) continue;

	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;

	  if(MIP)
	    {
	      if(ampl > m_MIPcut)
		{
		  SumEnergy += ampl;
		}
	    }
	  else
	    {
	      if(ampl > EstimateMIP(layer)*m_MIPcut)
		{
		  SumEnergy += ampl;
		}
	    }
	}

      //EBU Handling
      /*if(EBU)
	{
	  for(int i = 0; i < emc_nHits; i++)
	    {
	      int layer = emc_hitK[i];
	      float ampl = ahc_hitEnergy[i];
	      //if(layer >= nLayer) continue;

	      if(MIP)
		{
		  if(ampl > m_MIPcut)
		    {
		      SumEnergy += ampl;
		    }
		}
	      else
		{
		  if(ampl > m_MIPcut*EstimateMIP(layer))
		    {
		      SumEnergy += ampl;
		    }
		}

	    }
	    }*/

      //Fill Sum energy histogram
      if(SumEnergy != 0)
	{
	  TIter next(m_histoList);
	  TObject *obj;

	  while ((obj = next()))
	    {
	      if(obj->InheritsFrom("TH1"))
		{
		  TH1F* pHisto = static_cast<TH1F*>(obj);
		  pHisto->Fill(SumEnergy);
		}
	    }
	}
    }

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("Energy_Sum");
  pArchive->WriteElement(m_histoList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("Energy Sum done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));
  booker->deleteLater();
  delete m_histoList;
  delete tree;
}

//-----------------------------------------------------------------------------------------------

/*
 * Energy per Channel
 * Calculates the energy sum within one event and fills a histogram per single channel
 */

void AnalysisThread::EnergyChannel()
{
  emit log("MESSAGE", "Energy per Channel  started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
	
  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");
  Int_t numberOfEvents = tree->GetEntries();

  //Declare variable
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  /*EBU
  int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in an event
  Int_t emc_nHits;*/
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  /*if(EBU)
    {
      tree->SetBranchStatus("emc_nHits", 1);
      tree->SetBranchStatus("emc_hitI", 1);
      tree->SetBranchStatus("emc_hitJ", 1);
      tree->SetBranchStatus("emc_hitK", 1);
      tree->SetBranchStatus("emc_hitEnergy", 1);
      tree->SetBranchStatus("emc_hitTime", 1);
      }*/

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  /*if(EBU)
    {
      tree->SetBranchAddress("emc_nHits", &emc_nHits);
      tree->SetBranchAddress("emc_hitI", &emc_hitI);
      tree->SetBranchAddress("emc_hitJ", &emc_hitJ);
      tree->SetBranchAddress("emc_hitK", &emc_hitK);
      tree->SetBranchAddress("emc_hitEnergy", &emc_hitEnergy);
      tree->SetBranchAddress("emc_hitTime", &emc_hitTime);
      }*/

  //Booking of histogram for all layers
  DMAHCALBooker *booker = new DMAHCALBooker("EnergyPerChannel");

  std::vector<TProfile*> pProfile(nLayer);
  std::vector<TGraph*> pErrorGraph(nLayer);
  TList *m_energyProfileList = new TList();
  m_energyProfileList->SetName("Profile Energy Channel List");
  TList *m_rmsGraphList = new TList();
  m_rmsGraphList->SetName("Graph RMS Channel List");

  std::string name = "EnergyPerChannel_Layer";
  for (int layerNumber = 1; layerNumber < nLayer+1; layerNumber++)
    {
      std::stringstream s;
      s << layerNumber;
      name += s.str();
      Double_t min = 0.5 + 576*(layerNumber-1);
      Double_t max = 576.5 + 576*(layerNumber-1); 
      pProfile[layerNumber-1] = new TProfile(name.c_str(), name.c_str(), 576, min, max);
      pProfile[layerNumber-1]->GetXaxis()->SetTitle("Channel number");
      pProfile[layerNumber-1]->GetYaxis()->SetTitle("Energy [MIP]/Number of events");
      pProfile[layerNumber-1]->BuildOptions(0,0,"s");
      m_energyProfileList->Add(pProfile[layerNumber-1]);
      name = "EnergyPerChannel_Layer";
      pErrorGraph[layerNumber-1] = new TGraph(576);
    }

  //Loop over roofile
  for(int n = 0; n < numberOfEvents; n++)
    {
      tree->GetEntry(n);
      m_maxevents = std::max(m_maxevents, eventNumber);

      //T0 criteria
      int nT0 = NumberOfT0(ahc_hitI, ahc_hitJ,  ahc_hitK, ahc_hitEnergy, ahc_hitTime, ahc_nHits);
      if (nT0 < nT0s) continue;

      //Cut on number of hits
      if(ahc_nHits < nMinHits) continue;
      if(ahc_nHits > nMaxHits) continue;

      for(int i = 0; i < ahc_nHits; i++)
	{
	  int layerID = ahc_hitK[i];
	  if(layerID > nLayer) continue;
	  int ChpChn = GetChipChn(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]); 
	  int chipID = ChpChn/100;
	  int channelID = ChpChn%100;
	  int channel = (layerID-1)*576+chipID*36+channelID;
	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;
	  float ampl = ahc_hitEnergy[i];

	  if(MIP)
	    {
	      if(ampl > m_MIPcut)
		{
		  pProfile[layerID-1]->Fill(channel+1, ampl, 1);
		}
	    }
	  else
	    {
	      if(ampl > m_MIPcut*EstimateMIP(layerID))
		{
		  pProfile[layerID-1]->Fill(channel+1, ampl, 1);
		}
	    }
	}
    }

  std::string name2 = "RMSPerChannel_Layer";
  for(int number = 0; number < nLayer; number++)
    {
      for(int iChannel = 576*number; iChannel < 576*(number+1); iChannel++)
	{
	  pProfile[number]->SetBinEntries(iChannel+1-number*576,numberOfEvents);
	  Double_t error = pProfile[number]->GetBinError(iChannel+1-number*576);
	  pErrorGraph[number]->SetPoint(iChannel-number*576,iChannel+1,error);
	}
      std::stringstream s;
      s << number+1;
      name2 += s.str();
      pErrorGraph[number]->SetNameTitle(name2.c_str(), name2.c_str());
      pErrorGraph[number]->SetMarkerStyle(20);
      pErrorGraph[number]->SetMarkerSize(0.7);
      pErrorGraph[number]->SetMarkerColor(1);
      pErrorGraph[number]->SetLineColor(0);
      pErrorGraph[number]->GetXaxis()->SetTitle("Channel number");
      pErrorGraph[number]->GetYaxis()->SetTitle("RMS [MIP]");
      pErrorGraph[number]->GetXaxis()->SetRangeUser(number*576,577+number*576);
      m_rmsGraphList->Add(pErrorGraph[number]);
      name2 = "RMSPerChannel_Layer";
      pProfile[number]->BuildOptions(0,0,"");      
    }

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("EnergyPerChannel");
  pArchive->WriteElement(m_energyProfileList);
  pArchive->mkdir("RMSPerChannel");
  pArchive->WriteElement(m_rmsGraphList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("Energy Per Channel check done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));

  booker->deleteLater();
  //delete m_profileList;
  delete m_energyProfileList;
  delete m_rmsGraphList;
  for(int layerNumber = 0; layerNumber < nLayer; layerNumber++)
    {
      delete pProfile[layerNumber];
      delete pErrorGraph[layerNumber];
    }
  delete tree;
}





//-----------------------------------------------------------------------------------------------

/*
 * Energy per Layer
 * Calculates the energy sum within one event and fill an histogram per layer
 */

/*void AnalysisThread::EnergyCell()
{
  emit log("MESSAGE", "Energy per Layer started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
	
  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare variable
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  //EBU
  int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in an event
  Int_t emc_nHits;
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  if(EBU)
    {
      tree->SetBranchStatus("emc_nHits", 1);
      tree->SetBranchStatus("emc_hitI", 1);
      tree->SetBranchStatus("emc_hitJ", 1);
      tree->SetBranchStatus("emc_hitK", 1);
      tree->SetBranchStatus("emc_hitEnergy", 1);
      tree->SetBranchStatus("emc_hitTime", 1);
    }

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  if(EBU)
    {
      tree->SetBranchAddress("emc_nHits", &emc_nHits);
      tree->SetBranchAddress("emc_hitI", &emc_hitI);
      tree->SetBranchAddress("emc_hitJ", &emc_hitJ);
      tree->SetBranchAddress("emc_hitK", &emc_hitK);
      tree->SetBranchAddress("emc_hitEnergy", &emc_hitEnergy);
      tree->SetBranchAddress("emc_hitTime", &emc_hitTime);
    }

  //Booking of histogram for all layers
  DMAHCALBooker *booker = new DMAHCALBooker("EnergyperLayer");

  std::string hname = "Energy_Layer_";
  booker->Book1DHistograms(hname, nLayer, 80, -0.5, 4);
  booker->SetAxis("1D", "Energy [MIP]", "# Entries");

  //Create TList and get histograms
  TList *m_histoList = new TList();
  m_histoList = booker->GetObjects("1D");
  m_histoList->SetName("EnergyperLayer List");

  TIter next(m_histoList);
  TObject *obj;
  std::vector<TH1F*> pHisto(nLayer);
  int index = 0;

  while ((obj = next()))
    {
      pHisto[index] = static_cast<TH1F*>(obj);
      index++;
    }

  std::map<int, std::map<int, TH1F*> > pHistoCell;

  //Loop over roofile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      m_maxevents = std::max(m_maxevents, eventNumber);

      //T0 criteria
      int nT0 = NumberOfT0(ahc_hitI, ahc_hitJ,  ahc_hitK, ahc_hitEnergy, ahc_hitTime, ahc_nHits);
      if (nT0 < nT0s) continue;

      //Cut on number of hits
      if(ahc_nHits < nMinHits) continue;
      if(ahc_nHits > nMaxHits) continue;

      for(int i = 0; i < ahc_nHits; i++)
	{
	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;

	  float ampl = ahc_hitEnergy[i];
	  if(ahc_hitK[i] > nLayer) continue;

	  if(ampl < 0.5) continue;

	  //Fill AHCAL histos
	   pHisto[ahc_hitK[i]-1]->Fill(ampl);

	  int ChipChn = GetChipChn(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);

	  if(pHistoCell[ahc_hitK[i]-1].count(ChipChn) == 0)
	    {
	      TString histoname = "SpectrumLayer_";
	      histoname += ahc_hitK[i];
	      histoname += "_ChipChn";
	      histoname += ChipChn;
	      pHistoCell[ahc_hitK[i]-1][ChipChn] = new TH1F(histoname, histoname, 80, -0.5, 4);
	    }

	  pHistoCell[ahc_hitK[i]-1][ChipChn]->Fill(ampl);

	}
    }

  TList *m_cellHisto = new TList();
  for(std::map<int, std::map<int, TH1F*> >::iterator it = pHistoCell.begin(); it !=  pHistoCell.end(); it++)
    {
      for(std::map<int, TH1F*>::iterator itHisto = it->second.begin(); itHisto != it->second.end(); itHisto++)
	{
	  m_cellHisto->Add(itHisto->second);
	}
    }

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("EnergyPerLayer");
  pArchive->WriteElement(m_histoList);
  pArchive->mkdir("EnergyPerCell");
  pArchive->WriteElement(m_cellHisto);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("EnergyPerLayer check done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));
  booker->deleteLater();
  delete m_histoList;
  delete tree;
  delete m_cellHisto;
  pHistoCell.clear();
  for(int i = 0; i < nLayer; i++)
    delete pHisto[i];
}*/

//-----------------------------------------------------------------------------------------------

/*
 * Number of Hits per Layer
 * Calculates the number of hits within one event over a certain threshold and fill an histogram per layer + TProfile over all layers
 */

void AnalysisThread::Hits()
{
  emit log("MESSAGE", "Hits started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //Open rootfile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }

  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare variables
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  //EBU
  /*int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in one event
  Int_t emc_nHits;*/
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  /*if(EBU)
    {
      tree->SetBranchStatus("emc_nHits", 1);
      tree->SetBranchStatus("emc_hitI", 1);
      tree->SetBranchStatus("emc_hitJ", 1);
      tree->SetBranchStatus("emc_hitK", 1);
      tree->SetBranchStatus("emc_hitEnergy", 1);
      tree->SetBranchStatus("emc_hitTime", 1);
      }*/

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  /*if(EBU)
    {
      tree->SetBranchAddress("emc_nHits", &emc_nHits);
      tree->SetBranchAddress("emc_hitI", &emc_hitI);
      tree->SetBranchAddress("emc_hitJ", &emc_hitJ);
      tree->SetBranchAddress("emc_hitK", &emc_hitK);
      tree->SetBranchAddress("emc_hitEnergy", &emc_hitEnergy);
      tree->SetBranchAddress("emc_hitTime", &emc_hitTime);
      }*/

  //Booking of histograms (TProfile per layer)
  DMAHCALBooker *booker = new DMAHCALBooker("Hits");

  booker->BookProfile("NProf_0.25MIP", nLayer+1, -0.5, nLayer+0.5);
  booker->BookProfile("NProf_0.50MIP", nLayer+1, -0.5, nLayer+0.5);

  booker->Book1DHistograms("NHits_0.5MIP_Layer", nLayer, 50, -0.5, 50.5);

  //Create TProfile list
  TList *m_profileList = new TList();
  m_profileList = booker->GetObjects("Profile");
  m_profileList->SetName("Profile NHits List");

  //Create histo list
  TList *m_HistoList = new TList();
  m_HistoList = booker->GetObjects("1D");
  m_HistoList->SetName("Histo NHits List");

  std::vector<int> nhitover25(nLayer);
  std::vector<int> nhitover50(nLayer);

  //Loop over roofile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      for(int iLayer = 0; iLayer < nLayer; iLayer++)
	{
	  nhitover25[iLayer] = 0;
	  nhitover50[iLayer] = 0;
	}

      m_maxevents = std::max(m_maxevents, eventNumber);

      for(int i = 0; i < ahc_nHits; i++)
	{
	  float ampl_ahc = ahc_hitEnergy[i];
	  int layer = ahc_hitK[i];

	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;

	  if(ahc_hitK[i] > nLayer) continue;

	  if(MIP)
	    {
	      if(ampl_ahc > m_MIPcut/2)
		nhitover25[layer-1]++;
	      if(ampl_ahc > m_MIPcut)
		nhitover50[layer-1]++;
	    }
	  else
	    {
	      if(ampl_ahc > m_MIPcut*EstimateMIP(layer)/2)
		nhitover25[layer-1]++;
	      if(ampl_ahc > m_MIPcut*EstimateMIP(layer))
		nhitover50[layer-1]++;
	    }
	}

      /*if(EBU)
	{
	  for(int i = 0; i < emc_nHits; i++)
	    {
	      float ampl_emc = emc_hitEnergy[i];
	      int layer = emc_hitK[i];

	      if(emc_hitK[i] > nLayer) continue;

	      if(MIP)
		{
		  if(ampl_emc > m_MIPcut/2)
		    nhitover25[layer-1]++;
		  if(ampl_emc > m_MIPcut)
		    nhitover50[layer-1]++;
		}
	      else
		{
		  if(ampl_emc > m_MIPcut*EstimateMIP(layer)/2)
		    nhitover25[layer-1]++;
		  if(ampl_emc > m_MIPcut*EstimateMIP(layer))
		    nhitover50[layer-1]++;
		}
	    }
	    }*/

      TIter next(m_HistoList);
      TObject *obj;
      std::vector<TH1F*> pHisto(nLayer);
      int index = 0;

      //Fetch histograms and filling
      while ((obj = next()))
	{
	  pHisto[index] = static_cast<TH1F*>(obj);
	  pHisto[index]->GetYaxis()->SetTitle("#Entries");
	  pHisto[index]->GetXaxis()->SetTitle("Number of Hits over 0.5 MIPs");
	  pHisto[index]->Fill(nhitover50[index]);
	  index++;
	}

      TIter next2(m_profileList);
      while ((obj = next2()))
	{
	  if(strcmp(obj->GetName(), "NProf_0.25MIP") == 0)
	    {
	      TProfile *prof = static_cast<TProfile*>(obj);
	      prof->GetXaxis()->SetTitle("Layer number");
	      prof->GetYaxis()->SetTitle("Number of Hits over 0.25 MIPs");
	      for(int iLayer = 0; iLayer < nLayer; iLayer++)
		{
		  prof->Fill(iLayer+1, nhitover25[iLayer], 1);
		}
	    }
	  if(strcmp(obj->GetName(), "NProf_0.50MIP") == 0)
	    {
	      TProfile *prof2 = static_cast<TProfile*>(obj);
	      prof2->GetXaxis()->SetTitle("Layer Number");
	      prof2->GetYaxis()->SetTitle("Number of Hits over 0.50 MIPs");
	      for(int iLayer = 0; iLayer < nLayer; iLayer++)
		{
		  prof2->Fill(iLayer+1, nhitover50[iLayer], 1);
		}
	    }
	}
    }

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("NHits_Profile");
  pArchive->WriteElement(m_profileList);
  pArchive->GetRunDir()->cd();
  pArchive->mkdir("NHits_Layer");
  pArchive->WriteElement(m_HistoList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("Hits check done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));
  booker->deleteLater();
  delete m_HistoList;
  delete m_profileList;
  delete tree;
}

//-----------------------------------------------------------------------------------------------

/*
 * Shower Module
 * Create divers variables inherent for shower development and beam characteristics
 */

void AnalysisThread::Shower()
{
  emit log("MESSAGE", "Shower started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //Open roofile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }


  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare variables
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitPos[MAXCELL][3];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  //EBU
  /*int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitPos[MAXCELL][3];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in one event
  Int_t emc_nHits;*/
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);
  tree->SetBranchStatus("ahc_hitPos", 1);

  /*if(EBU)
    {
      tree->SetBranchStatus("emc_nHits", 1);
      tree->SetBranchStatus("emc_hitI", 1);
      tree->SetBranchStatus("emc_hitJ", 1);
      tree->SetBranchStatus("emc_hitK", 1);
      tree->SetBranchStatus("emc_hitEnergy", 1);
      tree->SetBranchStatus("emc_hitTime", 1);
      tree->SetBranchStatus("emc_hitPos", 1);
      }*/

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);
  tree->SetBranchAddress("ahc_hitPos", &ahc_hitPos);

  /*if(EBU)
    {
      tree->SetBranchAddress("emc_nHits", &emc_nHits);
      tree->SetBranchAddress("emc_hitI", &emc_hitI);
      tree->SetBranchAddress("emc_hitJ", &emc_hitJ);
      tree->SetBranchAddress("emc_hitK", &emc_hitK);
      tree->SetBranchAddress("emc_hitEnergy", &emc_hitEnergy);
      tree->SetBranchAddress("emc_hitTime", &emc_hitTime);
      tree->SetBranchAddress("emc_hitPos", &emc_hitPos);
      }*/

  //Booking of histograms
  DMAHCALBooker *booker = new DMAHCALBooker("Shower");

  //Get Moliere Radius to scale R histogram
  float MoliereRadius = 1.;
  if(m_absorber == "Tungsten")
    MoliereRadius = 0.93*10; //mm
  if(m_absorber == "Iron")
    MoliereRadius = 1.71*10; //mm

  //Energy Profile
  booker->BookProfile("Profile_EnergyPerLayer", nLayer+1, -0.5, nLayer+1.5);
  //Center of gravity in X
  booker->Book1DHistograms("MeanX", 360, -360, 360);
  //Center of gravity in Y
  booker->Book1DHistograms("MeanY", 360, -360, 360);
  //Center of gravity in Z
  booker->Book1DHistograms("MeanZ", 1000, 0, 1000);
  //Shower radius
  booker->Book1DHistograms("MeanR", 50, -10/MoliereRadius, 400/MoliereRadius);
  //Center of gravity in X versus Center of gravity in Y
  booker->Book2DHistograms("MeanX_vs_MeanY", 24, -360, 360);
  //Shower radius versus center of gravity in Z
  //booker->Book2DHistograms("MeanR vs MeanZ", 400, 0, 800);

  //Create list 1D histo
  TList *m_histo1DList = new TList();
  m_histo1DList = booker->GetObjects("1D");
  m_histo1DList->SetName("Mean pos List");

  //Create list TProfile
  TList *m_profileList = new TList();
  m_profileList = booker->GetObjects("Profile");
  m_profileList->SetName("Profile Energy List");

  //Create list 2D histo
  TList *m_histo2DList = new TList();
  m_histo2DList = booker->GetObjects("2D");
  m_histo2DList->SetName("Position comparison List");

  std::vector<float> sumEnergy(nLayer);
  float cogx, cogy, cogz = 0;
  float SumE  = 0;
  float SumXE, SumYE, SumZE =0;
  float MeanR = 0;
  float SumRE = 0;

  //Loop over roofile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      cogx = 0; cogy = 0; cogz = 0;
      SumE = 0;
      SumXE =0; SumYE = 0; SumZE = 0;
      SumRE = 0; MeanR = 0;
      for(int iLayer = 0; iLayer < nLayer; iLayer++)
	{
	  sumEnergy[iLayer] = 0;
	}
      m_maxevents = std::max(m_maxevents, eventNumber);

      //T0 criteria
      int nT0 = NumberOfT0(ahc_hitI, ahc_hitJ,  ahc_hitK, ahc_hitEnergy, ahc_hitTime, ahc_nHits);
      if (nT0 < nT0s) continue;

      //Cut on number of hits
      if(ahc_nHits < nMinHits) continue;
      if(ahc_nHits > nMaxHits) continue;

      for(int i = 0; i < ahc_nHits; i++)
	{
	  int layer = ahc_hitK[i];
	  if(ahc_hitK[i] > nLayer) continue;

	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;
	  float ampl = ahc_hitEnergy[i];

	  if(MIP)
	    {
	      if(ampl > m_MIPcut)
		{
		  //sum energy per layer
		  sumEnergy[layer-1] += ampl;
		  //total sum of energy
		  SumE += ampl;
		  //position sum energy weighted
		  SumXE += ampl*ahc_hitPos[i][0];
		  SumYE += ampl*ahc_hitPos[i][1];
		  SumZE += ampl*ahc_hitPos[i][2];
		}
	    }
	  else
	    {
	      if(ampl > m_MIPcut*EstimateMIP(layer))
		{
		  sumEnergy[layer-1] += ampl;
		  SumE += ampl;
		  SumXE += ampl*ahc_hitPos[i][0];
		  SumYE += ampl*ahc_hitPos[i][1];
		  SumZE += ampl*ahc_hitPos[i][2];
		}
	    }
	}
      /*if(EBU)
	{
	  for(int i = 0; i < emc_nHits; i++)
	    {
	      int layer = emc_hitK[i]-1;
	      float ampl = emc_hitEnergy[i];
	      if(emc_hitK[i] > nLayer) continue;

	      if(MIP)
		{
		  if(ampl > m_MIPcut)
		    {
		      sumEnergy[layer-1] += ampl;
		      SumE += ampl;
		      SumXE += ampl*emc_hitPos[i][0];
		      SumYE += ampl*emc_hitPos[i][1];
		      SumZE += ampl*emc_hitPos[i][2];
		    }
		}
	      else
		{
		  if(ampl > m_MIPcut*EstimateMIP(layer))
		    {
		      sumEnergy[layer-1] += ampl;
		      SumE += ampl;
		      SumXE += ampl*emc_hitPos[i][0];
		      SumYE += ampl*emc_hitPos[i][1];
		      SumZE += ampl*emc_hitPos[i][2];
		    }
		}
	    }
	    }*/

      //Calculate COG in one event
      cogx = SumXE/SumE;
      cogy = SumYE/SumE;
      cogz = SumZE/SumE;

      //Calcul of shower radius
      for(int i = 0; i < ahc_nHits; i++)
	{
	  int layer = ahc_hitK[i]-1;
	  if(ahc_hitK[i] > nLayer) continue;

	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;
	  float ampl = ahc_hitEnergy[i];

	  if(MIP)
	    {
	      if(ampl > m_MIPcut)
		{
		  //Radius energy weighted
		  SumRE += TMath::Sqrt(pow((ahc_hitPos[i][0] - cogx), 2) + pow((ahc_hitPos[i][1] - cogy), 2))*ampl;
		}
	    }
	  else
	    {
	      if(ampl > m_MIPcut*EstimateMIP(layer))
		{
		  SumRE += TMath::Sqrt(pow((ahc_hitPos[i][0] - cogx), 2) + pow((ahc_hitPos[i][1] - cogy), 2))*ampl;
		}
	    }
	}
      /*if(EBU)
	{
	  for(int i = 0; i < emc_nHits; i++)
	    {
	      int layer = emc_hitK[i];
	      if(emc_hitK[i] > nLayer) continue;
	      float ampl = emc_hitEnergy[i];
	      if(MIP)
		{
		  if(ampl > m_MIPcut)
		    {
		      SumRE += TMath::Sqrt(pow((emc_hitPos[i][0] - cogx), 2) + pow((emc_hitPos[i][1] - cogy), 2))*ampl;
		    }
		}
	      else
		{
		  if(ampl > m_MIPcut*EstimateMIP(layer))
		    {
		      SumRE += TMath::Sqrt(pow((emc_hitPos[i][0] - cogx), 2) + pow((emc_hitPos[i][1] - cogy), 2))*ampl;
		    }
		}
	    }
	    }*/

      //Calculate Radius for the event
      MeanR = SumRE/SumE;

      //Fill histograms
      TIter next(m_histo1DList);
      TObject *obj;
      int index = 0;
      while((obj = next()))
	{
	  TH1F* pHisto = static_cast<TH1F*>(obj);
	  if(index == 0)
	    {
	      pHisto->Fill(cogx);
	      pHisto->GetXaxis()->SetTitle("Cog X [mm]");
	      pHisto->GetYaxis()->SetTitle("# Entries");
	    }
	  if(index == 1)
	    {
	      pHisto->Fill(cogy);
	      pHisto->GetXaxis()->SetTitle("Cog Y [mm]");
	      pHisto->GetYaxis()->SetTitle("# Entries");
	    }
	  if(index == 2)
	    {
	      pHisto->Fill(cogz);
	      pHisto->GetXaxis()->SetTitle("Cog Z [mm]");
	      pHisto->GetYaxis()->SetTitle("# Entries");
	    }
	  if(index == 3)
	    {
	      if(MeanR != 0)
		pHisto->Fill(MeanR/MoliereRadius);
	      pHisto->GetXaxis()->SetTitle("Mean R [R_{M}]");
	      pHisto->GetYaxis()->SetTitle("# Entries");
	    }
	  index++;
	}

      //Fill 2D histo
      TIter next2(m_histo2DList);
      while((obj = next2()))
	{
	  TH2F* pHisto = static_cast<TH2F*>(obj);
	  pHisto->Fill(cogx, cogy);
	  pHisto->GetXaxis()->SetTitle("Cog X [mm]");
	  pHisto->GetYaxis()->SetTitle("Cog Y [mm]");
	  pHisto->SetDrawOption("COLZ");
	}

      //Fill TProfile
      TIter next3(m_profileList);
      while((obj = next3()))
	{
	  TProfile* prof = static_cast<TProfile*>(obj);
	  for(int iLayer = 0; iLayer < nLayer; iLayer++)
	    {
	      prof->Fill(iLayer+1, sumEnergy[iLayer], 1);
	      prof->GetXaxis()->SetTitle("Layer Number");
	      prof->GetYaxis()->SetTitle("Energy sum [MIP]/Number of Events");
	    }
	}
    }

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("Shower");
  pArchive->WriteElement(m_histo1DList);
  pArchive->WriteElement(m_histo2DList);
  pArchive->WriteElement(m_profileList);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("Shower check done : Treated %1 events in %2 secs").arg(QString::number(m_maxevents), QString::number(m_time)));

  booker->deleteLater();
  delete m_histo1DList;
  delete m_histo2DList;
  delete m_profileList;
  delete tree;
}

//-----------------------------------------------------------------------------------------------

/*
 * HitMap Module
 * Create a hit map for each layer with no selection
 */

void AnalysisThread::HitMap()
{
  emit log("MESSAGE", "HitMap started");

  //start timer
  timer.start();
  int m_maxevents = 0;
  double m_time = 0;

  //open roofile
  pAnalysis->OpenTFile(m_Rootfile, "READ");

  if(!pAnalysis->isOpened())
    {
      emit log("ERROR", QString("Can't open TFile : %1").arg(QString::fromStdString(m_Rootfile)));
      return;
    }
	
  //Declare TTree
  TTree *tree = (TTree*)pAnalysis->GetTree("bigtree");

  //Declare variables
  //HBU
  Int_t runNumber;
  Int_t eventNumber;
  int ahc_hitI[MAXCELL];
  int ahc_hitJ[MAXCELL];
  int ahc_hitK[MAXCELL];
  Float_t ahc_hitEnergy[MAXCELL];
  Float_t ahc_hitTime[MAXCELL];
  //EBU
  /*int emc_hitI[MAXCELL];
  int emc_hitJ[MAXCELL];
  int emc_hitK[MAXCELL];
  Float_t emc_hitEnergy[MAXCELL];
  Float_t emc_hitTime[MAXCELL];
  //Number of hits in one event
  Int_t emc_nHits;*/
  Int_t ahc_nHits;

  //Enable/disable branches
  tree->SetBranchStatus("*", 0);
  tree->SetBranchStatus("runNumber", 1);
  tree->SetBranchStatus("eventNumber", 1);
  tree->SetBranchStatus("ahc_nHits", 1);
  tree->SetBranchStatus("ahc_hitI", 1);
  tree->SetBranchStatus("ahc_hitJ", 1);
  tree->SetBranchStatus("ahc_hitK", 1);
  tree->SetBranchStatus("ahc_hitEnergy", 1);
  tree->SetBranchStatus("ahc_hitTime", 1);

  /*if(EBU)
    {
      tree->SetBranchStatus("emc_nHits", 1);
      tree->SetBranchStatus("emc_hitI", 1);
      tree->SetBranchStatus("emc_hitJ", 1);
      tree->SetBranchStatus("emc_hitK", 1);
      tree->SetBranchStatus("emc_hitEnergy", 1);
      tree->SetBranchStatus("emc_hitTime", 1);
      }*/

  //Declare branches
  tree->SetBranchAddress("runNumber", &runNumber);
  tree->SetBranchAddress("eventNumber", &eventNumber);
  tree->SetBranchAddress("ahc_nHits", &ahc_nHits);
  tree->SetBranchAddress("ahc_hitI", &ahc_hitI);
  tree->SetBranchAddress("ahc_hitJ", &ahc_hitJ);
  tree->SetBranchAddress("ahc_hitK", &ahc_hitK);
  tree->SetBranchAddress("ahc_hitEnergy", &ahc_hitEnergy);
  tree->SetBranchAddress("ahc_hitTime", &ahc_hitTime);

  /*if(EBU)
    {
      tree->SetBranchAddress("emc_nHits", &emc_nHits);
      tree->SetBranchAddress("emc_hitI", &emc_hitI);
      tree->SetBranchAddress("emc_hitJ", &emc_hitJ);
      tree->SetBranchAddress("emc_hitK", &emc_hitK);
      tree->SetBranchAddress("emc_hitEnergy", &emc_hitEnergy);
      tree->SetBranchAddress("emc_hitTime", &emc_hitTime);
      }*/

  //Create histograms
  std::vector<TH2I*> pHisto(nLayer);
  std::vector<TH2I*> pEnergyHisto(nLayer);
  for(int ilayer = 0; ilayer < nLayer; ilayer++)
    {
      TString hname = "Map_Layer";
      hname += ilayer+1;

      TString henergyname = "Energy_Map_Layer";
      henergyname += ilayer+1;

      //SPECIFIC TO DESY May 2016
      pHisto[ilayer] = new TH2I(hname, hname, 24, 0.5, 24.5, 24, 0.5, 24.5);
      pHisto[ilayer]->GetXaxis()->SetTitle("I");
      pHisto[ilayer]->GetYaxis()->SetTitle("J");
      pHisto[ilayer]->SetStats(0);

      pEnergyHisto[ilayer] = new TH2I(henergyname, henergyname, 24, 0.5, 24.5, 24, 0.5, 24.5);
      pEnergyHisto[ilayer]->GetXaxis()->SetTitle("I");
      pEnergyHisto[ilayer]->GetYaxis()->SetTitle("J");
      pEnergyHisto[ilayer]->SetStats(0);
    }

  //Create list
  TList *m_histoMap = new TList();
  m_histoMap->SetName("HitMap List");
  TList *m_histoEnergyMap = new TList();
  m_histoEnergyMap->SetName("HitMap_Energy List");
  //Loop over rootfile
  for(int n = 0; n < tree->GetEntries(); n++)
    {
      tree->GetEntry(n);
      m_maxevents = std::max(m_maxevents, eventNumber);

      for(int i = 0; i < ahc_nHits; i++)
	{
	  int I = ahc_hitI[i];
	  int J = ahc_hitJ[i];
	  int layer = ahc_hitK[i];
	  if(ahc_hitK[i] > nLayer) continue;

	  //bool isT0channel = isT0(ahc_hitI[i], ahc_hitJ[i], ahc_hitK[i]);
	  //if (isT0channel) continue;

	  //inverse filling because reverse axis /*Energy weighted */
	  pHisto[layer-1]->Fill(25 - I , J, 1);
	  pEnergyHisto[layer-1]->Fill(25 - I , J, ahc_hitEnergy[i]);
	}
    }

  //Add histo to the list
  for(int ilayer = 0; ilayer < nLayer; ilayer++)
    {
      m_histoMap->Add(pHisto[ilayer]);
      m_histoEnergyMap->Add(pEnergyHisto[ilayer]);
    }

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("HitMap");
  pArchive->WriteElement(m_histoMap);
  pArchive->mkdir("HitMap_Energy");
  pArchive->WriteElement(m_histoEnergyMap);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("HitMap check done in %1 secs").arg(QString::number(m_time)));
  delete m_histoMap;
  delete m_histoEnergyMap;
  for(int ilayer = 0; ilayer < nLayer; ilayer++)
    {
      delete pHisto[ilayer];
      delete pEnergyHisto[ilayer];
    }
  delete tree;
}

//-----------------------------------------------------------------------------------------------

/*
 * Temperature Module
 * Create a TGraph per layer containing the Mean Temperature of the module (on txt file)
 */

/*
  void AnalysisThread::Temperature()
  {
  emit log("MESSAGE", "Temperature started");

  //start timer
  timer.start();
  double m_time = 0;

  //Create list
  TList *m_histoTGraph = new TList();
  m_histoTGraph->SetName("TGraph List");

  std::string _data = m_InputFiletxt;
  ifstream file_to_read;
  file_to_read.open(_data.c_str());
  if (file_to_read == NULL){
  emit log("ERROR", QString("File %1 does not exist").arg(QString::fromStdString(_data)));
  return;
  }

  //Declare variables
  std::string line;
  std::string strTemperature("LDA:");
  size_t foundTemperature = 0;
  std::string TSensor[50][10];
  std::string lda[50], port[50];
  int index = 0;

  //Declare TGraph
  TGraphErrors* pGraph[50];
  for(int i = 0; i < 50; i++)
  {
  pGraph[i] = new TGraphErrors();
  }

  float MeanTemp[50][50] = {0};
  float TempPWR[50][50] = {0};
  int ipoint = -1;

  //looping over the txt file
  while (!file_to_read.eof())
  {
  getline(file_to_read, line);
  if(line[0] == '#')
  {
  //find temperature line
  foundTemperature = line.find(strTemperature);
  if (foundTemperature!=std::string::npos)
  {
  if(index == 0)
  ipoint++;

  std::stringstream strstr;
  strstr.str(line);

  //getting line
  getline(strstr, TSensor[index][0], '\t');
  lda[index] = TSensor[index][0].substr(7,1);
  port[index] = TSensor[index][0].substr(14,1);

  strstr >> TSensor[index][1] >> TSensor[index][2] >> TSensor[index][3] >> TSensor[index][4] >> TSensor[index][5] >> TSensor[index][6] >> TSensor[index][7] >> TSensor[index][8];

  //int LDANr = atoi(lda[index].c_str());
  //int PortNr = atoi(port[index].c_str());
  int T1 = atoi(TSensor[index][1].c_str());
  int T2 = atoi(TSensor[index][2].c_str());
  int T3 = atoi(TSensor[index][3].c_str());
  int T4 = atoi(TSensor[index][4].c_str());
  int T5 = atoi(TSensor[index][5].c_str());
  int T6 = atoi(TSensor[index][6].c_str());
  //int TDIF = atoi(TSensor[index][7].c_str());
  int TPWR = atoi(TSensor[index][8].c_str());

  MeanTemp[ipoint][index] = (T1 + T2 + T3 + T4 + T5 + T6)/60.;
  TempPWR[ipoint][index] = TPWR;

  pGraph[index]->SetPoint(ipoint, ipoint, MeanTemp[ipoint][index]);
  pGraph[index]->GetXaxis()->SetTitle("Measurement number in single Run");
  pGraph[index]->GetYaxis()->SetTitle("<T> [degrees]");
  index++;
  }

  if(foundTemperature==std::string::npos)
  {
  index = 0;
  }
  }
  else
  {
  continue;
  }
  }

  //Add graphs to list
  for(int i = 0; i < 50; i++)
  {
  if(pGraph[i]->GetN() > 0)
  {
  TString title = "Temp_Graph_Layer_";
  title += i+1;
  pGraph[i]->SetNameTitle(title, title);
  pGraph[i]->SetMarkerStyle(20);
  pGraph[i]->SetMarkerSize(1.3);
  pGraph[i]->SetMarkerColor(kBlue+i);
  m_histoTGraph->Add(pGraph[i]);
  }
  }

  file_to_read.close();

  //Lock rootfile to write
  mutex.lock();
  pArchive->OpenTFile(m_ArchiveName, "UPDATE");
  emit log("DEBUG", QString("Writing to Archive File : %1").arg(QString::fromStdString(m_ArchiveName)));

  std::string m_dirName = "Run_";
  m_dirName += m_runNumber;

  pArchive->MakeRoot(m_dirName);
  pArchive->mkdir("Temperature");
  pArchive->WriteElement(m_histoTGraph);
  pArchive->close();
  mutex.unlock();

  m_time = GetElapsedTime()/1000.;

  emit log("DEBUG", QString("Temperature check done in %1 secs").arg(QString::number(m_time)));
  delete m_histoTGraph;

  for(int i = 0; i < 50; i++)
  {
  delete pGraph[i];
  }
  }
*/
