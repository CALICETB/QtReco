
file(GLOB GLOB_SRC
       src/*.cpp
       src/tinyxml/*.cpp)

SET ( INC_FILES
       include/AnalysisModule.h
       include/AnalysisThread.h
       include/ApplicationModuleApi.h
       include/DMAHCALBooker.h
       include/DMAHCALMainWindow.h
       include/DMAHCALStorage.h
       include/EventDisplayWindow.h
       include/glced.h
       include/Logger.h
       include/MarlinModule.h
       include/RequestHandler.h
       include/SedModule.h
       include/ServerModule.h
       include/XmlParser.h )
       
file(GLOB GLOB_UI
        ${PROJECT_SOURCE_DIR}/ui/*.ui )
        
 file(GLOB GLOB_QRC
        ${PROJECT_SOURCE_DIR}/ressource/*.qrc )

# require proper c++
ADD_DEFINITIONS( "-pedantic -Wunused-value -O2" )
ADD_DEFINITIONS("-Wno-long-long -Wreturn-type -Werror")

IF ( WITH_QT5 )

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

SET( QT_INCLUDE_DIRS ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} )

INCLUDE_DIRECTORIES( BEFORE include include/tinyxml ${QT_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )
SET(QT_LIBRARIES ${Qt5OpenGL_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Network_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES} )
  
QT5_WRAP_CPP(MOC_SRC_FILES ${INC_FILES} )
QT5_WRAP_UI(UI_FILES ${GLOB_UI} )
QT5_ADD_RESOURCES(QRC_FILES ${GLOB_QRC} )

ADD_LIBRARY( ${PROJECT_NAME} SHARED ${GLOB_SRC} ${MOC_SRC_FILES} ${UI_FILES} ${QRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${QT_LIBRARIES} ${ROOT_LIBRARIES} ${GLUT_LIBRARIES} ${Boost_LIBRARIES} )

ADD_EXECUTABLE( QtReco main/QtReco.cpp )
TARGET_LINK_LIBRARIES( QtReco ${PROJECT_NAME} ${PROJECT_NAME} )

QT5_USE_MODULES(QtReco Core Gui Widgets Network)

ELSE( WITH_QT5 )

INCLUDE_DIRECTORIES( BEFORE include include/tinyxml ${QT_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

QT4_WRAP_CPP( MOC_SRC_FILES ${INC_FILES} )
QT4_WRAP_UI( UI_FILES ${GLOB_UI} )
QT4_ADD_RESOURCES(QRC_FILES ${GLOB_QRC} )
			
ADD_LIBRARY( ${PROJECT_NAME} SHARED ${GLOB_SRC} ${MOC_SRC_FILES} ${UI_FILES} ${QRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${QT_LIBRARIES} ${ROOT_LIBRARIES} ${GLUT_LIBRARIES} ${Boost_LIBRARIES} )

ADD_EXECUTABLE( QtReco main/QtReco.cpp )
TARGET_LINK_LIBRARIES( QtReco ${PROJECT_NAME} ${PROJECT_NAME} )

ENDIF ( WITH_QT5 )

# ----- Install headers -----
INSTALL_DIRECTORY( include DESTINATION . FILES_MATCHING PATTERN "*.h" )
INSTALL_DIRECTORY( include/tinyxml DESTINATION . FILES_MATCHING PATTERN "*.h" )
