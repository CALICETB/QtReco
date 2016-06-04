#include "SedModule.h"

#include <QDir>
#include <QDebug>
#include <string>

SedModule::SedModule(XmlParser *settings)
{
    //copy of Parser pointer
    xml = settings;
    Input_PATH = "";
    Run_number = "";
    Input_file = "";
    Output_PATH = "";
}

//-----------------------------------------------------------------------------------------------

SedModule::~SedModule()
{
    emit log("DEBUG", "Module Sed : destroyed");
    //xml->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void SedModule::init()
{
    //init pointers and Get parameters from parser
    process_sed = new QProcess();
    Run_number = xml->parser->GetRunNumber();
    Input_PATH = xml->parser->GetInputFile();
    Output_PATH = xml->parser->GetRootfile();
    PedSubs = xml->parser->HasPedSubstraction();
    ADC_Conversion = xml->parser->HasADCtoMIP();
    emit log("DEBUG", QString("SedModule : %1 %2 %3").arg(QString::fromStdString(Run_number), QString::fromStdString(Input_PATH), QString::fromStdString(Output_PATH)));
}

//-----------------------------------------------------------------------------------------------

void SedModule::start()
{
    //Look for txt file in directory
    QDir dir(Input_PATH.c_str());
    QStringList filter;
    filter << "*.txt" << "*.slcio";
    dir.setNameFilters(filter);
    //filter, only file, no links
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    std::string temp_str;

    //look for file corresponding to Run looked for
    QStringList fileList = dir.entryList();
    for (int i=0; i<fileList.count(); i++)
    {
        std::string file = fileList[i].toStdString();
        if(file.find(Run_number.c_str()) != std::string::npos)
        {
            emit log("VERBOSE", QString("SedModule : Found file: %1").arg(fileList[i]));
            temp_str = fileList[i].toStdString();
            Input_file = dir.absolutePath().toStdString();
            Input_file += "/";
            Input_file += fileList[i].toStdString();
        }
    }

    //case file not found
    if(Input_file == "")
    {
        emit log("ERROR", "SedModule : File not found");
        return;
    }

    //List of arguments for QProcess
    QStringList args;
    std::string Output_root = Output_PATH;
    Output_root += "/";
    Output_root += temp_str.substr(0, temp_str.find_last_of("."));
    Output_root += ".root";
    std::string Output_slcio = Output_PATH;
    Output_slcio += "/";
    Output_slcio += temp_str.substr(0, temp_str.find_last_of("."));
    Output_slcio += ".slcio";

    std::string str_Pedestal;
    if(PedSubs == 0)
        str_Pedestal = "false";
    if(PedSubs == 1)
        str_Pedestal = "true";

    std::string str_ADCtoMIP;
    if(ADC_Conversion == 0)
        str_ADCtoMIP = "false";
    if(ADC_Conversion == 1)
        str_ADCtoMIP = "true";

    std::string script = "../scripts/Reco_sed.sh";
    //Arguments : scripts, Run, Input, Rootfile, slciofile, Ped_Sub, MIP_Conv
    args << QString::fromStdString(script) << QString::fromStdString(Run_number) << QString::fromStdString(Input_file) << QString::fromStdString(Output_root) << QString::fromStdString(Output_slcio) << QString::fromStdString(str_Pedestal) << QString::fromStdString(str_ADCtoMIP);

    emit log("DEBUG", QString("SedModule : Run : %1").arg(QString::fromStdString(Run_number)));
    emit log("DEBUG", QString("SedModule : Input : %1").arg(QString::fromStdString(Input_file))) ;
    emit log("DEBUG", QString("SedModule : Outputs : %1 %2").arg(QString::fromStdString(Output_root), QString::fromStdString(Output_slcio)));

    //re-set xmlParameters!!
    xml->parser->SetInputFile(Input_file);
    xml->parser->SetRootfile(Output_root);
    xml->parser->SetSlciofile(Output_slcio);

    //Start QProcess in same terminal
    process_sed->start("/bin/bash", args);
    //case QProcess can t start
    if(!process_sed->waitForStarted())
    {
        emit log("ERROR", QString("Problem is sed QProcess : %1").arg(process_sed->errorString()));
        return;
    }
    else
    {
        process_sed->waitForFinished(-1);
        emit log("DEBUG", "SedModule : Sed done!");
        emit finished();
        return;
    }
}

//-----------------------------------------------------------------------------------------------

void SedModule::clean()
{
    //delete QProcess
    process_sed->deleteLater();
}
