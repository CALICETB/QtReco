#!/bin/bash

ILCSOFT=/cvmfs/clicdp.cern.ch/iLCSoft/builds/current/CI_gcc
export QTDIR=/cvmfs/clicdp.cern.ch/software/Qt/4.8.7/x86_64-slc6-gcc48-opt/
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -DBUILD_DOCUMENTATION=OFF -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
make -j4
