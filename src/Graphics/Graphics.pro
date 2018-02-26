#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(../common.pri)
QT += widgets
TARGET = AwGraphics
TEMPLATE = lib
CONFIG += qwt plugin
DEFINES += AW_BUILD_GRAPHICS_LIB
DESTDIR = $$LIB_DIR

#FRAMEWORK_HEADERS.version = Versions
#FRAMEWORK_HEADERS.files =    ../../include/graphics/AwGraphicsObjects.h   ../../include/AwGlobal.h \
#    ../../include/AwAmplitudeManager.h \
#    ../../include/graphics/AwColorMap.h \
#    ../../include/graphics/AwCursorItem.h \
#    ../../include/graphics/AwGraphicInterface.h \
#    ../../include/graphics/AwGraphicsDefines.h \
#    ../../include/graphics/AwGraphicsObjects.h \
#    ../../include/graphics/AwHighLightMarker.h \
#    ../../include/graphics/AwHighLightPositionMarker.h \
#    ../../include/graphics/AwLabelItem.h \
#    ../../include/graphics/AwMappingCursorItem.h \
#    ../../include/graphics/AwMarkerChannelItem.h \
#    ../../include/graphics/AwMarkerItem.h \
#    ../../include/graphics/AwQwtColorMap.h \
#    ../../include/graphics/AwSignalItem.h \
#    ../../include/graphics/AwSignalLabelItem.h
#FRAMEWORK_HEADERS.path = Headers
#QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Mark/AwHighLightPositionMarker.cpp \
    MarkerItem/AwCursorItem.cpp \
    MarkerItem/AwHighLightMarker.cpp \
    MarkerItem/AwMappingCursorItem.cpp \
    MarkerItem/AwMarkerChannelItem.cpp \
    MarkerItem/AwMarkerItem.cpp \
    MarkerItem/AwMarkerLabelButton.cpp \
    MarkerItem/AwMarkerValueButton.cpp \
    SignalItem/AwLabelItem.cpp \
    SignalItem/AwSIAmpButton.cpp \
    SignalItem/AwSignalItem.cpp \
    SignalItem/AwSignalItemSettings.cpp \
    SignalItem/AwSignalLabelItem.cpp \
    AwAmplitudeManager.cpp \
    AwColorMap.cpp \
    AwGraphicsObjects.cpp \
    AwQwtColorMap.cpp

HEADERS += ../../include/AwAmplitudeManager.h \
    MarkerItem/AwMarkerLabelButton.h \
    MarkerItem/AwMarkerValueButton.h \
    SignalItem/AwSIAmpButton.h \
    SignalItem/AwSignalItemSettings.h \
    ../../include/graphics/AwColorMap.h \
    ../../include/graphics/AwCursorItem.h \
    ../../include/graphics/AwGraphicInterface.h \
    ../../include/graphics/AwGraphicsDefines.h \
    ../../include/graphics/AwGraphicsObjects.h \
    ../../include/graphics/AwHighLightMarker.h \
    ../../include/graphics/AwHighLightPositionMarker.h \
    ../../include/graphics/AwLabelItem.h \
    ../../include/graphics/AwMappingCursorItem.h \
    ../../include/graphics/AwMarkerChannelItem.h \
    ../../include/graphics/AwMarkerItem.h \
    ../../include/graphics/AwQwtColorMap.h \
    ../../include/graphics/AwSignalItem.h \
    ../../include/graphics/AwSignalLabelItem.h

macx {
    LIBS += -framework AwCore
}

unix:!macx{
 LIBS += -lAwCore
}

LIBS += -lAwUtilities

unix {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}

FORMS += \
    SignalItem/AwSignalItemSettings.ui

RESOURCES += \
    ../../resources/amplitudes.qrc
