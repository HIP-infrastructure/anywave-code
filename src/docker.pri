
VTK_INCLUDE_PATH = /usr/include/vtk-7.1
VTK_LIB_PATH = /usr/lib/x86_64-linux-gnu
# ARMA_INCLUDE_PATH  Path to find armadillo C++ include files
ARMA_INCLUDE_PATH = $$PWD/../armadillo/include
H5_INCLUDE_PATH = /usr/include/hdf5/serial
QWT_INCLUDE_PATH = /usr/include/qwt


QMAKE_RPATHDIR += '$ORIGIN/lib'

# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui