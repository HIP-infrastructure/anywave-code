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
    LIBS += -framework AwCore -framework qwt -framework AppKit
    QMAKE_LFLAGS_PLUGIN += -Wl,-install_name,@rpath/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
   LIBS += -lvtkGUISupportQt-$$VTK_VERSION_SUFFIX -lvtkFiltersCore-$$VTK_VERSION_SUFFIX -lvtkCommonMisc-$$VTK_VERSION_SUFFIX \
   -lvtkRenderingCore-$$VTK_VERSION_SUFFIX -lvtkIOGeometry-$$VTK_VERSION_SUFFIX -lvtkCommonExecutionModel-$$VTK_VERSION_SUFFIX \
    -lvtkRenderingFreeType-$$VTK_VERSION_SUFFIX -lvtkFiltersSources-$$VTK_VERSION_SUFFIX -lvtkRenderingAnnotation-$$VTK_VERSION_SUFFIX \
    -lvtkCommonCore-$$VTK_VERSION_SUFFIX -lvtkCommonDataModel-$$VTK_VERSION_SUFFIX -lvtkIOCore-$$VTK_VERSION_SUFFIX \
    -lvtkRenderingOpenGL2-$$VTK_VERSION_SUFFIX

   QMAKE_POST_LINK = \
    install_name_tool -change libAwGraphics.dylib  @rpath/libAwGraphics.dylib $$DESTDIR/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB} \
    install_name_tool -change libAwUtilities.dylib  @rpath/libAwUtilities.dylib $$DESTDIR/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB} \
    install_name_tool -change libAwMath.dylib  @rpath/libAwMath.dylib $$DESTDIR/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB} \
    install_name_tool -change libAwLayout.dylib  @rpath/libAwLayout.dylib $$DESTDIR/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB} \
    install_name_tool -change libAwMapping.dylib  @rpath/libAwMapping.dylib $$DESTDIR/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB} \
    install_name_tool -change AwCore.framework/Versions/1/AwCore  @rpath/AwCore.framework/Versions/1/AwCore $$DESTDIR/lib$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
}


unix:!macx{
 LIBS += -lAwCore $$VTK_LIBRARIES
}

LIBS += -lAwGraphics -lAwUtilities -lAwMath -lAwLayout -lAwMapping

unix {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}
