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
VTK_INCLUDE_PATH = /home/bruno/vtk8.1/include/vtk-$$VTK_VERSION_SUFFIX
VTK_LIB_PATH = /home/bruno/vtk8.1/lib
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
QWT_INCLUDE_PATH=
QWT_LIB_PATH=
}
macx {
QWT_INCLUDE_PATH=
QWT_LIB_PATH=
INCLUDEPATH += /Library/Frameworks/qwt.framework/Headers
}
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
# add rpath to frameworks for all targets
QMAKE_RPATHDIR += /Library/Frameworks
QMAKE_RPATHDIR += @executable_path/../Frameworks
}

# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui

