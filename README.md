## Build for Ubuntu 18.04

```bash
sudo apt-get -y update && apt-get -y install cmake qtbase5-dev libqt5dbus5 
    qt5-default libvtk8-qt-dev libopenblas-base libopenblas-dev libqt5charts5-dev
    libmatio-dev tcsh libfftw3-dev libqwt-qt5-dev qttools5-dev-tools libqt5svg5-dev libxt-dev libhdf5-dev
    build-essential libpython2.7-dev python2.7 python-numpy
```

```bash
mkdir build && cd build
cmake ..
make install
```