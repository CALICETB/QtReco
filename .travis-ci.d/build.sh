#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_${COMPILER}
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -DBUILD_DOCUMENTATION=OFF -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
make
