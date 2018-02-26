#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

QT       -= gui
include(../common.pri)

isEmpty(MATLAB_ROOT) {
    message("No MATLAB support will be built")
}
requires(!isEmpty(MATLAB_ROOT))

TARGET = AwMatlabSupport
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$INSTALL_LIB_PATH

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += $$MATLAB_ROOT/extern/include
macx {
   LIBS += -L$$MATLAB_ROOT/bin/maci64 -leng
}
unix:!macx {
 MATLAB_SYS_DIR = $$MATLAB_ROOT/sys/os/glnxa64
 LIBS += -L$$MATLAB_ROOT/bin/glnxa64 -leng
 QMAKE_RPATHDIR += $$MATLAB_SYS_DIR $$MATLAB_ROOT/bin/glnxa64
}


HEADERS += \
    AwMatlabSupport.h

SOURCES += \
    AwMatlabSupport.cpp

