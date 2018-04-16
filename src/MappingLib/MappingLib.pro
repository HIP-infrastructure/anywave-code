#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

QT       -= gui
include(../common.pri)
TARGET = AwMapping
TEMPLATE = lib
CONFIG += plugin
DEFINES += AW_BUILD_MAPPING_LIB
DESTDIR = $$LIB_DIR

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

macx {
    LIBS += -framework AwCore  -framework OpenGL -framework ApplicationServices -framework AppKit
    LIBS += -lAwLayout -lvtkCommonCore-$$VTK_VERSION_SUFFIX -lvtkCommonDataModel-$$VTK_VERSION_SUFFIX -lvtkCommonMisc-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersCore-$$VTK_VERSION_SUFFIX -lvtkGUISupportQt-$$VTK_VERSION_SUFFIX -lvtkFiltersGeometry-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersModeling-$$VTK_VERSION_SUFFIX -lvtkRenderingCore-$$VTK_VERSION_SUFFIX -lvtkInteractionStyle-$$VTK_VERSION_SUFFIX -lvtkCommonExecutionModel-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersSources-$$VTK_VERSION_SUFFIX -lvtkRenderingOpenGL2-$$VTK_VERSION_SUFFIX -lvtkRenderingFreeType-$$VTK_VERSION_SUFFIX -lvtkRenderingAnnotation-$$VTK_VERSION_SUFFIX  \
    -lvtkIOCore-$$VTK_VERSION_SUFFIX -lvtkIOXML-$$VTK_VERSION_SUFFIX -lvtkzlib-$$VTK_VERSION_SUFFIX -lvtksys-$$VTK_VERSION_SUFFIX -lvtkFreeType-$$VTK_VERSION_SUFFIX \
    -lvtklz4-$$VTK_VERSION_SUFFIX -lvtkglew-$$VTK_VERSION_SUFFIX  \
    -lvtkIOXMLParser-$$VTK_VERSION_SUFFIX -lvtkexpat-$$VTK_VERSION_SUFFIX -lvtkImagingCore-$$VTK_VERSION_SUFFIX -lvtkCommonSystem-$$VTK_VERSION_SUFFIX \
   -lvtkCommonMath-$$VTK_VERSION_SUFFIX -lvtkCommonTransforms-$$VTK_VERSION_SUFFIX -lvtkCommonColor-$$VTK_VERSION_SUFFIX -lvtkFiltersGeneral-$$VTK_VERSION_SUFFIX

QMAKE_LFLAGS_PLUGIN += -Wl,-install_name,@rpath/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
}

unix:!macx{
    LIBS += -lAwCore -lAwLayout $$VTK_LIBRARIES
}

SOURCES += \
    AwMap.cpp \
    AwMeshManager.cpp

HEADERS += \
    ../../include/mapping/AwMap.h \
    ../../include/mapping/AwMeshManager.h

macx {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}


