#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

QT += widgets
include(../common.pri)
TARGET = AwProcess
TEMPLATE = lib
CONFIG += plugin
DEFINES += AW_BUILD_PROCESS_LIB
DESTDIR = $$LIB_DIR

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


macx {
    LIBS += -framework AwCore
QMAKE_LFLAGS_PLUGIN += -Wl,-install_name,@rpath/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
}

unix:!macx{
 LIBS += -lAwCore
}

LIBS += -lAwFilter


unix {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}

SOURCES += \
    AwProcessGUIWidget.cpp \
    AwProcessIO.cpp \
    AwProcessLib.cpp

HEADERS += \
    ../../include/AwProcess.h \
    ../../include/AwProcessInterface.h \
    ../../include/AwProcessIO.h \
    ../../include/AwProcessLib.h \
    ../../include/process/AwProcessGUIWidget.h


