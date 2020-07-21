# config paths to build AnyWave plugin
# First: check for a AWROOT environment variable
# Second: manually set the path to the AnyWave root dir (where include and lib dirs are located).
AW_ROOT_DIR=/home/bruno/dev/aw-gitlab/qmake_build/release
_AWROOT = $$(AWROOT)
isEmpty(_AWROOT) {
   _AWROOT = $$AW_ROOT_DIR
}
isEmpty(_AWROOT) {
   message("Please set AW_ROOT_DIR in the file or the AWROOT environment variable")
}

macx {
   PLUGIN_DIR = $$_AWROOT/bin/Anywave_Plugins
}

unix:!macx {
 PLUGIN_DIR = $$_AWROOT/Plugins
}

DESTDIR = $$PLUGIN_DIR
LIBS += -L$$_AWROOT/lib

macx {
 LIBS += -F$$_AWROOT/lib
 LIBS += -framework AwCore
}
unix:!macx {
  LIBS += -lAwCore
}

DEFINES += ARMA_DONT_USE_WRAPPER NDEBUG
macx {
INSTALL_LIB_PATH = $$DESTDIR/bin/AnyWave.app/Contents/dylibs
INSTALL_APP_PATH = $$DESTDIR/bin/AnyWave.app/Contents/MacOS
}

CONFIG += release warn_off c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
unix:!macx{
QMAKE_CXXFLAGS_RELEASE += -fopenmp
}

INCLUDEPATH += $$_AWROOT/include $$_AWROOT/armadillo/include
LIBS += -L$$_AWROOT/lib
# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui

macx {
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10
}
