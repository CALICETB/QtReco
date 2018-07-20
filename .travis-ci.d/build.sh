#!/bin/bash

ILCSOFT=/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -DBUILD_DOCUMENTATION=OFF -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
make
