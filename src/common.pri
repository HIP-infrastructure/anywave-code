#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------

include(config.pri)
CONFIG += release
INCLUDEPATH += $$INCLUDEDIR

!isEmpty(ARMA_INCLUDE_PATH) {
    INCLUDEPATH += $$ARMA_INCLUDE_PATH
    DEFINES += ARMA_DONT_USE_WRAPPER
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
HEADER_DIR = $$DESTDIR

macx {
  LIBS += -F$$LIB_DIR -F/Library/Frameworks 
}

!isEmpty(H5_LIB_PATH) {
    LIBS += -L$$H5_LIB_PATH
}

# extract all .so files present in VTK_LIB_PATH and format them as flags for the linker.
!isEmpty(VTK_LIB_PATH) {
     LIBS += -L$$VTK_LIB_PATH
 #    VTK_LIBRARIES += -L$$VTK_LIB_PATH
     unix:!macx {
        LIST = $$files($$VTK_LIB_PATH/*.so)
        for(f, LIST) {
    # remove first lib and last .so
          base = $$basename(f)
          noso = $$section(base, .so, 0 , 0)
          nolib = $$section(noso, lib, 1, 3)
          VTK_LIBRARIES += -l$$nolib
        }
      }
      macx {
        LIST = $$files($$VTK_LIB_PATH/*.dylib)
        for(f, LIST) {
    # remove first lib and last .dylib
          base = $$basename(f)
          noso = $$section(base, .dylib, 0 , 0)
          nolib = $$section(noso, lib, 1, 3)
          VTK_LIBRARIES += -l$$nolib
        }
      }
}

!isEmpty(MATIO_LIB_PATH) {
     LIBS += -L$$MATIO_LIB_PATH
}

!isEmpty(QWT_LIB_PATH) {
     LIBS += -L$$QWT_LIB_PATH
}


LIBS += -L$$LIB_DIR

DEFINES +=  NDEBUG
macx {
INSTALL_LIB_PATH = $$DESTDIR/bin/AnyWave.app/Contents/Frameworks
INSTALL_APP_PATH = $$DESTDIR/bin/AnyWave.app/Contents/MacOS
PLUGIN_DIR = $$DESTDIR/bin/Anywave_Plugins
}

unix:!macx {
    PLUGIN_DIR = $$DESTDIR/Plugins
}

CONFIG += release warn_off c++11
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3

unix:!macx{
QMAKE_CXXFLAGS_RELEASE += -fopenmp
QMAKE_CXXFLAGS_RELEASE += -fPIC
}

macx { 
  QMAKE_CXXFLAGS += -framework qwt
}
