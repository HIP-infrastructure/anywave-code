#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(../plugins.pri)
TARGET = TFWavelet
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$PLUGIN_DIR
QT += widgets gui concurrent
macx {
QMAKE_POST_LINK = \
  install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $${DESTDIR}/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
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

LIBS += -lAwRW -lAwProcess -lAwFilter

FORMS += \
    TFWidget.ui settingsui.ui

HEADERS += \
   display_settings.h settingsui.h TFPicker.h TFPlot.h tf_settings.h TFWavelet2.h TFWidget.h


SOURCES += \
    display_settings.cpp settingsui.cpp TFPicker.cpp TFPlot.cpp tf_settings.cpp TFWavelet2.cpp TFWidget.cpp


