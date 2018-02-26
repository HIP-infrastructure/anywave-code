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
LIBS += -lAwGraphics -lAwUtilities -lAwMath -lAwLayout -lAwMapping  -lvtkCommonCore-$$VTK_VERSION -lvtkCommonDataModel-$$VTK_VERSION -lvtkCommonMisc-$$VTK_VERSION \
-lvtkFiltersCore-$$VTK_VERSION -lvtkGUISupportQt-$$VTK_VERSION -lvtkFiltersGeometry-$$VTK_VERSION -lvtkIOGeometry-$$VTK_VERSION \
-lvtkRenderingCore-$$VTK_VERSION -lvtkInteractionStyle-$$VTK_VERSION -lvtkCommonExecutionModel-$$VTK_VERSION \
-lvtkFiltersSources-$$VTK_VERSION -lvtkRenderingOpenGL2-$$VTK_VERSION -lvtkRenderingFreeType-$$VTK_VERSION -lvtkRenderingAnnotation-$$VTK_VERSION  \
-lvtkIOCore-$$VTK_VERSION -lvtkIOXML-$$VTK_VERSION
}

unix:!macx{
 LIBS += -lAwCore
 LIBS += -lAwGraphics -lAwUtilities -lAwMath -lAwLayout -lAwMapping
}





unix {
    target.path = $$INSTALL_LIB_PATH
    INSTALLS += target
}
