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
    DESTDIR = $$DESTDIR/bin/AnyWave.app/Contents/MacOS
}
unix:!macx {
    DESTDIR = $$DESTDIR
}

macx {
    LIBS += -framework AwCore -framework Accelerate
    LIBS += -lAwLayout -lvtkCommonCore-$$VTK_VERSION_SUFFIX -lvtkCommonDataModel-$$VTK_VERSION_SUFFIX -lvtkCommonMisc-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersCore-$$VTK_VERSION_SUFFIX -lvtkGUISupportQt-$$VTK_VERSION_SUFFIX -lvtkFiltersGeometry-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersModeling-$$VTK_VERSION_SUFFIX -lvtkRenderingCore-$$VTK_VERSION_SUFFIX -lvtkInteractionStyle-$$VTK_VERSION_SUFFIX -lvtkCommonExecutionModel-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersSources-$$VTK_VERSION_SUFFIX -lvtkRenderingOpenGL2-$$VTK_VERSION_SUFFIX -lvtkRenderingFreeType-$$VTK_VERSION_SUFFIX -lvtkRenderingAnnotation-$$VTK_VERSION_SUFFIX  \
    -lvtkIOCore-$$VTK_VERSION_SUFFIX -lvtkIOXML-$$VTK_VERSION_SUFFIX
}

unix:!macx{
 LIBS += -lAwCore -lopenblas -lgomp -lqwt  $$VTK_LIBRARIES
}

LIBS += -lAwUtilities -lAwMath -lAwFilter -lAwLayout -lAwMapping -lAwGraphics -lAwHDF5 -lAwMATLAB -lAwWidgets -lAwEpoch
LIBS += -lAwRW -lAwProcess -lhdf5 -lhdf5_hl -lmatio

install_extra.path = $$DESTDIR

macx{
install_extra.files += ../../resources/macx/Anywave.sh 
}
unix:!macx{
install_extra.files += ../../resources/linux/anywave.sh 
}

INSTALLS += install_extra

install_headers.path = $$HEADER_DIR
install_headers.files += $$PWD/../../include

INSTALLS += install_headers

install_arma.path = $$HEADER_DIR
install_arma.files += $$PWD/../../armadillo

INSTALLS += install_arma

DISTFILES += \
    anywave_en.qm \
    anywave_fr.qm \
    anywave_en.ts \
    anywave_fr.ts
