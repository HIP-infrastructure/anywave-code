#-------------------------------------------------
#
# Configuration file to build AnyWave using VTK, Python, MATLAB
#
#-------------------------------------------------

# specify the VTK version used
VTK_VERSION_SUFFIX = 8.1
# VTK_INCLUDE_PATH  Path where to find headers for vtk
VTK_INCLUDE_PATH = /Users/bruno/VTK810/include/vtk-$$VTK_VERSION_SUFFIX
# VTK_LIB_DIR Path where to find vtk libraries
VTK_LIB_PATH = /Users/bruno/VTK810/lib

# ARMA_INCLUDE_PATH  Path to find armadillo C++ include files
ARMA_INCLUDE_PATH = $$PWD/../armadillo/include

# H5_INCLUDE_PATH Path to HDF5 headers
H5_INCLUDE_PATH = /Users/bruno/HDF5/include
# H5_LIB_PATH
H5_LIB_PATH = /Users/bruno/HDF5/lib
# MATIO PATHS
MATIO_INCLUDE_PATH = /Users/bruno/matio/include
MATIO_LIB_PATH = /Users/bruno/matio/lib
# QWT
QWT_INCLUDE_PATH=
QWT_LIB_PATH=

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

