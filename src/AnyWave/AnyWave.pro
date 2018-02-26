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
    DESTDIR = $$AW_OUT_DIR/bin/AnyWave.app/Contents/MacOS
}
unix:!macx {
    DESTDIR = $$AW_OUT_DIR/bin
}

macx {
    LIBS += -framework AwCore -framework Accelerate
}

unix:!macx{
 LIBS += -lAwCore -lopenblas -lgomp
}

LIBS += -lAwUtilities -lAwMath -lAwFilter -lAwLayout -lAwMapping -lAwGraphics -lAwHDF5 -lAwMATLAB -lAwWidgets -lAwEpoch
LIBS += -lAwRW -lAwProcess -lhdf5 -lhdf5_hl
LIBS += -lvtkCommonCore-8.1 -lvtkCommonDataModel-$$VTK_VERSION -lvtkCommonExecutionModel-$$VTK_VERSION -lvtkCommonMisc-$$VTK_VERSION \
-lvtkFiltersCore-$$VTK_VERSION -lvtkGUISupportQt-$$VTK_VERSION -lvtkFiltersGeometry-$$VTK_VERSION -lvtkIOGeometry-$$VTK_VERSION \
-lvtkRenderingCore-$$VTK_VERSION -lvtkInteractionStyle-$$VTK_VERSION -lvtkCommonExecutionModel-$$VTK_VERSION \
-lvtkFiltersSources-$$VTK_VERSION -lvtkRenderingOpenGL2-$$VTK_VERSION -lvtkRenderingFreeType-$$VTK_VERSION -lvtkRenderingAnnotation-$$VTK_VERSION  \
-lvtkIOCore-$$VTK_VERSION -lvtkIOXML-$$VTK_VERSION -lvtkIOXMLParser-$$VTK_VERSION -lvtksys-$$VTK_VERSION -lvtkglew-$$VTK_VERSION -lvtkzlib-$$VTK_VERSION \
-lvtkfreetype-$$VTK_VERSION -lvtkCommonTransforms-$$VTK_VERSION -lvtkCommonColor-$$VTK_VERSION -lvtkexpat-$$VTK_VERSION -lvtklz4-$$VTK_VERSION \
-lvtkCommonSystem-$$VTK_VERSION -lvtkCommonMath-$$VTK_VERSION -lvtkCommonComputationalGeometry-$$VTK_VERSION -lvtkFiltersGeneral-$$VTK_VERSION \
-lvtkFiltersModeling-$$VTK_VERSION -lvtkFiltersExtraction-$$VTK_VERSION -lvtkFiltersStatistics-$$VTK_VERSION -lvtkImagingFourier-$$VTK_VERSION \
-lvtkImagingCore-$$VTK_VERSION -lvtkalglib-$$VTK_VERSION



DISTFILES += \
    anywave_en.qm \
    anywave_fr.qm \
    anywave_en.ts \
    anywave_fr.ts
