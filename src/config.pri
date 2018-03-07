#-------------------------------------------------
#
# Configuration file to build AnyWave using VTK, Python, MATLAB
#
#-------------------------------------------------

# specify the VTK version used
VTK_VERSION_SUFFIX = 
# VTK_INCLUDE_PATH  Path where to find headers for vtk
VTK_INCLUDE_PATH = /home/bruno/vtk8.1/include/vtk-8.1
# VTK_LIB_DIR Path where to find vtk libraries
VTK_LIB_PATH = /home/bruno/vtk8.1/lib

# ARMA_INCLUDE_PATH  Path to find armadillo C++ include files
ARMA_INCLUDE_PATH = $$PWD/../armadillo/include
# H5_INCLUDE_PATH Path to HDF5 headers
H5_INCLUDE_PATH = /home/bruno/hdf5/include
# H5_LIB_PATH
H5_LIB_PATH = /home/bruno/hdf5/lib
# MATIO PATHS
MATIO_INCLUDE_PATH = /home/bruno/matio/include
MATIO_LIB_PATH = /home/bruno/matio/lib
# QWT
QWT_INCLUDE_PATH=/usr/include/qwt
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
