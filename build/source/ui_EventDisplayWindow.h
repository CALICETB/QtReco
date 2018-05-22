/********************************************************************************
** Form generated from reading UI file 'EventDisplayWindow.ui'
**
** Created: Sat May 19 14:40:30 2018
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EVENTDISPLAYWINDOW_H
#define UI_EVENTDISPLAYWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
#include <glced.h>

QT_BEGIN_NAMESPACE

class Ui_EventDisplayWindow
{
public:
    QWidget *centralwidget;
    QPushButton *Quit;
    QPushButton *Start;
    QPushButton *Next_Ev;
    GLCED *glced;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *EventDisplayWindow)
    {
        if (EventDisplayWindow->objectName().isEmpty())
            EventDisplayWindow->setObjectName(QString::fromUtf8("EventDisplayWindow"));
        EventDisplayWindow->resize(1238, 999);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EventDisplayWindow->sizePolicy().hasHeightForWidth());
        EventDisplayWindow->setSizePolicy(sizePolicy);
        centralwidget = new QWidget(EventDisplayWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        Quit = new QPushButton(centralwidget);
        Quit->setObjectName(QString::fromUtf8("Quit"));
        Quit->setGeometry(QRect(1040, 860, 115, 31));
        Start = new QPushButton(centralwidget);
        Start->setObjectName(QString::fromUtf8("Start"));
        Start->setGeometry(QRect(1050, 70, 115, 31));
        Next_Ev = new QPushButton(centralwidget);
        Next_Ev->setObjectName(QString::fromUtf8("Next_Ev"));
        Next_Ev->setGeometry(QRect(1050, 140, 115, 31));
        glced = new GLCED(centralwidget);
        glced->setObjectName(QString::fromUtf8("glced"));
        glced->setGeometry(QRect(40, 40, 950, 870));
        sizePolicy.setHeightForWidth(glced->sizePolicy().hasHeightForWidth());
        glced->setSizePolicy(sizePolicy);
        EventDisplayWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(EventDisplayWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1238, 29));
        EventDisplayWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(EventDisplayWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        EventDisplayWindow->setStatusBar(statusbar);

        retranslateUi(EventDisplayWindow);

        QMetaObject::connectSlotsByName(EventDisplayWindow);
    } // setupUi

    void retranslateUi(QMainWindow *EventDisplayWindow)
    {
        EventDisplayWindow->setWindowTitle(QApplication::translate("EventDisplayWindow", "AHCAL Event Display", 0, QApplication::UnicodeUTF8));
        Quit->setText(QApplication::translate("EventDisplayWindow", "Quit", 0, QApplication::UnicodeUTF8));
        Start->setText(QApplication::translate("EventDisplayWindow", "Start", 0, QApplication::UnicodeUTF8));
        Next_Ev->setText(QApplication::translate("EventDisplayWindow", "Next Event", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventDisplayWindow: public Ui_EventDisplayWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EVENTDISPLAYWINDOW_H
