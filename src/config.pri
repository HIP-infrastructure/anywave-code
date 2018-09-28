#-------------------------------------------------
#
# Configuration file to build AnyWave using VTK, Python, MATLAB
#
#-------------------------------------------------

# specify the VTK version used
VTK_VERSION_SUFFIX = 8.1
# VTK_INCLUDE_PATH  Path where to find headers for vtk
# Linux
unix:!macx {
VTK_INCLUDE_PATH = /home/bruno/VTK/include/vtk-$$VTK_VERSION_SUFFIX
VTK_LIB_PATH = /home/bruno/VTK/lib
}
# Mac OS X
macx {
VTK_INCLUDE_PATH = /Users/bruno/vtk810/include/vtk-$$VTK_VERSION_SUFFIX
VTK_LIB_PATH = /Users/bruno/vtk810/lib
}

# ARMA_INCLUDE_PATH  Path to find armadillo C++ include files
ARMA_INCLUDE_PATH = $$PWD/../armadillo/include

# HDF5 DIRS
# Linux
unix:!macx {
H5_INCLUDE_PATH = /home/bruno/hdf5/include
H5_LIB_PATH = /home/bruno/hdf5/lib
}
# Mac OS X
macx {
H5_INCLUDE_PATH = /Users/bruno/hdf5/include
H5_LIB_PATH = /Users/bruno/hdf5/lib
}

# MATIO PATHS
# Linux
unix:!macx {
MATIO_INCLUDE_PATH = /home/bruno/matio/include
MATIO_LIB_PATH = /home/bruno/matio/lib
}
# Mac OS X
macx {
MATIO_INCLUDE_PATH = /Users/bruno/matio/include
MATIO_LIB_PATH = /Users/bruno/matio/lib
}


# QWT
unix:!mac{
QWT_INCLUDE_PATH=/usr/include/qwt
QWT_LIB_PATH=/usr/lib
}
macx {
INCLUDEPATH += /Library/Frameworks/qwt.framework/Headers
}
# MATLAB_ROOT  Path to the installation of MATLAB
macx {
MATLAB_ROOT = /Applications/MATLAB_R2015a.app
}

unix:!macx {
MATLAB_ROOT = /usr/local/MATLAB/R2018b
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
# add rpath to frameworks for all targets
QMAKE_RPATHDIR += /Library/Frameworks
QMAKE_RPATHDIR += @executable_path/../Frameworks
}

unix:!macx{
QMAKE_RPATHDIR += '$ORIGIN/lib'
}

# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui


