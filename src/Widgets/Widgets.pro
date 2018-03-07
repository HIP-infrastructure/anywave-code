#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(../common.pri)
TARGET = AwWidgets
TEMPLATE = lib
CONFIG += plugin qwt
DEFINES += AW_BUILD_WIDGETS_LIB
DESTDIR = $$LIB_DIR
QT += widgets
include(Widgets.pri)

macx {
    LIBS += -framework AwCore
}

unix:!macx{
 LIBS += -lAwCore
}

LIBS += -lAwGraphics -lAwUtilities -lAwMath -lAwLayout -lAwMapping  $$VTK_LIBRARIES
