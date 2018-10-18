# config paths to build AnyWave plugin
# First: check for a AWROOT environment variable
# Second: manually set the path to the AnyWave root dir (where include and lib dirs are located).
AW_ROOT_DIR=/Users/bruno/dev/aw-gitlab
_AWROOT = $$(AWROOT)
isEmpty(_AWROOT) {
   _AWROOT = $$AW_ROOT_DIR
}
isEmpty(_AWROOT) {
   message("Please set AW_ROOT_DIR in the file or the AWROOT environment variable")
}

PLUGIN_DIR = $$_AWROOT/qmake_build/release/bin/Anywave_Plugins
QMAKE_RPATHDIR += /Library/Frameworks
QMAKE_RPATHDIR += @executable_path/../Frameworks

DESTDIR = $$PLUGIN_DIR
LIBS += -L$$_AWROOT/qmake_build/release/lib -F/Library/Frameworks

LIBS += -F$$_AWROOT/lib
LIBS += -framework AwCore

DEFINES += ARMA_DONT_USE_WRAPPER NDEBUG

CONFIG += release warn_off c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -Wc++11-narrowing

INCLUDEPATH += $$_AWROOT/include $$_AWROOT/armadillo/include

LIBS += -L$$_AWROOT/lib
# build moc and objects file in a directory
OBJECTS_DIR = ./tmp
MOC_DIR = ./moc
UI_DIR = ./ui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10

