#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

QT       -= gui
QT += concurrent
include(../common.pri)
TARGET = AwFilter
TEMPLATE = lib
CONFIG += plugin
DEFINES += AW_BUILD_FILTER_LIB
DESTDIR = $$LIB_DIR

#FRAMEWORK_HEADERS.version = Versions
#FRAMEWORK_HEADERS.files =    ../../include/AwFiltering.h  ../../include/AwGlobal.h
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
    AwButterWorth.cpp \
    AwFiltering.cpp \
    DspFilters/Bessel.cpp \
    DspFilters/Biquad.cpp \
    DspFilters/Butterworth.cpp \
    DspFilters/Cascade.cpp \
    DspFilters/ChebyshevI.cpp \
    DspFilters/ChebyshevII.cpp \
    DspFilters/Custom.cpp \
    DspFilters/Design.cpp \
    DspFilters/Documentation.cpp \
    DspFilters/Elliptic.cpp \
    DspFilters/Filter.cpp \
    DspFilters/Legendre.cpp \
    DspFilters/Param.cpp \
    DspFilters/PoleFilter.cpp \
    DspFilters/RBJ.cpp \
    DspFilters/RootFinder.cpp \
    DspFilters/State.cpp


HEADERS += \
    ../../include/AwFiltering.h

macx {
    LIBS += -framework AwCore
}

unix:!macx{
 LIBS += -lAwCore
}

LIBS += -lAwMath

unix {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}


