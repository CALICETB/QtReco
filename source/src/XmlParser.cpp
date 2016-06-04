#include "XmlParser.h"
#include <QDebug>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "DMAHCAL.h"

XmlParser::XmlParser()
{
    //Parameter variable
    parser = new Parameters();
}

//-----------------------------------------------------------------------------------------------

XmlParser::XmlParser(const XmlParser &obj)
{
    parser = obj.parser;
}

//-----------------------------------------------------------------------------------------------

XmlParser::~XmlParser()
{
    emit log("DEBUG","XmlParser : destroyed");
    delete parser;
}

//-----------------------------------------------------------------------------------------------

//Configure function called by main GUI
void XmlParser::configure()
{
    emit log("DEBUG", "XmlHelper : Reading Settings");
    this->readSettings("../xml/steering.xml");
    PrintSettings();
    emit finished();
}

//-----------------------------------------------------------------------------------------------

//To get attribute in xml file
const char* XmlParser::getAttribute(TiXmlElement *pXmlElement, std::string attributeName)
{
    if(NULL == pXmlElement)
        return 0;

    return pXmlElement->Attribute(attributeName.c_str());
}

//-----------------------------------------------------------------------------------------------

//Reading settings in xml file and save it in memory
void XmlParser::readSettings(const std::string &xmlfilename)
{
    std::string RunNumber, Input_PATH, Output_PATH, Archivefile, Absorber;
    bool EBU, MIP_Conversion, Ped_Substraction, test;
    int nlayer, MinHits, MaxHits, nT0s;
    double MIPCut;

    //load xml file
    TiXmlDocument xmlDocument(xmlfilename.c_str());
    bool loadOkay = xmlDocument.LoadFile();

    if (loadOkay)
    {
        //Finding the root
        TiXmlElement* root = xmlDocument.RootElement();
        if(strcmp(root->Value(), "DMAHCAL") != 0)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml root <DMAHCAL> in xml file");
            return;
        }

        //Read Settings Element
        TiXmlElement *const pSettingsElement = root->FirstChildElement("Settings");
        if(NULL == pSettingsElement)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml element <Settings> in xml file");
            return;
        }

        for(TiXmlElement *pXmlElement = pSettingsElement->FirstChildElement("parameter"); NULL != pXmlElement; pXmlElement = pXmlElement->NextSiblingElement("parameter"))
        {
            std::string parameterName;

            parameterName = getAttribute(pXmlElement,"name");

            if(parameterName == "Analysis_Only")
            {
                if(strcmp(getAttribute(pXmlElement,"value"), "true") == 0)
                    test = true;
                else
                    test = false;
            }

        }

        //Read Mapping Element
        TiXmlElement *const pMappingElement = root->FirstChildElement("Mapping");
        if(NULL == pMappingElement)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml element <Mapping> in xml file");
            return;
        }

        std::string Mapfile;
        for(TiXmlElement *pXmlElement = pMappingElement->FirstChildElement("parameter"); NULL != pXmlElement; pXmlElement = pXmlElement->NextSiblingElement("parameter"))
        {
            std::string parameterName;
            parameterName = getAttribute(pXmlElement,"name");
            if(parameterName == "Filename")
            {
                Mapfile = getAttribute(pXmlElement,"value");
            }
        }

        //Reading T0s Element
        TiXmlElement *const pT0Element = root->FirstChildElement("T0s");
        if(NULL == pT0Element)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml element <T0s> in xml file");
            return;
        }

        std::map<int, std::vector<int> > mapT0s;
        std::string Number_T0s, T0VecString;
        int index = 0;

        for(TiXmlElement *pXmlElement = pT0Element->FirstChildElement("parameter"); NULL != pXmlElement; pXmlElement = pXmlElement->NextSiblingElement("parameter"))
        {
            std::string parameterName;

            parameterName = getAttribute(pXmlElement,"name");

            if(parameterName == "nT0s")
	      {
                Number_T0s = getAttribute(pXmlElement,"value");
	      }
	    else
	      {
		T0VecString = getAttribute(pXmlElement,"value");

		std::vector<std::string> tokens;
		std::string delimiter = " ";
		DMAHCAL::tokenize(T0VecString, tokens, delimiter);

		int Layer = atoi(tokens.at(0).c_str());
		int Chip = atoi(tokens.at(1).c_str());
		int Chn = atoi(tokens.at(2).c_str());

		//std::cout << Layer << "\t" << Chip << "\t" << Chn << std::endl;

		int indexChipChn = Chip*100+Chn;
		if(mapT0s.count(Layer) == 0)
		  {
		    std::vector<int> vec;
		    vec.push_back(indexChipChn);
		    mapT0s.insert(std::make_pair(Layer, vec));
		    index++;
		  }
		else
		  {
		    mapT0s[Layer].push_back(indexChipChn);
		    index++;
		  }

		//std::cout << mapT0s.size() << std::endl;
	      }
        }

        if(index == atoi(Number_T0s.c_str()))
            emit log("DEBUG", QString("XmlHelper: Number of T0s %1").arg(QString::fromStdString(Number_T0s)));
        else{
            emit log("ERROR", QString("XmlHelper: Number of T0s %1 does not match %2").arg(QString::fromStdString(Number_T0s), QString::number(index)));
            return;
        }

        //Reading Steering Element
        TiXmlElement *const pSteeringElement = root->FirstChildElement("Steering");
        if(NULL == pSteeringElement)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml element <Steering> in xml file");
            return;
        }

        for(TiXmlElement *pXmlElement = pSteeringElement->FirstChildElement("parameter"); NULL != pXmlElement; pXmlElement = pXmlElement->NextSiblingElement("parameter"))
        {
            std::string parameterName;

            parameterName = getAttribute(pXmlElement,"name");

            if(parameterName == "Absorber")
            {
                Absorber = getAttribute(pXmlElement,"value");
            }

            if(parameterName == "Run")
            {
                RunNumber = getAttribute(pXmlElement,"value");
            }
            if(parameterName == "Input_PATH")
            {
                Input_PATH = getAttribute(pXmlElement,"value");
            }
            if(parameterName == "Output_PATH")
            {
                Output_PATH = getAttribute(pXmlElement,"value");
            }
            if(parameterName == "Pedestal")
            {
                if(strcmp(getAttribute(pXmlElement,"value"), "true") == 0)
                    Ped_Substraction = true;
                else
                    Ped_Substraction = false;
            }
            if(parameterName == "ADCtoMIP")
            {
                if(strcmp(getAttribute(pXmlElement,"value"), "true") == 0)
                    MIP_Conversion = true;
                else
                    MIP_Conversion = false;
            }
        }

        //Reading Storage Element
        TiXmlElement *const pStorageElement = root->FirstChildElement("Storage");
        if(NULL == pStorageElement)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml element <Storage> in xml file");
            return;
        }

        for(TiXmlElement *pXmlElement = pStorageElement->FirstChildElement("parameter"); NULL != pXmlElement; pXmlElement = pXmlElement->NextSiblingElement("parameter"))
        {
            std::string parameterName;

            parameterName = getAttribute(pXmlElement,"name");

            if(parameterName == "File")
            {
                Archivefile = getAttribute(pXmlElement,"value");
            }
        }

        //Reading Analysis Element
        TiXmlElement *const pAnalysisElement = root->FirstChildElement("Analysis");
        if(NULL == pAnalysisElement)
        {
            emit log("ERROR", "XmlHelper: Couldn't find xml element <Analysis> in xml file");
            return;
        }

        for(TiXmlElement *pXmlElement = pAnalysisElement->FirstChildElement("parameter"); NULL != pXmlElement; pXmlElement = pXmlElement->NextSiblingElement("parameter"))
        {
            std::string parameterName;

            parameterName = getAttribute(pXmlElement,"name");

            if(parameterName == "NLayer")
            {
                nlayer = atoi(getAttribute(pXmlElement,"value"));
            }
            if(parameterName == "NMinHits")
            {
                MinHits = atoi(getAttribute(pXmlElement,"value"));
            }
            if(parameterName == "NMaxHits")
            {
                MaxHits = atoi(getAttribute(pXmlElement,"value"));
            }
            if(parameterName == "With_EBU")
            {
                if(strcmp(getAttribute(pXmlElement,"value"), "true") == 0)
                    EBU = true;
                else
                    EBU = false;
            }
            if(parameterName == "MIP_Cut")
            {
                MIPCut = atof(getAttribute(pXmlElement,"value"));
            }
	    if(parameterName == "nT0s")
            {
                nT0s = atoi(getAttribute(pXmlElement,"value"));
            }
        }

        //Save parameters in structure
        parser->SetRunNumber(RunNumber);
        parser->SetInputFile(Input_PATH);
        parser->SetRootfile(Output_PATH);
        parser->SetSlciofile(Output_PATH);
        parser->SetArchiveFile(Archivefile);
        parser->SetT0Map(mapT0s);
	parser->SetnT0s(nT0s);
        parser->SetPedestalSubstraction(Ped_Substraction);
        parser->SetMIPConversion(MIP_Conversion);
        parser->SetNLayer(nlayer);
        parser->SetMinNHits(MinHits);
        parser->SetMaxNHits(MaxHits);
        parser->SetEBU(EBU);
        parser->SetMIPCut(MIPCut);
        parser->SetMode(test);
        parser->SetMapName(Mapfile);
        parser->SetAbsorber(Absorber);

        emit log("DEBUG", "Module XmlHelper : read settings done");
    }
    else
    {
        emit log("ERROR", QString("XmlHelper: Couldn't load input xml file %1").arg(loadOkay));
        return;
    }
}

//-----------------------------------------------------------------------------------------------

void XmlParser::PrintSettings()
{
    if(parser->GetMode())
        emit log("VERBOSE", "Running in Analysis Mode");
    else{
        emit log("VERBOSE", "Running in Normal Mode");
    }

    emit log("VERBOSE", "Module XmlHelper : Settings");
    emit log("VERBOSE", QString("Run# %1 in %2 -- Storage: %3").arg(QString::fromStdString(parser->GetRunNumber()), QString::fromStdString(parser->GetRootfile()), QString::fromStdString(parser->GetArchive())));
    emit log("VERBOSE", QString("Number of Layer %1, EBU %2, ADC to MIP %3, Pedestal Substraction %4, MIP cut %5").arg(QString::number(parser->GetNLayer()), QString::number(parser->HasEBU()), QString::number(parser->HasADCtoMIP()), QString::number(parser->HasPedSubstraction()), QString::number(parser->GetMIPCut())));
}
