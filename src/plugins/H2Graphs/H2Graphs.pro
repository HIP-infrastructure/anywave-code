#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

macx{
include(config_macx.pri)
}

unix:!macx{
include(config_linux.pri)
}

TARGET = H2Graphs
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

LIBS += -lAwProcess -lAwMath -lAwWidgets -lAwGraphics -lAwMATLAB

macx {
LIBS += -framework Accelerate
}

unix:!macx {
  LIBS += -lopenblas
}

FORMS += \
    GraphManagerWidget.ui GraphWindow.ui

HEADERS += \
    GraphArrow.h GraphArrowBase.h GraphArrowDir.h \
    GraphArrowLag.h GraphColorMap.h GraphLegend.h GraphManagerWidget.h \
   GraphSensorItem.h GraphSet.h GraphSetModel.h GraphWindow.h H2Graphs.h

SOURCES += \
GraphArrow.cpp GraphArrowBase.cpp GraphArrowDir.cpp \
GraphArrowLag.cpp GraphColorMap.cpp GraphLegend.cpp GraphManagerWidget.cpp \
GraphSensorItem.cpp GraphSet.cpp GraphSetModel.cpp GraphWindow.cpp H2Graphs.cpp

RESOURCES += Graphs.qrc 




