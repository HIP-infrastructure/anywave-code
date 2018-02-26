#-------------------------------------------------
#
# Configuration file to build AnyWave using VTK, Python, MATLAB
#
#-------------------------------------------------

# AWSDK Path to the source directory
AWSDK = 
# AW_OUT_DIR path to put binaries (Create the AnyWave.app in that folder : use the zip in resources dir)
AW_OUT_DIR = $$AWSDK/qmake_build
# specify the VTK version used
VTK_VERSION = 8.1
# VTK_INCLUDE_PATH  Path where to find headers for vtk
VTK_INCLUDE_PATH = 
# VTK_LIB_DIR Path where to find vtk libraries
VTK_LIB_PATH = 
# ARMA_INCLUDE_PATH  Path to find armadillo C++ include files
ARMA_INCLUDE_PATH = $$AWSDK/armadillo/include
# H5_INCLUDE_PATH Path to HDF5 headers
H5_INCLUDE_PATH = 
# H5_LIB_PATH
H5_LIB_PATH = 
# MATIO PATHS
MATIO_INCLUDE_PATH = 
MATIO_LIB_PATH = 

# MATLAB_ROOT  Path to the installation of MATLAB
macx {
MATLAB_ROOT = /Applications/MATLAB_R2015a.app
}

unix:!macx {
MATLAB_ROOT = /usr/local/MATLAB/R2015a
}
# PYTHON PATH (Anaconda recommenced)
macx {
PYTHON_ROOT = 
}

unix:!macx{
PYTHON_ROOT = 
}

macx {
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10
}

# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui
