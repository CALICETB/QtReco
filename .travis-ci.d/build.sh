#!/bin/bash

export ILCSOFT=/cvmfs/ilc.desy.de/sw/x86_64_gcc48_sl6/v01-17-11
export CC=/cvmfs/sft.cern.ch/lcg/external/gcc/4.8.1/x86_64-slc6-gcc48-opt/bin/gcc
export CXX=/cvmfs/sft.cern.ch/lcg/external/gcc/4.8.1/x86_64-slc6-gcc48-opt/bin/g++
source $ILCSOFT/init_ilcsoft.sh

cd /Package
mkdir build
cd build
cmake -C $ILCSOFT/ILCSoft.cmake -DBUILD_DOCUMENTATION=OFF -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" .. && \
make
