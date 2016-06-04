#include "DMAHCALMainWindow.h"
#include "ui_DMAHCALMainWindow.h"

#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <map>

DMAHCALMainWindow::DMAHCALMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DMAHCALMainWindow)
{
    ui->setupUi(this);

    //Set General parameters
    m_hcal = new DMAHCAL();
    m_hcal->SetVersion("1.3");
    m_hcal->SetDetector("AHCAL");

    //instance of dispatcher
    m_dispatcher = new ApplicationModuleApi();
    isConfigured = false;

    //Dispatcher signals
    connect(m_dispatcher,SIGNAL(started()), this, SLOT(UpdateGUI()));
    connect(m_dispatcher,SIGNAL(stopped()), this, SLOT(UpdateButtons()));
    connect(m_dispatcher, SIGNAL(update(ParameterMap)), this, SLOT(UpdateGUI(ParameterMap)));
    connect(this, SIGNAL(log(QString,QString)), m_dispatcher, SLOT(RedirectMessage(QString,QString)));

    //Button signals
    connect(ui->Quit, SIGNAL(clicked()), this, SLOT(Quit()));
    connect(ui->Steering, SIGNAL(clicked()), this, SLOT(Configure()));
    connect(ui->Start, SIGNAL(clicked()), m_dispatcher, SLOT(Start()));
    connect(ui->Stop, SIGNAL(clicked()), m_dispatcher, SLOT(Stop()));
    connect(ui->Event_Display, SIGNAL(clicked()), this, SLOT(Open_EventDisplay()));

    //Update UI timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateGUI()));
    timer->start(10);
}

//----------------------------------------------------------------------------------

DMAHCALMainWindow::~DMAHCALMainWindow()
{
    emit log("DEBUG","DMAHCALMainWindow : destroyed");
    delete ui;
    delete m_hcal;
    delete m_dispatcher;
    timer->deleteLater();
}

//----------------------------------------------------------------------------------

void DMAHCALMainWindow::Configure()
{
    //Call method in dispatcher to configure the run
    m_dispatcher->Configure();
    isConfigured = true;
    //Update UI
    this->UpdateGUI();
}

//----------------------------------------------------------------------------------

void DMAHCALMainWindow::Quit()
{
    //Close the dispatcher & main window
    m_dispatcher->Quit();
    if(!m_dispatcher->isRunning())
        close();
}

//-----------------------------------------------------------------------------------

void DMAHCALMainWindow::UpdateGUI()
{
    //Update buttons
    if(isConfigured)
    {
        //Case Marlin Running
        if(m_dispatcher->isRunning())
        {
            ui->Start->setEnabled(false);
            ui->Stop->setEnabled(false);
            ui->Steering->setEnabled(false);
            ui->Quit->setEnabled(false);
        }
        else
        {
            ui->Start->setEnabled(true);
            ui->Stop->setEnabled(false);
            ui->Steering->setEnabled(true);
            ui->Quit->setEnabled(true);
        }
    }
    //Case idle
    else
    {
        ui->Start->setEnabled(false);
        ui->Stop->setEnabled(false);
        ui->Steering->setEnabled(true);
        ui->Quit->setEnabled(true);
    }
}

//-----------------------------------------------------------------------------------

void DMAHCALMainWindow::UpdateButtons()
{
    //Update buttons after end of dispatcher
    ui->Start->setEnabled(true);
    ui->Stop->setEnabled(false);
    ui->Steering->setEnabled(true);
}

//-----------------------------------------------------------------------------------

void DMAHCALMainWindow::UpdateGUI(ParameterMap map)
{
    //Update UI Parameters
    for(std::map<QString, QString>::iterator it = map.begin(); it != map.end(); it++)
    {
        //Run Number
        if(it->first == "Run")
            ui->Runnumber->setText(it->second);
        //Input file
        if(it->first == "Input")
            ui->PathIn->setText(it->second);
        //Output roofile/slcio
        if(it->first == "Output")
            ui->PathOut->setText(it->second);
        //Flag MIP Conversion
        if(it->first == "MIP")
            ui->MIP_Conversion_value->setText(it->second);
        //Flag Pedestal Substraction
        if(it->first == "Ped")
            ui->PedSub_value->setText(it->second);
        //Mapping filename
        if(it->first == "MapIJK")
            ui->MapIJK_value->setText(it->second);
        //Number of Clients connected
        if(it->first == "ClientNumber")
            ui->NClients->setText(it->second);

        //Number of Layers
        if(it->first == "NLayer")
            ui->NLayer_value->setText(it->second);
        //EBU Flag
        if(it->first == "EBU")
	  {
	    if(it->second == "0")
	      ui->With_EBU_value->setText("No");
	    else
	      ui->With_EBU_value->setText("Yes");
	  }
        //MIP Cut value
        if(it->first == "MIPCut")
            ui->MIP_Cut_Value->setText(it->second.append(" MIP"));
        //Running Mode
        if(it->first == "AnalysisMode")
        {
            if(it->second == "0")
            {
                ui->Mode_value->setText("OFF");
                QString win_title = "DMAHCAL v";
                win_title.append(QString::fromStdString(m_hcal->GetVersion()));
                this->setWindowTitle(win_title);
            }
            else
            {
                ui->Mode_value->setText("<font color='Red'>ON</font>");
                this->setWindowTitle("DMAHCAL (Analysis Only)");
            }
        }
        //Value mininum hits
        if(it->first == "NMinHits")
            ui->NMinHits_value->setText(it->second);
        //Value maximum hits
        if(it->first == "NMaxHits")
            ui->NMaxHits_value->setText(it->second);
	if(it->first == "nT0s")
            ui->nT0s_value->setText(it->second);

    }
}

//------------------------------------------------------------------------------------

void DMAHCALMainWindow::Open_EventDisplay()
{
    emit log("DEBUG", "Event Display not yet available!");
    m_dispatcher->OpenDEH();
}
