#################################################
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
#################################################

# ----- project name -----
PROJECT( QtReco )

SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 0 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

#################################################################
# Require C++11                                                 #
#################################################################

INCLUDE( CheckCXXCompilerFlag )

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)

IF( COMPILER_SUPPORTS_CXX11 )
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ELSE()
MESSAGE( SEND_ERROR "QtReco Client requires C++11 support. Please upgrade your compiler !" )
ENDIF()

###############################
# Dependencies
LIST( APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake )
INCLUDE( default_settings )

######### Options #############

OPTION( BUILD_DOCUMENTATION  "Set to OFF to skip build/install Documentation" ON )

################################################################
# ROOT                                                         #
################################################################

# ----- Find ROOT package -----
LIST( APPEND ROOT_COMPONENT_LIST Core Hist Rint HistPainter Graf Graf3d MathCore Net RIO Tree Gui )
FIND_PACKAGE( ROOT COMPONENTS ${ROOT_COMPONENT_LIST} REQUIRED )

################################################################
# GLUT                                                         #
################################################################

FIND_PACKAGE(GLUT REQUIRED)

################################################################
# Boost                                                        #
################################################################

FIND_PACKAGE ( Boost 1.36.0 REQUIRED )

# ----- Include package properties -----
FOREACH( pkg ROOT GLUT Boost)

  INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
  LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
  ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )

  LIST( APPEND ${PROJECT_NAME}_DEPENDS_INCLUDE_DIRS ${${pkg}_INCLUDE_DIRS} )
  LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARY_DIRS ${${pkg}_LIBRARY_DIRS} )
  LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARIES ${${pkg}_LIBRARIES} )

ENDFOREACH()

################################################################
# Qt                                                           #
################################################################

SET( QT_ROOT_DIR $ENV{QTDIR} )
SET( QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake )

FIND_PACKAGE( Qt4 COMPONENTS QtCore QtGui QtNetwork QtOpenGL REQUIRED )

INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIRS} )
ADD_DEFINITIONS ( ${QT_DEFINITIONS} )

INCLUDE( ${QT_USE_FILE} )

LIST( APPEND ${PROJECT_NAME}_DEPENDS_INCLUDE_DIRS ${QT_INCLUDE_DIRS} )
LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARY_DIRS ${QT_LIBRARY_DIRS} )
LIST( APPEND ${PROJECT_NAME}_DEPENDS_LIBRARIES    ${QT_LIBRARIES}    )

# ----- Install doc -----
IF( BUILD_DOCUMENTATION )

    FIND_PACKAGE( Doxygen )

    IF( DOXYGEN_FOUND )
        ADD_SUBDIRECTORY( ./doc )
    ELSE()
        MESSAGE( SEND_ERROR "Could not find doxygen required to build documentation" )
        MESSAGE( "Please install doxygen or set BUILD_DOCUMENTATION to OFF" )
        MESSAGE( "" )
    ENDIF()

ENDIF()

# ---- Generate QtRecoConfig.h
CONFIGURE_FILE( "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.h.cmake.in"
    "${PROJECT_SOURCE_DIR}/source/include/${PROJECT_NAME}Config.h" @ONLY )

# ----- Add source directory -----
ADD_SUBDIRECTORY( ./source )

################################################################

INSTALL ( DIRECTORY cmake DESTINATION . PATTERN "*.in" EXCLUDE )
INSTALL ( DIRECTORY scripts DESTINATION . FILES_MATCHING PATTERN "*.sh" )
INSTALL ( DIRECTORY xml DESTINATION . FILES_MATCHING PATTERN "*.xml" )
INSTALL ( DIRECTORY mapping DESTINATION . FILES_MATCHING PATTERN "*.txt" )

# ----- display some variables and write them to cache -----
DISPLAY_STD_VARIABLES()

# ----- generate and install following configuration files -----
GENERATE_PACKAGE_CONFIGURATION_FILES( ${PROJECT_NAME}Config.cmake ${PROJECT_NAME}ConfigVersion.cmake ${PROJECT_NAME}LibDeps.cmake )
