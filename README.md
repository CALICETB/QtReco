# Qt Interface for AHCAL Reconstruction during Testbeam

This package has been created by E.Brianne, DESY Hamburg.
QtReco performs the reconstruction of the AHCAL data during Testbeam via the Marlin framework . 
It includes a Qt Interface to deals with the reconstruction and also a module perform the analysis of the data. 

## Dependencies

* g++ 4.7 minimum
* CMake 2.6 
* ilcsoft v17-0X (v17-09 tested)
* Qt version 4.7
* ROOT v5.30.XX
* GLUT 
* Git
* Doxygen optional
* Boost 1.36
 
## Non-direct dependencies (Programs or librairies called during the reconstruction)

* Ilcsoft (Marlin, LCIO) 
* Calice Soft (Marlin processors for AHCAL Reconstruction) 

## Installation

Fairly simple, download the latest version :
<pre>
git clone https://github.com/CALICETB/QtReco.git
</pre>

<pre>
mkdir build
cd build
cmake -DBUILD_DOCUMENTATION=ON ..
make 
make install
</pre>
