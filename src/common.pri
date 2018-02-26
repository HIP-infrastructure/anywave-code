#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------

include(config.pri)
INCLUDEPATH += $$AWSDK/include $$ARMA_INCLUDE_PATH $$VTK_INCLUDE_PATH $$H5_INCLUDE_PATH $$MATIO_INCLUDE_PATH
INCLUDEPATH += /usr/local/include

CONFIG(debug, debug|release) {
    AW_OUT_DIR = $$AW_OUT_DIR/debug
}
CONFIG(release, debug|release) {
    AW_OUT_DIR = $$AW_OUT_DIR/release
}

LIB_DIR = $$AW_OUT_DIR/lib

macx {
  LIBS += -F$$LIB_DIR
}


LIBS += -L$$LIB_DIR -L/usr/local/lib -L$$H5_LIB_PATH -L$$VTK_LIB_PATH -L$$MATIO_LIB_PATH


DEFINES += ARMA_DONT_USE_WRAPPER NDEBUG
macx {
INSTALL_LIB_PATH = $$AW_OUT_DIR/bin/AnyWave.app/Contents/dylibs
INSTALL_APP_PATH = $$AW_OUT_DIR/bin/AnyWave.app/Contents/MacOS
PLUGIN_DIR = $$AW_OUT_DIR/bin/Anywave_Plugins
}

unix:!macx {
    INSTALL_LIB_PATH = $$AW_OUT_DIR/bin/lib
    INSTALL_APP_PATH = $$AW_OUT_DIR/bin
    PLUGIN_DIR = $$AW_OUT_DIR/bin/Plugins
}

CONFIG += release warn_off c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
unix:!macx{
QMAKE_CXXFLAGS_RELEASE += -fopenmp
}
