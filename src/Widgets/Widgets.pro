#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(../common.pri)
TARGET = AwWidgets
TEMPLATE = lib
#CONFIG += plugin qwt
CONFIG += plugin
DEFINES += AW_BUILD_WIDGETS_LIB
DESTDIR = $$LIB_DIR
QT += widgets
include(Widgets.pri)

macx {
    LIBS += -framework AwCore -framework qwt
}

macx{
   QMAKE_LFLAGS_PLUGIN += -Wl,-install_name,@executable_path/../Frameworks/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
}

unix:!macx{
 LIBS += -lAwCore $$VTK_LIBRARIES
}

LIBS += -lAwGraphics -lAwUtilities -lAwMath -lAwLayout -lAwMapping
