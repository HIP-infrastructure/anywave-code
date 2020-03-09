## Get ready to build using Ubuntu 18.04

```bash
sudo apt-get -y update && apt-get -y install git cmake qtbase5-dev
    qt5-default libvtk7-qt-dev libopenblas-base libopenblas-dev qtmultimedia5-dev libqt5x11extras5-dev
    libmatio-dev tcsh libfftw3-dev libqwt-qt5-dev qttools5-dev-tools libqt5svg5-dev libxt-dev libhdf5-dev
    build-essential libpython3.6-dev python3-numpy 
```
You should be able to adapt the script above for your own distribution.   
Note that cmake v3 is required and some distributions (CentOS) only provides CMake 2.8.   
You may have to add extra source repositories to get cmake3.

## Packages required for Ubuntu 16.xx

```bash
sudo apt-get -y update && apt-get -y install git cmake qtbase5-dev libqt5dbus5 
    qt5-default libvtk6-qt-dev libopenblas-base libopenblas-dev qtmultimedia5-dev libqt5x11extras5-dev
    libmatio-dev tcsh libfftw3-dev libqwt-qt5-dev qttools5-dev-tools libqt5svg5-dev libxt-dev libhdf5-dev
    build-essential libpython2.7-dev python2.7 python-numpy libproj-dev libqt5x11extras5 libqt5multimedia5
```

WARNING: On Ubuntu 16.04 some Qt5 packages are not availabel (Qt5Charts Qt5Declarative). 
Download and install Qt 5.9 or earlier from Qt5 website before building AnyWave.

## Get the code
```bash
git clone https://gitlab.thevirtualbrain.org/anywave/AnyWave.git 
cd AnyWave
```
## Build with CMake
```bash
mkdir build && cd build
cmake ..
sudo make install
```

By default the installation path is /usr/local/AnyWave but you can change the prefix path using:   
```bash
cmake .. -DCMAKE_PREFIX_PATH=/home/user
```
The application will then be installed in /home/user/AnyWave   
The binaries are built using RPATH to the CMAKE_PREFIX_PATH and a link called anywave will be created in /usr/bin    

# MATLAB
MATLAB support should be automatic if you have MATLAB installed in /usr/local/MATLAB  
That will build the mex files required by MATLAB Plugins to communicate with AnyWave.

## Use MKL
If you have installed the MKL libraries on your system, AnyWave can use it to speed up some calculations.
Before building using cmake, execute the following script:   
````bash
source /opt/intel/mkl/bin/mklvars.sh intel64
````
CMake will detect and use MKL to build AnyWave

Launch Anywave:
```bash
anywave
```