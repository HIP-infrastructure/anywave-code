#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(config.pri)
TARGET = TFWavelet
TEMPLATE = lib
CONFIG += plugin qwt
DESTDIR = $$PLUGIN_DIR
QT += widgets concurrent

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

macx { # add FFTW3 DIRS
LIBS += -framework AwCore -L/Users/bruno/fftw/lib
INCLUDEPATH += /Users/bruno/fftw/include

}

unix:!macx{
LIBS += -lAwCore
}

LIBS += -lAwRW -lAwProcess -lAwMath -lAwWidgets -lAwGraphics -lAwLayout -lAwFilter -lfftw3


FORMS += \
    settingsui.ui \
    TFWidget.ui

HEADERS += \
    display_settings.h \
    settingsui.h \
    tf_settings.h \
    TFPicker.h \
    TFPlot.h \
    tfwavelet2_global.h \
    TFWavelet2.h \
    TFWidget.h

SOURCES += \
    display_settings.cpp \
    settingsui.cpp \
    tf_settings.cpp \
    TFPicker.cpp \
    TFPlot.cpp \
    TFWavelet2.cpp \
    TFWidget.cpp


