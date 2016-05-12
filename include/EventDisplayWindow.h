/*
 * Author Eldwan Brianne
 * July 2015
 * PhD DESY
*/

#ifndef EVENTDISPLAYWINDOW_H
#define EVENTDISPLAYWINDOW_H

#include <QMainWindow>

#include "XmlParser.h"

namespace Ui {
class EventDisplayWindow;
}

/*
 * EventDisplayWindow Class
 * GUI for rendering of the EventDisplay OGL
 * Interacts with the user for displaying events
*/

class EventDisplayWindow : public QMainWindow
{
    Q_OBJECT
public:
    //Constructor
    explicit EventDisplayWindow(XmlParser *xml, QWidget *parent = 0);
    //Destructor
    ~EventDisplayWindow();

public slots:
    //Start Marlin for event display
    void Start();
    //Close the window
    void Close();

private:
    //Parameter pointer
    XmlParser *parser;
    //Global pointer for ui
    Ui::EventDisplayWindow *ui;
};

#endif // EVENTDISPLAYWINDOW_H
