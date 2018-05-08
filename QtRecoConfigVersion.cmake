##############################################################################
# this file is parsed when FIND_PACKAGE is called with version argument
#
# @author Jan Engels, Desy IT
# Modified for QtReco use by : R.Ete, IPNL
##############################################################################


SET( ${PACKAGE_FIND_NAME}_VERSION_MAJOR  )
SET( ${PACKAGE_FIND_NAME}_VERSION_MINOR  )
SET( ${PACKAGE_FIND_NAME}_VERSION_PATCH  )


INCLUDE( "/cmake/MacroCheckPackageVersion.cmake" )
CHECK_PACKAGE_VERSION( ${PACKAGE_FIND_NAME} .. )
