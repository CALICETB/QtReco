#################################################
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 FATAL_ERROR )
#################################################

#As of CMake 2.6.0 the ELSE() and ENDIF() constructs can be empty
SET( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS TRUE )
cmake_policy(SET CMP0020 NEW)

# ----- project name -----
PROJECT( QTRECO )

SET( ${PROJECT_NAME}_MAJOR_VERSION 1 )
SET( ${PROJECT_NAME}_MINOR_VERSION 0 )
SET( ${PROJECT_NAME}_PATCH_LEVEL 0 )

# library *nix style versioning
SET( ${PROJECT_NAME}_SOVERSION "${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}" )
SET( ${PROJECT_NAME}_VERSION   "${${PROJECT_NAME}_SOVERSION}.${${PROJECT_NAME}_VERSION_PATCH}" )
    
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

IF(NOT CMAKE_BUILD_TYPE)
SET(CMAKE_BUILD_TYPE RelWithDebInfo)
ENDIF(NOT CMAKE_BUILD_TYPE)

SET( CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE )

#################################################################
# cmake modules                                                 #
#################################################################

SET ( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

INCLUDE ( MacroInstallDirectory )    
INCLUDE( FULL_INSTALL_PATH )

#################################################################
# installation options                                          #
#################################################################

# change default install prefix
IF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )
    SET( CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "Install prefix" FORCE )
ENDIF( CMAKE_INSTALL_PREFIX STREQUAL "/usr/local" )

# default destination for header files: ${CMAKE_INSTALL_PREFIX}/install
SET( INCLUDE_INSTALL_DIR "include" CACHE PATH "Directory to install the header files" )
MARK_AS_ADVANCED( INCLUDE_INSTALL_DIR )

# default destination for executables: ${CMAKE_INSTALL_PREFIX}/install
SET( BIN_INSTALL_DIR "bin" CACHE PATH "Directory to install the executables" )
MARK_AS_ADVANCED( INCLUDE_INSTALL_DIR )

# default destination for library files: ${CMAKE_INSTALL_PREFIX}/install
SET( LIB_INSTALL_DIR "lib" CACHE PATH "Directory to install the libraries" )
MARK_AS_ADVANCED( LIB_INSTALL_DIR )

# default destination for .cmake files: ${CMAKE_INSTALL_PREFIX}/install
SET( CONFIG_INSTALL_DIR "cmake" CACHE PATH "Directory to install the XXXConfig.cmake files" )
MARK_AS_ADVANCED( CONFIG_INSTALL_DIR )

# provide nicer directory layout in build directory
SET( EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin" CACHE PATH
    "EXECUTABLE_OUTPUT_PATH" FORCE )
SET( LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib" CACHE PATH
    "LIBRARY_OUTPUT_PATH" FORCE )
MARK_AS_ADVANCED( EXECUTABLE_OUTPUT_PATH LIBRARY_OUTPUT_PATH )

##################################################################
# install configuration                                          #
##################################################################
#compute install path for bin, lib and include
FULL_INSTALL_PATH( LIB     FULL_LIB_INSTALL_DIR )
FULL_INSTALL_PATH( INCLUDE FULL_INCLUDE_INSTALL_DIR )

CONFIGURE_FILE( ${QTRECO_SOURCE_DIR}/cmake/QTRECOConfig.cmake.in
                ${QTRECO_BINARY_DIR}/QTRECOConfig.cmake  @ONLY   )

INSTALL( FILES ${QTRECO_BINARY_DIR}/QTRECOConfig.cmake
         DESTINATION ${CONFIG_INSTALL_DIR}                               )

# add library install path to the rpath list
SET( CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib" )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH )

# add install path to the rpath list (apple)
IF( APPLE )
    SET( CMAKE_INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib" )
    MARK_AS_ADVANCED( CMAKE_INSTALL_NAME_DIR )
ENDIF()

# append link pathes to rpath list
SET( CMAKE_INSTALL_RPATH_USE_LINK_PATH 1 )
MARK_AS_ADVANCED( CMAKE_INSTALL_RPATH_USE_LINK_PATH )


################################################################
# Git                                                          #
################################################################

OPTION ( USE_GIT "Set to OFF to skip git pull" ON )

IF( USE_GIT )
FIND_PACKAGE( Git )
IF( GIT_FOUND )

	MESSAGE( STATUS "" )
    MESSAGE( STATUS "---------------------------------------" )
    MESSAGE( STATUS "    Git found: ${GIT_EXECUTABLE}       " )
    MESSAGE( STATUS "    Pulling last Qt Reco version       " )
    MESSAGE( STATUS "---------------------------------------" )
    MESSAGE( STATUS "" )
    
    EXECUTE_PROCESS(
      COMMAND git pull
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
ELSE ( GIT_FOUND )
    MESSAGE( WARNING "************************************************************")
    MESSAGE( WARNING "*      Git was not found! No pulling can be done.          *")
    MESSAGE( WARNING "************************************************************")
ENDIF( GIT_FOUND )
ENDIF()

################################################################
# ROOT                                                         #
################################################################

# ----- Find ROOT package -----
LIST( APPEND ROOT_COMPONENT_LIST Core Hist Rint HistPainter Graf Graf3d MathCore Net RIO Tree QtRoot GQt Gui )
FIND_PACKAGE( ROOT COMPONENTS ${ROOT_COMPONENT_LIST} REQUIRED )

################################################################
# Qt                                                           #
################################################################

OPTION ( WITH_QT5 "Set to ON to build using Qt 5" OFF )

IF( DEFINED ENV{QTDIR} )
MESSAGE( STATUS "QT_ROOT_DIR Found in $ENV{QTDIR}" )
SET( QT_ROOT_DIR "$ENV{QTDIR}" CACHE PATH "Qt directory" FORCE )
ELSE ( DEFINED ENV{QTDIR} )
SET( QT_ROOT_DIR "CMAKE_PREFIX_PATH" CACHE PATH "Qt directory" FORCE )
ENDIF ( DEFINED ENV{QTDIR} )

SET( QT_QMAKE_EXECUTABLE ${QT_ROOT_DIR}/bin/qmake )

IF( WITH_QT5 )
FIND_PACKAGE( Qt5 COMPONENTS Core Gui Widgets Network REQUIRED )
ELSE ( WITH_QT5 )
FIND_PACKAGE( Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED )
ADD_DEFINITIONS ( ${QT_DEFINITIONS} )
INCLUDE( ${QT_USE_FILE} )
ENDIF( WITH_QT5 )

# ----- Add source directory -----
ADD_SUBDIRECTORY( ./source )

################################################################
# documentation                                                #
################################################################

OPTION( BUILD_DOCUMENTATION  "Set to OFF to skip build/install Documentation" ON )

IF( BUILD_DOCUMENTATION )
INCLUDE( Documentation )
ENDIF()

################################################################
# print status report                                          #
################################################################
MESSAGE( STATUS )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX                    = ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "USE_GIT                                 = ${USE_GIT}" )
MESSAGE( STATUS "BUILD_DOCUMENTATION                     = ${BUILD_DOCUMENTATION}" )
MESSAGE( STATUS "WITH_QT5                                = ${WITH_QT5}" )
MESSAGE( STATUS "QT_QMAKE_EXECUTABLE                     = ${QT_QMAKE_EXECUTABLE}" )
MESSAGE( STATUS "FULL_LIB_INSTALL_DIR                    = ${FULL_LIB_INSTALL_DIR}" )
MESSAGE( STATUS "FULL_INCLUDE_INSTALL_DIR                = ${FULL_INCLUDE_INSTALL_DIR}" )

MESSAGE( STATUS "Change a value with: cmake -D<Variable>=<Value>" )
MESSAGE( STATUS "-------------------------------------------------------------------------------" )