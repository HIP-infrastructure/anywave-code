#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(../../common.pri)
include(../plugins.pri)

TARGET = Rewrite4D
TEMPLATE = lib
CONFIG += plugin
DESTDIR = $$PLUGIN_DIR
QT += widgets

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS USE_ARMADILLO

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lAwRW -lAwProcess -lAwMath -lAwWidgets -lAwFilter

macx {
LIBS += -framework Accelerate
}

unix:!macx {
  LIBS += -lopenblas
}

FORMS += \
    rewriteui.ui ChooseMarker.ui


HEADERS += \
    rewrite4d.h rewriteui.h ChooseMarker.h

SOURCES += \
 rewrite4d.cpp rewriteui.cpp ChooseMarker.cpp
