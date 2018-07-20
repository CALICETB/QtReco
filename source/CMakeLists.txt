
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

INCLUDE_DIRECTORIES( BEFORE include include/tinyxml ${QT_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} )

SET(CMAKE_AUTOMOC OFF)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

QT4_WRAP_CPP( MOC_SRC_FILES ${INC_FILES} )
QT4_WRAP_UI( UI_FILES ${GLOB_UI} )
QT4_ADD_RESOURCES(QRC_FILES ${GLOB_QRC} )

ADD_SHARED_LIBRARY( ${PROJECT_NAME} ${GLOB_SRC} ${MOC_SRC_FILES} ${UI_FILES} ${QRC_FILES} )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${QT_LIBRARIES} ${ROOT_LIBRARIES} ${GLUT_LIBRARIES} ${Boost_LIBRARIES} )

INSTALL(
        TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION lib
)

ADD_EXECUTABLE( QtReco_interface main/QtReco.cpp )
TARGET_LINK_LIBRARIES( QtReco_interface ${PROJECT_NAME} ${PROJECT_NAME} )

INSTALL(
    TARGETS QtReco_interface
    RUNTIME DESTINATION bin
)

# ----- Install headers -----
INSTALL_DIRECTORY( include DESTINATION . FILES_MATCHING PATTERN "*.h" )
