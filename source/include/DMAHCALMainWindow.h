/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef DMAHCALMAINWINDOW_H
#define DMAHCALMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QStringList>

#include "DMAHCAL.h"
#include "ApplicationModuleApi.h"

//Typedef for map containing parameters, ui display
typedef std::map<QString, QString> ParameterMap;

namespace Ui {
class DMAHCALMainWindow;
}

/*
 * Data Monitoring AHCAL Main Window Class
 * Main UI Interface for the Online Monitoring
 * Handles every user actions
*/

class DMAHCALMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    //Constructor
    explicit DMAHCALMainWindow(QWidget *parent = 0);
    //Destructor
    ~DMAHCALMainWindow();

signals :
    //Signal to Logegr
    void log(QString type, QString message);

public slots:
    //Update UI
    void UpdateGUI();
    //Update buttons states
    void UpdateButtons();
    //Update UI Parameters
    void UpdateGUI(ParameterMap map);
    //Action to configure run
    void Configure();
    //Quit
    void Quit();

public:

private:
    //Global pointer for ui
    Ui::DMAHCALMainWindow *ui;
    //Pointer for general parameter of DMAHCAL
    DMAHCAL *m_hcal;
    //Pointer to dispatcher module
    ApplicationModuleApi *m_dispatcher;
    bool isConfigured;
    QTimer *timer;
};

#endif // DMAHCALMAINWINDOW_H
