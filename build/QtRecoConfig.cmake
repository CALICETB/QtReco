##############################################################################
# cmake configuration file for QtReco
#
# requires:
#   MacroCheckPackageLibs.cmake for checking package libraries
#   MacroExportPackageDeps.cmake for exporting package dependencies
#
# returns following variables:
#
#   QtReco_FOUND      : set to TRUE if QtReco found
#       if FIND_PACKAGE called with REQUIRED and COMPONENTS arguments
#       QtReco_FOUND is only set to TRUE if ALL components are also found
#       if REQUIRED is NOT set components may or may not be available
#
#   QtReco_ROOT       : path to this QtReco installation
#   QtReco_VERSION    : package version
#   QtReco_LIBRARIES  : list of QtReco libraries (NOT including COMPONENTS)
#   QtReco_INCLUDE_DIRS  : list of paths to be used with INCLUDE_DIRECTORIES
#   QtReco_LIBRARY_DIRS  : list of paths to be used with LINK_DIRECTORIES
#   QtReco_COMPONENT_LIBRARIES      : list of QtReco component libraries
#   QtReco_${COMPONENT}_FOUND       : set to TRUE or FALSE for each library
#   QtReco_${COMPONENT}_LIBRARY     : path to individual libraries
#   QtReco_${COMPONENT}_LIB_DEPENDS : individual library dependencies
#
# @author Jan Engels, Desy
##############################################################################

SET( QtReco_ROOT "/home/calice/Software/QtReco" )
SET( QtReco_VERSION ".." )


# ---------- include dirs -----------------------------------------------------
# do not store find results in cache
SET( QtReco_INCLUDE_DIRS QtReco_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( QtReco_INCLUDE_DIRS )

FIND_PATH( QtReco_INCLUDE_DIRS
	NAMES QtRecoConfig.h
	PATHS /home/calice/Software/QtReco/include/
	NO_DEFAULT_PATH
)

# ---------- libraries --------------------------------------------------------
INCLUDE( "/cmake/MacroCheckPackageLibs.cmake" )

# only standard libraries should be passed as arguments to CHECK_PACKAGE_LIBS
# additional components are set by cmake in variable PKG_FIND_COMPONENTS
# first argument should be the package name
CHECK_PACKAGE_LIBS( QtReco QtReco )


# ---------- dependencies -----------------------------------------------------
INCLUDE( "/cmake/MacroExportPackageDeps.cmake" )
# exports following package dependencies (if set)
# first argument of macro should be the package name
SET( QtReco_DEPENDS_INCLUDE_DIRS  /home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/include;/home/calice/Software/ILCSoftInstall/ilcsoft/boost/1.58.0/boost )
SET( QtReco_DEPENDS_LIBRARY_DIRS  /home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib )
SET( QtReco_DEPENDS_LIBRARIES  /home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libCore.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libCint.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libRIO.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libNet.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libHist.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libGraf.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libGraf3d.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libGpad.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libTree.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libRint.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libPostscript.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libMatrix.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libPhysics.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libMathCore.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libThread.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libCore.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libHist.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libRint.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libGraf.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libGraf3d.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libMathCore.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libNet.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libRIO.so;/home/calice/Software/ILCSoftInstall/ilcsoft/root/5.34.36/lib/libTree.so;/usr/lib/x86_64-linux-gnu/libdl.so;/usr/lib/x86_64-linux-gnu/libglut.so;/usr/lib/x86_64-linux-gnu/libXmu.so;/usr/lib/x86_64-linux-gnu/libXi.so;/home/calice/Software/ILCSoftInstall/ilcsoft/QT/4.7.4/lib/libQtOpenGL.so;/home/calice/Software/ILCSoftInstall/ilcsoft/QT/4.7.4/lib/libQtGui.so;/home/calice/Software/ILCSoftInstall/ilcsoft/QT/4.7.4/lib/libQtNetwork.so;/home/calice/Software/ILCSoftInstall/ilcsoft/QT/4.7.4/lib/libQtCore.so )
EXPORT_PACKAGE_DEPENDENCIES( QtReco )



# ---------- final checking ---------------------------------------------------
INCLUDE( FindPackageHandleStandardArgs )
# set QtReco_FOUND to TRUE if all listed variables are TRUE and not empty
# QtReco_COMPONENT_VARIABLES will be set if FIND_PACKAGE is called with REQUIRED argument
FIND_PACKAGE_HANDLE_STANDARD_ARGS( QtReco DEFAULT_MSG QtReco_ROOT QtReco_INCLUDE_DIRS QtReco_LIBRARIES ${QtReco_COMPONENT_VARIABLES} )

SET( QtReco_FOUND ${QtReco_FOUND} )



# ---------- definitions ------------------------------------------------------
IF( QtReco_FOUND )

    IF( CMAKE_BUILD_TYPE STREQUAL "Debug" OR
        CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo" )
        MARK_AS_ADVANCED( QtReco_DEFINITIONS )
    ENDIF()

ENDIF()
