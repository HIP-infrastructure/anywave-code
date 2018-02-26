#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

QT       -= gui

include(../common.pri)
TARGET = AwMath
TEMPLATE = lib
CONFIG += plugin
DEFINES += AW_BUILD_MATH_LIB
DESTDIR = $$LIB_DIR
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10


#FRAMEWORK_HEADERS.version = Versions
#FRAMEWORK_HEADERS.files =    ../../include/AwGlobal.h ../../include/math/AwMath.h ../../include/math/AwSpline2D.h
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
    AwMath.cpp \
    AwSpline2D.cpp

macx {
    LIBS += -framework AwCore -framework Accelerate
}

unix:!macx{
 LIBS += -lAwCore -lopenblas
}


unix {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}


