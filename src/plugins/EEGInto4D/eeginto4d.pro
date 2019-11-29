#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(config.pri)

TARGET = EEGInto4D
TEMPLATE = lib
CONFIG += plugin qwt
DESTDIR = $$PLUGIN_DIR
QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lAwRW -lAwProcess -lAwMath -lAwWidgets

macx {
LIBS += -framework Accelerate
}

unix:!macx {
  LIBS += -lopenblas
}

FORMS += \
    settings.ui


HEADERS += \
    settings.h eeginto4d.h

SOURCES += \
 settings.cpp eeginto4d.cpp



