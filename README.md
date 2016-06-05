# Qt Interface for AHCAL Reconstruction during Testbeam

This package has been created by E.Brianne, DESY Hamburg.
QtReco performs the reconstruction of the AHCAL data during Testbeam via the Marlin framework . 
It includes a Qt Interface to deals with the reconstruction and also a module perform the analysis of the data. 

## Dependencies

* g++ 4.7 minimum
* CMake 2.6 
* Qt version 4.7 or 5
* Ilcsoft (Marlin, LCIO) not directly with GUI compilation
* Calice Soft (Marlin processors for AHCAL Reconstruction) 
* ROOT v5.30.XX
* GLUT 
* Git optional
* Doxygen optional
* Boost 1.36 minimum

## Installation

Fairly simple, download the latest version :
<pre>
git clone https://github.com/CALICETB/QtReco.git
</pre>

To compile do for Qt 4 :
<pre>
export QTDIR=/path/to/qt4
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=. -DWITH_QT5=OFF -DBUILD_DOCUMENTATION=ON ..
make 
make install
</pre>

For Qt5 :
<pre>
export QTDIR=/path/to/qt5
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=. -DWITH_QT5=ON -DBUILD_DOCUMENTATION=ON ..
make 
make install
</pre>
