#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(../common.pri)
QT       -= gui
QT += network
isEmpty(PYTHON_ROOT) {
    message("No Python support will be built")
}
requires(!isEmpty(PYTHON_ROOT))

TARGET = anywave
TEMPLATE = lib
CONFIG += plugin no_plugin_name_prefix
QMAKE_EXTENSION_SHLIB = so

macx {
    DESTDIR = $$DESTDIR/AnyWave.app/Contents/Python
}
unix:!macx {
    DESTDIR = $$DESTDIR/Python
}

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PYTHON_ROOT/include/python2.7 $$PYTHON_ROOT/include/python2.7  $$PYTHON_ROOT/lib/python2.7/site-packages/numpy/core/include
LIBS += -L$$PYTHON_ROOT/lib -lpython2.7

macx {
    LIBS += -framework AwCore
}

HEADERS += \
    common.h \
    pythonmodule.h

SOURCES += \
    AddMarkers.cpp \
    Channel.cpp \
    common.cpp \
    GetData.cpp \
    GetMarkers.cpp \
    GetPluginInfo.cpp \
    Marker.cpp \
    pythonmodule.cpp \
    SendMessage.cpp

