## Get ready to build AnyWave

Install or build dependencies:  
The examples below are for ubuntu systems but it should build also on other distros if you pick up the corresponding packages names.

1. CMake   

cmake is required to build, if you want to build a compatible version of cmake do:
```bash
$ curl -L -O 'https://github.com/Kitware/CMake/releases/download/v3.13.2/cmake-3.13.2-Linux-x86_64.sh' && \
    chmod a+x cmake-3.13.2-Linux-x86_64.sh 
$ sudo ./cmake-3.13.2-Linux-x86_64.sh --prefix=/usr --skip-license
```

2. Qt5 libs and tools
```bash
$ sudo apt-get install -y   libdbus-1-3 qt5-default qt5-qmake  qtbase5-dev-tools  libqt5opengl5 libqt5x11extras5
```

3. Git
```bash
$ sudo apt-get install -y   git
```

4. building tools
   
```bash
$ sudo apt-get install -y   build-essentials
```

5. Qwt  
 ```bash
$ sudo apt-get install -y  libqwt-qt5-dev
```

6. openblas
 ```bash
$ sudo apt-get install -y  libopenblas-dev
```

7. VTK
 ```bash
$ sudo apt-get -y update && apt-get install -y \ 
libxt-dev libqt5x11extras5-dev qttools5-dev
$ git clone https://github.com/Kitware/VTK.git vtk
$ cd vtk && git checkout v8.2.0 && cd .. 
$ mkdir build && cd build
$  cmake ../vtk -DCMAKE_INSTALL_PREFIX=/usr/local -DModule_vtkGUISupportQt=ON \
  -DModule_vtkGUISupportQtOpenGL=ON -DModule_vtkRenderingQt=ON -DModule_vtkViewsQt=ON -DVTK_RENDERING_BACKEND=OpenGL2 -DVTK_QT_VERSION=5  \
  -DBUILD_TESTING=OFF -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON 
 $ make
 $ sudo make install
 ```

8. libmatio
```bash
$ sudo apt-get -y update && apt-get install -y \ 
 libmatio-dev
 ```
 
9. fftw
```bash
$ sudo apt-get -y update && apt-get install -y \ 
  libfftw3-dev
```
10. AnyWave
```bash
$ sudo  apt-get -y update && apt-get install -y \
  libqt5svg5-dev qtdeclarative5-dev  qtmultimedia5-dev libopenblas-dev python3-dev python3-numpy
$ git clone https://gitlab.dynamap.tvb-ins.fr/anywave/anywave.git
$ cd anywave
$ mkdir build && cd build && cmake ../anywave && make
$ sudo make install 
```


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