#include "EventDisplayWindow.h"
#include "ui_EventDisplayWindow.h"

/****** TO BE IMPLEMENTED **********/

EventDisplayWindow::EventDisplayWindow(XmlParser *xml, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EventDisplayWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(), this->height());

    //copy of parser pointer
    parser = xml;

    connect(ui->Quit, SIGNAL(clicked()), this, SLOT(Close()));
    connect(ui->Start, SIGNAL(clicked()), this, SLOT(Start()));
}

//-----------------------------------------------------------------------------------------------

EventDisplayWindow::~EventDisplayWindow()
{
    delete ui;
    parser->deleteLater();
}

//-----------------------------------------------------------------------------------------------

void EventDisplayWindow::Start()
{
    //Start Marlin with QProcess
}

//-----------------------------------------------------------------------------------------------

void EventDisplayWindow::Close()
{
    //Close the window
    this->close();
}
