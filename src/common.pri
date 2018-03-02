#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------

include(config.pri)
CONFIG += debug_and_release
INCLUDEPATH += $$INCLUDEDIR

!isEmpty(ARMA_INCLUDE_PATH) {
    INCLUDEPATH += $$ARMA_INCLUDE_PATH
}
!isEmpty(VTK_INCLUDE_PATH) {
    INCLUDEPATH += $$VTK_INCLUDE_PATH
}
!isEmpty(H5_INCLUDE_PATH) {
    INCLUDEPATH += $$H5_INCLUDE_PATH
}
!isEmpty(MATIO_INCLUDE_PATH) {
    INCLUDEPATH += $$MATIO_INCLUDE_PATH
}
!isEmpty(QWT_INCLUDE_PATH) {
    INCLUDEPATH += $$QWT_INCLUDE_PATH
}

DESTDIR = $$OUTDIR

CONFIG(debug, debug|release) {
    DESTDIR = $$DESTDIR/debug
}
CONFIG(release, debug|release) {
    DESTDIR = $$DESTDIR/release
}

LIB_DIR = $$DESTDIR/lib

macx {
  LIBS += -F$$LIB_DIR
}

!isEmpty(H5_LIB_PATH) {
    LIBS += -L$$H5_LIB_PATH
}
!isEmpty(VTK_LIB_PATH) {
     LIBS += -L$$VTK_LIB_PATH
}
!isEmpty(MATIO_LIB_PATH) {
     LIBS += -L$$MATIO_LIB_PATH
}
!isEmpty(QWT_LIB_PATH) {
     LIBS += -L$$QWT_LIB_PATH
}


LIBS += -L$$LIB_DIR

DEFINES += ARMA_DONT_USE_WRAPPER NDEBUG
macx {
INSTALL_LIB_PATH = $$DESTDIR/AnyWave.app/Contents/dylibs
INSTALL_APP_PATH = $$DESTDIR/AnyWave.app/Contents/MacOS
PLUGIN_DIR = $$DESTDIR/Anywave_Plugins
}

unix:!macx {
    PLUGIN_DIR = $$DESTDIR/Plugins
}

CONFIG += release warn_off c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
unix:!macx{
QMAKE_CXXFLAGS_RELEASE += -fopenmp
}
