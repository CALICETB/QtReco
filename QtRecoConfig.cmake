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

SET( QtReco_ROOT "/afs/desy.de/group/flc/pool/saivahu/QtReco" )
SET( QtReco_VERSION ".." )


# ---------- include dirs -----------------------------------------------------
# do not store find results in cache
SET( QtReco_INCLUDE_DIRS QtReco_INCLUDE_DIRS-NOTFOUND )
MARK_AS_ADVANCED( QtReco_INCLUDE_DIRS )

FIND_PATH( QtReco_INCLUDE_DIRS
	NAMES QtRecoConfig.h
	PATHS /afs/desy.de/group/flc/pool/saivahu/QtReco/include/
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
SET( QtReco_DEPENDS_INCLUDE_DIRS  /cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/include;/cvmfs/ilc.desy.de/sw/boost/1.58.0 )
SET( QtReco_DEPENDS_LIBRARY_DIRS  /cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib )
SET( QtReco_DEPENDS_LIBRARIES  /cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libCore.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libRIO.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libNet.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libHist.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libGraf.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libGraf3d.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libGpad.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libTree.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libRint.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libPostscript.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libMatrix.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libPhysics.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libMathCore.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libThread.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libMultiProc.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libCore.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libHist.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libRint.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libGraf.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libGraf3d.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libMathCore.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libNet.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libRIO.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11/root/6.08.00/lib/libTree.so;/usr/lib64/libdl.so;/usr/lib64/libglut.so;/usr/lib64/libXmu.so;/usr/lib64/libXi.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/QT/4.7.4/lib/libQtOpenGL.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/QT/4.7.4/lib/libQtGui.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/QT/4.7.4/lib/libQtNetwork.so;/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/QT/4.7.4/lib/libQtCore.so )
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
