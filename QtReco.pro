#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T20:14:27
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtReco
TEMPLATE = app

SOURCES += src/main.cpp\
    src/glced.cpp \
    src/ApplicationModuleApi.cpp \
    src/MarlinModule.cpp \
    src/tinyxml/tinystr.cpp \
    src/tinyxml/tinyxml.cpp \
    src/tinyxml/tinyxmlerror.cpp \
    src/tinyxml/tinyxmlparser.cpp \
    src/DMAHCALMainWindow.cpp \
#    src/EventDisplayWindow.cpp \
    src/SedModule.cpp \
    src/ServerModule.cpp \
    src/AnalysisModule.cpp \
    src/Logger.cpp \
    src/XmlParser.cpp \
    src/AnalysisThread.cpp \
    src/DMAHCALStorage.cpp \
    src/DMAHCALBooker.cpp \
    src/RequestHandler.cpp

HEADERS  += \
    include/glced.h \
    include/ApplicationModuleApi.h \
    include/MarlinModule.h \
    include/tinyxml/tinystr.h \
    include/tinyxml/tinyxml.h \
    include/DMAHCALMainWindow.h \
#    include/EventDisplayWindow.h \
    include/SedModule.h \
    include/ServerModule.h \
    include/AnalysisModule.h \
    include/Logger.h \
    include/XmlParser.h \
    include/AnalysisThread.h \
    include/DMAHCALStorage.h \
    include/DMAHCALBooker.h \
    include/DMAHCAL.h \
    include/RequestHandler.h

FORMS    += \
    ui/DMAHCALMainWindow.ui \
    ui/EventDisplayWindow.ui

DISTFILES += \
    images/calice_logo.jpg \
    images/desy.gif \
    xml/steering.xml \
    scripts/myMarlin.sh \
    xml/Reco.xml \
    scripts/Reco_sed.sh

INCLUDEPATH += $(ROOTSYS)/include \
include include/tinyxml \
#$(ILCSOFT)/CED/v01-09-01/include $(ILCSOFT)/CED/v01-09-01/src \
$(ILCSOFT)/Marlin/v01-05/include \
/usr/include \ 

LIBS += -lGL -lglut -lGLEW \
#-L$(ILCSOFT)/CED/v01-09-01/lib -lCED \
-L$(ROOTSYS)/lib -Wl,--no-as-needed -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree \
-lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lm -ldl -rdynamic \

CONFIG += x86_64

RESOURCES += \
    ui/ressources.qrc
