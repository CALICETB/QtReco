/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
 */

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include "tinyxml.h"
#include <map>
#include <vector>

/*
 * Xml Parser Class
 * Parsing of the xml steering file using the tinyxml library.
 * Keep in memory all parameters needed for reconstruction and analysis
 */

class XmlParser : public QObject
{
  Q_OBJECT
    public:

  //Parameter Structure
  struct Parameters
  {
    //Important parameter for steering
    const std::string GetRunNumber() {return m_Run_number;}
    //Give Input txt file
    const std::string GetInputFile() {return m_Input_file;}
    //Give Output Rootfile
    const std::string GetRootfile() {return m_Output_root;}
    //Give Output slcio file
    const std::string GetSlciofile() {return m_Output_slcio;}
    //Give Filename where plots are save
    const std::string GetArchive() {return m_Archive_file;}
    //Give Mapping filename
    const std::string GetMapFile() {return m_map_file;}
    //Pedestal Substraction flag
    bool HasPedSubstraction() {return m_Ped_Substraction;}
    //MIP Conversion flag
    bool HasADCtoMIP() {return m_ADCtoMIP;}
    //Give Number of Layers
    int GetNLayer() {return m_NLayer;}
    //Give cut on minimum number of hits in an event
    int GetMinHits() {return m_nMinHits;}
    //Give cut on maximum number of hits in an event
    int GetMaxHits() {return m_nMaxHits;}
    //EBU presence flag
    bool HasEBU() {return m_isEBU;}
    //Give Zero suppresion cut value
    double GetMIPCut() {return m_MIPCut;}
    //Running mode flag
    bool GetMode() {return m_mode;}
    //Get number of T0s used for analysis
    int GetnT0s() {return m_nT0s;}
    //T0 Mapping (Layer, ChipChn)
    std::map<int, std::vector<int> > GetT0Map() {return m_mapT0;}
    //Return absorber
    std::string GetAbsorber() {return m_absorber;}

    //Set Parameter functions
    //Setting Input file
    void SetInputFile(std::string Input) {m_Input_file = Input;}
    //Setting Rootfile file
    void SetRootfile(std::string rootfile) {m_Output_root = rootfile;}
    //Setting slcio file
    void SetSlciofile(std::string slciofile) {m_Output_slcio = slciofile;}
    //Setting RunNumber
    void SetRunNumber(std::string Run_Number) {m_Run_number = Run_Number;}
    //Setting Archive path
    void SetArchiveFile(std::string Archivefile) {m_Archive_file = Archivefile;}
    //Setting T0 Map
    void SetT0Map(std::map<int, std::vector<int> > mapT0s) {m_mapT0 = mapT0s;}
    //Setting Ped Substraction flag
    void SetPedestalSubstraction(bool Ped_Substraction) {m_Ped_Substraction = Ped_Substraction;}
    //Setting MIP Conversion flag
    void SetMIPConversion(bool MIP_Conversion) {m_ADCtoMIP = MIP_Conversion;}
    //Setting Number of layers
    void SetNLayer(int nlayer) {m_NLayer = nlayer;}
    //Setting Minimum number of hits per events
    void SetMinNHits(int MinHits) {m_nMinHits = MinHits;}
    //Setting Maximum number of hits per events
    void SetMaxNHits(int MaxHits) {m_nMaxHits = MaxHits;}
    //Setting EBU flag
    void SetEBU(bool EBU) {m_isEBU = EBU;}
    //Setting MIP Cut value
    void SetMIPCut(double MIPCut) {m_MIPCut = MIPCut;}
    //Setting Running Mode (Marlin/Analysis only)
    void SetMode(bool test) {m_mode = test;}
    //Setting Map file name
    void SetMapName(std::string Mapfile) {m_map_file = Mapfile;}
    //Setting Absorber type
    void SetAbsorber(std::string Absorber) {m_absorber = Absorber;}
    //Setting number of T0s for analysis
    void SetnT0s(int nT0s) {m_nT0s = nT0s;}

    //Variables parameters
    std::string m_Run_number, m_Input_file, m_Output_root, m_Output_slcio, m_Archive_file, m_map_file, m_absorber;
    int m_NLayer, m_nMinHits, m_nMaxHits, m_nT0s;
    bool m_isEBU, m_ADCtoMIP, m_mode, m_Ped_Substraction;
    double m_MIPCut;
    std::map<int, std::vector<int> > m_mapT0;
  };

  //Constructor
  XmlParser();
  //Copy constructor
  XmlParser(const XmlParser& obj);
  //Destructor
  ~XmlParser();
  //Get Attribute tinyxml library
  const char *getAttribute(TiXmlElement *pXmlElement, std::string attributeName);
  //Called by Main GUI
  void configure();
  //Read xml file function
  void readSettings(const std::string &xmlfilename);
  //Print xml settings
  void PrintSettings();

  //Parameter structure
  Parameters *parser;

 signals:
  //Signal finished
  void finished();
  void log(QString type, QString message);
};

#endif // XMLPARSER_H

//-------------------------------------------------------------------------------------------------------------------------------
