#!/bin/bash

ILC_SOFT=/home/calice/ilcsoft/v01-17-09
MY_INSTALL=/home/calice/Calice_soft/myInstall

source ${ILC_SOFT}/init_ilcsoft.sh

export MARLIN_DLL=
export MARLIN_DLL=${MY_INSTALL}/lib/libuserlib.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/libcaliceAnalysis.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/libraw2calohit.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/libcellGeometry.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/librecoSiPM.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/liblabview2lcio.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/libclustering.so
export MARLIN_DLL=${MARLIN_DLL}:${MY_INSTALL}/lib/libRootTreeWriter.so

# skip unloading processors
# to avoid the problems to release the root library at the end of Marlin job
export MARLIN_DEBUG=1

#next line prevents empty cdb.log file
export COND_DB_DEBUGLOG=/dev/stdout

exec ${ILC_SOFT}/Marlin/*/bin/Marlin $@
