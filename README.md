## Get ready to build using Ubuntu 18.04

```bash
sudo apt-get -y update && apt-get -y install git cmake qtbase5-dev libqt5dbus5 
    qt5-default libvtk7-qt-dev libopenblas-base libopenblas-dev libqt5charts5-dev qtdeclaratives5-dev
    libmatio-dev tcsh libfftw3-dev libqwt-qt5-dev qttools5-dev-tools libqt5svg5-dev libxt-dev libhdf5-dev
    build-essential libpython2.7-dev python2.7 python-numpy
```
You should be able to adapt the script above for your own distribution.   
Note that cmake v3 is required and some distributions (CentOS) only provides CMake 2.8.   
You may have to add extra source repositories to get cmake3.

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

MATLAB support should be automatic if you have MATLAB installed in /usr/local/MATLAB   

Launch Anywave:
```bash
anywave
=======
## Get ready to build using Ubuntu 18.04

```bash
sudo apt-get -y update && apt-get -y install git cmake qtbase5-dev libqt5dbus5 
    qt5-default libvtk7-qt-dev libopenblas-base libopenblas-dev libqt5charts5-dev
    libmatio-dev tcsh libfftw3-dev libqwt-qt5-dev qttools5-dev-tools libqt5svg5-dev libxt-dev libhdf5-dev
    build-essential libpython2.7-dev python2.7 python-numpy
```
You should be able to adapt the script above for your own distribution.   
Note that cmake v3 is required and some distributions (CentOS) only provides CMake 2.8.   
You may have to add extra source repositories to get cmake3.

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
Launch Anywave:
```bash
anywave
```