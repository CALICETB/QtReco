#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/2016-11-22/x86_64-slc6-gcc48-opt
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -DBUILD_DOCUMENTATION=OFF -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
make -j4
