#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------

TEMPLATE = app
macx {
    TARGET = AnyWaveMac
}
unix:!macx {
    TARGET = AnyWaveLinux
}

include(../common.pri)
include(AnyWave.pri)
QT += widgets xml svg concurrent qml printsupport charts
CONFIG += qwt
macx {
CONFIG -= app_bundle
}

DEFINES += AW_DISABLE_EPOCHING

macx {
    DESTDIR = $$DESTIR/AnyWave.app/Contents/MacOS
}

macx {
    LIBS += -framework AwCore -framework Accelerate
}

unix:!macx{
 LIBS += -lAwCore -lopenblas -lgomp
}

LIBS += -lAwUtilities -lAwMath -lAwFilter -lAwLayout -lAwMapping -lAwGraphics -lAwHDF5 -lAwMATLAB -lAwWidgets -lAwEpoch
LIBS += -lAwRW -lAwProcess -lhdf5 -lhdf5_hl -lqwt
LIBS += -lvtkCommonCore$$VTK_VERSION_SUFFIX -lvtkCommonDataModel$$VTK_VERSION_SUFFIX -lvtkCommonExecutionModel$$VTK_VERSION_SUFFIX \ 
-lvtkCommonMisc$$VTK_VERSION_SUFFIX -lvtkFiltersCore$$VTK_VERSION_SUFFIX -lvtkGUISupportQt$$VTK_VERSION_SUFFIX \
-lvtkFiltersGeometry$$VTK_VERSION_SUFFIX -lvtkIOGeometry$$VTK_VERSION_SUFFIX \
-lvtkRenderingCore$$VTK_VERSION_SUFFIX -lvtkInteractionStyle$$VTK_VERSION_SUFFIX -lvtkCommonExecutionModel$$VTK_VERSION_SUFFIX \
-lvtkFiltersSources$$VTK_VERSION_SUFFIX -lvtkRenderingOpenGL2$$VTK_VERSION_SUFFIX -lvtkRenderingFreeType$$VTK_VERSION_SUFFIX \ 
-lvtkRenderingAnnotation$$VTK_VERSION_SUFFIX -lvtkIOCore$$VTK_VERSION_SUFFIX -lvtkIOXML$$VTK_VERSION_SUFFIX \ 
-lvtkIOXMLParser$$VTK_VERSION_SUFFIX -lvtksys$$VTK_VERSION_SUFFIX -lvtkglew$$VTK_VERSION_SUFFIX \
-lvtkCommonTransforms$$VTK_VERSION_SUFFIX -lvtkCommonColor$$VTK_VERSION_SUFFIX \
-lvtkCommonSystem$$VTK_VERSION_SUFFIX -lvtkCommonMath$$VTK_VERSION_SUFFIX -lvtkCommonComputationalGeometry$$VTK_VERSION_SUFFIX \
-lvtkFiltersGeneral$$VTK_VERSION_SUFFIX -lvtkFiltersModeling$$VTK_VERSION_SUFFIX -lvtkFiltersExtraction$$VTK_VERSION_SUFFIX \
-lvtkFiltersStatistics$$VTK_VERSION_SUFFIX -lvtkImagingFourier$$VTK_VERSION_SUFFIX \
-lvtkImagingCore$$VTK_VERSION_SUFFIX -lvtkalglib$$VTK_VERSION_SUFFIX

install_sh.path = $$DESTDIR
install_sh.files += ../../resources/anywave.sh
INSTALLS += install_sh


DISTFILES += \
    anywave_en.qm \
    anywave_fr.qm \
    anywave_en.ts \
    anywave_fr.ts
