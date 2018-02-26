#-------------------------------------------------
#
# Configuration file to build AnyWave using VTK, Python, MATLAB
#
#-------------------------------------------------

# AWSDK Path to the source directory
AWSDK = /home/bruno/dev/aw-svn
# AW_OUT_DIR path to put binaries (Create the AnyWave.app in that folder : use the zip in resources dir)
AW_OUT_DIR = /home/bruno/dev/aw-svn/qmake_build
# specify the VTK version used
VTK_VERSION = 8.1
# VTK_INCLUDE_PATH  Path where to find headers for vtk
VTK_INCLUDE_PATH = /home/bruno/vtk8.1/include/vtk-$$VTK_VERSION
# VTK_LIB_DIR Path where to find vtk libraries
VTK_LIB_PATH = /home/bruno/vtk8.1/lib
# ARMA_INCLUDE_PATH  Path to find armadillo C++ include files
ARMA_INCLUDE_PATH = /home/bruno/dev/armadillo-7.800.2/include
# H5_INCLUDE_PATH Path to HDF5 headers
H5_INCLUDE_PATH = /usr/include/hdf5/serial
# H5_LIB_PATH
H5_LIB_PATH = /usr/lib/x86_64-linux-gnu/hdf5/serial/lib
# MATIO PATHS
MATIO_INCLUDE_PATH = /home/bruno/matio/include
MATIO_LIB_PATH = /home/bruno/matio/lib

# MATLAB_ROOT  Path to the installation of MATLAB
macx {
MATLAB_ROOT = /Applications/MATLAB_R2015a.app
}

unix:!macx {
MATLAB_ROOT = /usr/local/MATLAB/R2015a
}
# PYTHON PATH (Anaconda recommenced)
macx {
PYTHON_ROOT = /Users/bruno/anaconda2
}

unix:!macx{
PYTHON_ROOT = /home/bruno/anaconda2
}

macx {
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10
}

# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui
