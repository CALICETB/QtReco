/********************************************************************************
** Form generated from reading UI file 'DMAHCALMainWindow.ui'
**
** Created: Sat May 19 14:40:30 2018
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DMAHCALMAINWINDOW_H
#define UI_DMAHCALMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFormLayout>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DMAHCALMainWindow
{
public:
    QWidget *MainGUI;
    QSplitter *splitter_3;
    QSplitter *splitter;
    QPushButton *Event_Display;
    QWidget *layoutWidget;
    QHBoxLayout *ServerLayout;
    QLabel *labelserver;
    QLabel *NClients;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_3;
    QFormLayout *formLayout_4;
    QLabel *NLayer_value;
    QLabel *With_EBU_value;
    QLabel *MIP_Cut_Value;
    QLabel *NMinHits_value;
    QLabel *NMaxHits_value;
    QLabel *nT0s_value;
    QFormLayout *formLayout_3;
    QLabel *NLayer_label;
    QLabel *With_EBU_label;
    QLabel *MIP_Cut_label;
    QLabel *NMaxHits_label;
    QLabel *NMinHits_label;
    QLabel *nT0s_label;
    QLabel *Analysis_Paramete_label;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *FindRun;
    QPushButton *Steering;
    QLineEdit *lineEdit;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QLabel *MapIJK_value;
    QLabel *label_run;
    QLineEdit *PathIn;
    QLabel *MapIJK_label;
    QLabel *label_input;
    QLabel *label_output;
    QLineEdit *Runnumber;
    QLabel *desy_logo;
    QLabel *label;
    QLineEdit *PathOut;
    QFrame *frame;
    QWidget *tab2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLabel *label_BIF_OFFSET;
    QLabel *label_input_path;
    QLabel *label_out_root;
    QLabel *label_out_slcio;
    QLineEdit *MIP_cut;
    QLabel *label_MIP_cut;
    QLineEdit *MIP_cut_2;
    QLabel *label_MIP_cut_2;
    QLabel *calice_logo;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *Mode_label;
    QLabel *Mode_value;
    QLabel *Convertion_MIP_label;
    QLabel *Reco_Parameter_label;
    QLabel *PedSub_label;
    QLabel *MIP_Conversion_value;
    QLabel *PedSub_value;
    QPushButton *Quit;
    QPushButton *Stop;
    QPushButton *Start;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuMenu;

    void setupUi(QMainWindow *DMAHCALMainWindow)
    {
        if (DMAHCALMainWindow->objectName().isEmpty())
            DMAHCALMainWindow->setObjectName(QString::fromUtf8("DMAHCALMainWindow"));
        DMAHCALMainWindow->setWindowModality(Qt::NonModal);
        DMAHCALMainWindow->setEnabled(true);
        DMAHCALMainWindow->resize(1050, 527);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DMAHCALMainWindow->sizePolicy().hasHeightForWidth());
        DMAHCALMainWindow->setSizePolicy(sizePolicy);
        DMAHCALMainWindow->setMinimumSize(QSize(1050, 470));
        DMAHCALMainWindow->setMaximumSize(QSize(1050, 570));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 255, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Light, brush1);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        QBrush brush2(QColor(127, 127, 127, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush2);
        QBrush brush3(QColor(170, 170, 170, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush1);
        QBrush brush4(QColor(255, 255, 220, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        DMAHCALMainWindow->setPalette(palette);
        DMAHCALMainWindow->setWindowOpacity(1);
        DMAHCALMainWindow->setAutoFillBackground(false);
        MainGUI = new QWidget(DMAHCALMainWindow);
        MainGUI->setObjectName(QString::fromUtf8("MainGUI"));
        MainGUI->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(MainGUI->sizePolicy().hasHeightForWidth());
        MainGUI->setSizePolicy(sizePolicy1);
        MainGUI->setSizeIncrement(QSize(10, 10));
        MainGUI->setLayoutDirection(Qt::LeftToRight);
        MainGUI->setAutoFillBackground(true);
        splitter_3 = new QSplitter(MainGUI);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setGeometry(QRect(819, 352, 0, 31));
        splitter_3->setOrientation(Qt::Horizontal);
        splitter_3->setHandleWidth(20);
        splitter = new QSplitter(splitter_3);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        splitter_3->addWidget(splitter);
        Event_Display = new QPushButton(splitter_3);
        Event_Display->setObjectName(QString::fromUtf8("Event_Display"));
        splitter_3->addWidget(Event_Display);
        layoutWidget = new QWidget(MainGUI);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(819, 122, 193, 24));
        ServerLayout = new QHBoxLayout(layoutWidget);
        ServerLayout->setSpacing(6);
        ServerLayout->setContentsMargins(11, 11, 11, 11);
        ServerLayout->setObjectName(QString::fromUtf8("ServerLayout"));
        ServerLayout->setContentsMargins(0, 0, 0, 0);
        labelserver = new QLabel(layoutWidget);
        labelserver->setObjectName(QString::fromUtf8("labelserver"));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        labelserver->setFont(font);

        ServerLayout->addWidget(labelserver);

        NClients = new QLabel(layoutWidget);
        NClients->setObjectName(QString::fromUtf8("NClients"));

        ServerLayout->addWidget(NClients);

        layoutWidget1 = new QWidget(MainGUI);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(819, 232, 181, 161));
        gridLayout_3 = new QGridLayout(layoutWidget1);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        formLayout_4 = new QFormLayout();
        formLayout_4->setSpacing(6);
        formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        NLayer_value = new QLabel(layoutWidget1);
        NLayer_value->setObjectName(QString::fromUtf8("NLayer_value"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, NLayer_value);

        With_EBU_value = new QLabel(layoutWidget1);
        With_EBU_value->setObjectName(QString::fromUtf8("With_EBU_value"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, With_EBU_value);

        MIP_Cut_Value = new QLabel(layoutWidget1);
        MIP_Cut_Value->setObjectName(QString::fromUtf8("MIP_Cut_Value"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, MIP_Cut_Value);

        NMinHits_value = new QLabel(layoutWidget1);
        NMinHits_value->setObjectName(QString::fromUtf8("NMinHits_value"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, NMinHits_value);

        NMaxHits_value = new QLabel(layoutWidget1);
        NMaxHits_value->setObjectName(QString::fromUtf8("NMaxHits_value"));

        formLayout_4->setWidget(4, QFormLayout::LabelRole, NMaxHits_value);

        nT0s_value = new QLabel(layoutWidget1);
        nT0s_value->setObjectName(QString::fromUtf8("nT0s_value"));

        formLayout_4->setWidget(5, QFormLayout::LabelRole, nT0s_value);


        gridLayout_3->addLayout(formLayout_4, 1, 1, 1, 1);

        formLayout_3 = new QFormLayout();
        formLayout_3->setSpacing(6);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        NLayer_label = new QLabel(layoutWidget1);
        NLayer_label->setObjectName(QString::fromUtf8("NLayer_label"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, NLayer_label);

        With_EBU_label = new QLabel(layoutWidget1);
        With_EBU_label->setObjectName(QString::fromUtf8("With_EBU_label"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, With_EBU_label);

        MIP_Cut_label = new QLabel(layoutWidget1);
        MIP_Cut_label->setObjectName(QString::fromUtf8("MIP_Cut_label"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, MIP_Cut_label);

        NMaxHits_label = new QLabel(layoutWidget1);
        NMaxHits_label->setObjectName(QString::fromUtf8("NMaxHits_label"));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, NMaxHits_label);

        NMinHits_label = new QLabel(layoutWidget1);
        NMinHits_label->setObjectName(QString::fromUtf8("NMinHits_label"));

        formLayout_3->setWidget(3, QFormLayout::SpanningRole, NMinHits_label);

        nT0s_label = new QLabel(layoutWidget1);
        nT0s_label->setObjectName(QString::fromUtf8("nT0s_label"));

        formLayout_3->setWidget(5, QFormLayout::LabelRole, nT0s_label);


        gridLayout_3->addLayout(formLayout_3, 1, 0, 1, 1);

        Analysis_Paramete_label = new QLabel(layoutWidget1);
        Analysis_Paramete_label->setObjectName(QString::fromUtf8("Analysis_Paramete_label"));
        Analysis_Paramete_label->setFont(font);
        Analysis_Paramete_label->setWordWrap(false);

        gridLayout_3->addWidget(Analysis_Paramete_label, 0, 0, 1, 2);

        tabWidget = new QTabWidget(MainGUI);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(19, 12, 771, 371));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        FindRun = new QPushButton(tab);
        FindRun->setObjectName(QString::fromUtf8("FindRun"));
        FindRun->setGeometry(QRect(169, 22, 141, 41));
        Steering = new QPushButton(tab);
        Steering->setObjectName(QString::fromUtf8("Steering"));
        Steering->setGeometry(QRect(169, 72, 141, 41));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(29, 82, 113, 27));
        radioButton = new QRadioButton(tab);
        radioButton->setObjectName(QString::fromUtf8("radioButton"));
        radioButton->setGeometry(QRect(29, 32, 116, 22));
        radioButton_2 = new QRadioButton(tab);
        radioButton_2->setObjectName(QString::fromUtf8("radioButton_2"));
        radioButton_2->setGeometry(QRect(29, 52, 151, 22));
        MapIJK_value = new QLabel(tab);
        MapIJK_value->setObjectName(QString::fromUtf8("MapIJK_value"));
        MapIJK_value->setGeometry(QRect(110, 303, 641, 17));
        label_run = new QLabel(tab);
        label_run->setObjectName(QString::fromUtf8("label_run"));
        label_run->setGeometry(QRect(59, 222, 47, 17));
        PathIn = new QLineEdit(tab);
        PathIn->setObjectName(QString::fromUtf8("PathIn"));
        PathIn->setGeometry(QRect(109, 246, 641, 27));
        PathIn->setAutoFillBackground(true);
        PathIn->setReadOnly(true);
        MapIJK_label = new QLabel(tab);
        MapIJK_label->setObjectName(QString::fromUtf8("MapIJK_label"));
        MapIJK_label->setGeometry(QRect(49, 302, 57, 17));
        label_input = new QLabel(tab);
        label_input->setObjectName(QString::fromUtf8("label_input"));
        label_input->setGeometry(QRect(39, 252, 64, 17));
        label_output = new QLabel(tab);
        label_output->setObjectName(QString::fromUtf8("label_output"));
        label_output->setGeometry(QRect(19, 278, 89, 17));
        Runnumber = new QLineEdit(tab);
        Runnumber->setObjectName(QString::fromUtf8("Runnumber"));
        Runnumber->setGeometry(QRect(109, 218, 129, 27));
        Runnumber->setAutoFillBackground(true);
        Runnumber->setReadOnly(false);
        desy_logo = new QLabel(tab);
        desy_logo->setObjectName(QString::fromUtf8("desy_logo"));
        desy_logo->setGeometry(QRect(669, 22, 91, 81));
        desy_logo->setPixmap(QPixmap(QString::fromUtf8(":/ressource/images/desy.gif")));
        desy_logo->setScaledContents(true);
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(329, 200, 201, 21));
        QFont font1;
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        label->setFont(font1);
        PathOut = new QLineEdit(tab);
        PathOut->setObjectName(QString::fromUtf8("PathOut"));
        PathOut->setGeometry(QRect(110, 273, 641, 27));
        PathOut->setAutoFillBackground(true);
        PathOut->setReadOnly(true);
        frame = new QFrame(tab);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(10, 190, 751, 141));
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        frame->setLineWidth(1);
        frame->setMidLineWidth(0);
        tabWidget->addTab(tab, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        lineEdit_2 = new QLineEdit(tab2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(120, 10, 113, 27));
        lineEdit_3 = new QLineEdit(tab2);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(120, 70, 631, 27));
        lineEdit_4 = new QLineEdit(tab2);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(120, 100, 631, 27));
        lineEdit_5 = new QLineEdit(tab2);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(120, 40, 631, 27));
        label_BIF_OFFSET = new QLabel(tab2);
        label_BIF_OFFSET->setObjectName(QString::fromUtf8("label_BIF_OFFSET"));
        label_BIF_OFFSET->setGeometry(QRect(10, 10, 81, 20));
        label_input_path = new QLabel(tab2);
        label_input_path->setObjectName(QString::fromUtf8("label_input_path"));
        label_input_path->setGeometry(QRect(10, 40, 81, 20));
        label_out_root = new QLabel(tab2);
        label_out_root->setObjectName(QString::fromUtf8("label_out_root"));
        label_out_root->setGeometry(QRect(10, 70, 101, 20));
        label_out_slcio = new QLabel(tab2);
        label_out_slcio->setObjectName(QString::fromUtf8("label_out_slcio"));
        label_out_slcio->setGeometry(QRect(10, 100, 91, 20));
        MIP_cut = new QLineEdit(tab2);
        MIP_cut->setObjectName(QString::fromUtf8("MIP_cut"));
        MIP_cut->setGeometry(QRect(120, 130, 71, 27));
        label_MIP_cut = new QLabel(tab2);
        label_MIP_cut->setObjectName(QString::fromUtf8("label_MIP_cut"));
        label_MIP_cut->setGeometry(QRect(10, 130, 91, 20));
        MIP_cut_2 = new QLineEdit(tab2);
        MIP_cut_2->setObjectName(QString::fromUtf8("MIP_cut_2"));
        MIP_cut_2->setGeometry(QRect(290, 130, 101, 27));
        label_MIP_cut_2 = new QLabel(tab2);
        label_MIP_cut_2->setObjectName(QString::fromUtf8("label_MIP_cut_2"));
        label_MIP_cut_2->setGeometry(QRect(210, 130, 91, 20));
        tabWidget->addTab(tab2, QString());
        lineEdit_2->raise();
        lineEdit_3->raise();
        lineEdit_4->raise();
        lineEdit_5->raise();
        label_BIF_OFFSET->raise();
        label_input_path->raise();
        label_out_root->raise();
        label_out_slcio->raise();
        MIP_cut->raise();
        label_MIP_cut->raise();
        MIP_cut_2->raise();
        label_MIP_cut_2->raise();
        desy_logo->raise();
        calice_logo = new QLabel(MainGUI);
        calice_logo->setObjectName(QString::fromUtf8("calice_logo"));
        calice_logo->setGeometry(QRect(819, 12, 181, 71));
        sizePolicy.setHeightForWidth(calice_logo->sizePolicy().hasHeightForWidth());
        calice_logo->setSizePolicy(sizePolicy);
        calice_logo->setMaximumSize(QSize(203, 96));
        calice_logo->setPixmap(QPixmap(QString::fromUtf8(":/ressource/images/calice_logo.jpg")));
        calice_logo->setScaledContents(true);
        calice_logo->setWordWrap(false);
        widget = new QWidget(MainGUI);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(821, 94, 170, 24));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Mode_label = new QLabel(widget);
        Mode_label->setObjectName(QString::fromUtf8("Mode_label"));
        Mode_label->setFont(font);

        gridLayout->addWidget(Mode_label, 0, 0, 1, 1);

        Mode_value = new QLabel(widget);
        Mode_value->setObjectName(QString::fromUtf8("Mode_value"));

        gridLayout->addWidget(Mode_value, 0, 1, 1, 1);

        Convertion_MIP_label = new QLabel(MainGUI);
        Convertion_MIP_label->setObjectName(QString::fromUtf8("Convertion_MIP_label"));
        Convertion_MIP_label->setGeometry(QRect(821, 182, 114, 17));
        Reco_Parameter_label = new QLabel(MainGUI);
        Reco_Parameter_label->setObjectName(QString::fromUtf8("Reco_Parameter_label"));
        Reco_Parameter_label->setGeometry(QRect(821, 162, 128, 19));
        Reco_Parameter_label->setFont(font);
        PedSub_label = new QLabel(MainGUI);
        PedSub_label->setObjectName(QString::fromUtf8("PedSub_label"));
        PedSub_label->setGeometry(QRect(821, 202, 156, 17));
        MIP_Conversion_value = new QLabel(MainGUI);
        MIP_Conversion_value->setObjectName(QString::fromUtf8("MIP_Conversion_value"));
        MIP_Conversion_value->setGeometry(QRect(984, 180, 16, 17));
        PedSub_value = new QLabel(MainGUI);
        PedSub_value->setObjectName(QString::fromUtf8("PedSub_value"));
        PedSub_value->setGeometry(QRect(984, 202, 16, 17));
        Quit = new QPushButton(MainGUI);
        Quit->setObjectName(QString::fromUtf8("Quit"));
        Quit->setGeometry(QRect(243, 400, 85, 41));
        Stop = new QPushButton(MainGUI);
        Stop->setObjectName(QString::fromUtf8("Stop"));
        Stop->setGeometry(QRect(132, 400, 85, 41));
        Start = new QPushButton(MainGUI);
        Start->setObjectName(QString::fromUtf8("Start"));
        Start->setGeometry(QRect(20, 400, 85, 41));
        DMAHCALMainWindow->setCentralWidget(MainGUI);
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        splitter_3->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        layoutWidget->raise();
        tabWidget->raise();
        labelserver->raise();
        calice_logo->raise();
        mainToolBar = new QToolBar(DMAHCALMainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        DMAHCALMainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DMAHCALMainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        DMAHCALMainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(DMAHCALMainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1050, 25));
        menuMenu = new QMenu(menuBar);
        menuMenu->setObjectName(QString::fromUtf8("menuMenu"));
        DMAHCALMainWindow->setMenuBar(menuBar);
        QWidget::setTabOrder(FindRun, Steering);
        QWidget::setTabOrder(Steering, Start);
        QWidget::setTabOrder(Start, Quit);
        QWidget::setTabOrder(Quit, radioButton);
        QWidget::setTabOrder(radioButton, radioButton_2);
        QWidget::setTabOrder(radioButton_2, Event_Display);
        QWidget::setTabOrder(Event_Display, lineEdit);
        QWidget::setTabOrder(lineEdit, Stop);
        QWidget::setTabOrder(Stop, tabWidget);
        QWidget::setTabOrder(tabWidget, lineEdit_2);
        QWidget::setTabOrder(lineEdit_2, lineEdit_3);
        QWidget::setTabOrder(lineEdit_3, lineEdit_4);
        QWidget::setTabOrder(lineEdit_4, lineEdit_5);
        QWidget::setTabOrder(lineEdit_5, Runnumber);
        QWidget::setTabOrder(Runnumber, PathIn);
        QWidget::setTabOrder(PathIn, PathOut);
        QWidget::setTabOrder(PathOut, MIP_cut);

        menuBar->addAction(menuMenu->menuAction());

        retranslateUi(DMAHCALMainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(DMAHCALMainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DMAHCALMainWindow)
    {
        DMAHCALMainWindow->setWindowTitle(QApplication::translate("DMAHCALMainWindow", "DMACHAL v1.5", 0, QApplication::UnicodeUTF8));
        Event_Display->setText(QApplication::translate("DMAHCALMainWindow", "eventD", 0, QApplication::UnicodeUTF8));
        labelserver->setText(QApplication::translate("DMAHCALMainWindow", "Server connections :", 0, QApplication::UnicodeUTF8));
        NClients->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        NLayer_value->setText(QApplication::translate("DMAHCALMainWindow", "15", 0, QApplication::UnicodeUTF8));
        With_EBU_value->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        MIP_Cut_Value->setText(QApplication::translate("DMAHCALMainWindow", "0.5 MIP", 0, QApplication::UnicodeUTF8));
        NMinHits_value->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        NMaxHits_value->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        nT0s_value->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        NLayer_label->setText(QApplication::translate("DMAHCALMainWindow", "N Layers :", 0, QApplication::UnicodeUTF8));
        With_EBU_label->setText(QApplication::translate("DMAHCALMainWindow", "With EBU :", 0, QApplication::UnicodeUTF8));
        MIP_Cut_label->setText(QApplication::translate("DMAHCALMainWindow", "MIP Cut : ", 0, QApplication::UnicodeUTF8));
        NMaxHits_label->setText(QApplication::translate("DMAHCALMainWindow", "nMaxHits :", 0, QApplication::UnicodeUTF8));
        NMinHits_label->setText(QApplication::translate("DMAHCALMainWindow", "nMinHits :", 0, QApplication::UnicodeUTF8));
        nT0s_label->setText(QApplication::translate("DMAHCALMainWindow", "nT0s :", 0, QApplication::UnicodeUTF8));
        Analysis_Paramete_label->setText(QApplication::translate("DMAHCALMainWindow", "Analysis Parameters", 0, QApplication::UnicodeUTF8));
        FindRun->setText(QApplication::translate("DMAHCALMainWindow", "Find Run", 0, QApplication::UnicodeUTF8));
        Steering->setText(QApplication::translate("DMAHCALMainWindow", "Configure", 0, QApplication::UnicodeUTF8));
        lineEdit->setText(QString());
        lineEdit->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Run number", 0, QApplication::UnicodeUTF8));
        radioButton->setText(QApplication::translate("DMAHCALMainWindow", "Last run", 0, QApplication::UnicodeUTF8));
        radioButton_2->setText(QApplication::translate("DMAHCALMainWindow", "Enter manually", 0, QApplication::UnicodeUTF8));
        MapIJK_value->setText(QString());
        label_run->setText(QApplication::translate("DMAHCALMainWindow", "Run # :", 0, QApplication::UnicodeUTF8));
        PathIn->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Output file indicator", 0, QApplication::UnicodeUTF8));
        MapIJK_label->setText(QApplication::translate("DMAHCALMainWindow", "MapIJK :", 0, QApplication::UnicodeUTF8));
        label_input->setText(QApplication::translate("DMAHCALMainWindow", "Input File", 0, QApplication::UnicodeUTF8));
        label_output->setText(QApplication::translate("DMAHCALMainWindow", "Output PATH", 0, QApplication::UnicodeUTF8));
        Runnumber->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Run number indicator", 0, QApplication::UnicodeUTF8));
        desy_logo->setText(QString());
        label->setText(QApplication::translate("DMAHCALMainWindow", "Indicators", 0, QApplication::UnicodeUTF8));
        PathOut->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Output path indicator", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("DMAHCALMainWindow", "Configure", 0, QApplication::UnicodeUTF8));
        lineEdit_2->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "BIF Offset", 0, QApplication::UnicodeUTF8));
        lineEdit_3->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Output ROOT file path", 0, QApplication::UnicodeUTF8));
        lineEdit_4->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Output SLCIO file path", 0, QApplication::UnicodeUTF8));
        lineEdit_5->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Inut file path", 0, QApplication::UnicodeUTF8));
        label_BIF_OFFSET->setText(QApplication::translate("DMAHCALMainWindow", "BIF Offset", 0, QApplication::UnicodeUTF8));
        label_input_path->setText(QApplication::translate("DMAHCALMainWindow", "INPUT Path", 0, QApplication::UnicodeUTF8));
        label_out_root->setText(QApplication::translate("DMAHCALMainWindow", "Ouput ROOT", 0, QApplication::UnicodeUTF8));
        label_out_slcio->setText(QApplication::translate("DMAHCALMainWindow", "Output SLCIO", 0, QApplication::UnicodeUTF8));
        MIP_cut->setText(QString());
        MIP_cut->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "MIP Cut", 0, QApplication::UnicodeUTF8));
        label_MIP_cut->setText(QApplication::translate("DMAHCALMainWindow", "Mip cut", 0, QApplication::UnicodeUTF8));
        MIP_cut_2->setText(QString());
        MIP_cut_2->setPlaceholderText(QApplication::translate("DMAHCALMainWindow", "Max events", 0, QApplication::UnicodeUTF8));
        label_MIP_cut_2->setText(QApplication::translate("DMAHCALMainWindow", "Max events", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("DMAHCALMainWindow", "Other Settings", 0, QApplication::UnicodeUTF8));
        calice_logo->setText(QString());
        Mode_label->setText(QApplication::translate("DMAHCALMainWindow", "Mode Analysis : ", 0, QApplication::UnicodeUTF8));
        Mode_value->setText(QApplication::translate("DMAHCALMainWindow", "ON", 0, QApplication::UnicodeUTF8));
        Convertion_MIP_label->setText(QApplication::translate("DMAHCALMainWindow", "MIP Conversion : ", 0, QApplication::UnicodeUTF8));
        Reco_Parameter_label->setText(QApplication::translate("DMAHCALMainWindow", "Reco Parameters", 0, QApplication::UnicodeUTF8));
        PedSub_label->setText(QApplication::translate("DMAHCALMainWindow", "Pedestal Substraction : ", 0, QApplication::UnicodeUTF8));
        MIP_Conversion_value->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        PedSub_value->setText(QApplication::translate("DMAHCALMainWindow", "0", 0, QApplication::UnicodeUTF8));
        Quit->setText(QApplication::translate("DMAHCALMainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        Stop->setText(QApplication::translate("DMAHCALMainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        Start->setText(QApplication::translate("DMAHCALMainWindow", "Start", 0, QApplication::UnicodeUTF8));
        menuMenu->setTitle(QApplication::translate("DMAHCALMainWindow", "Menu", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DMAHCALMainWindow: public Ui_DMAHCALMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DMAHCALMAINWINDOW_H
