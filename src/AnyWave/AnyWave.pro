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
    DESTDIR = $$DESTDIR/AnyWave.app/Contents/MacOS
}

macx {
    LIBS += -framework AwCore -framework Accelerate
}

unix:!macx{
 LIBS += -lAwCore -lopenblas -lgomp -lqwt
}

LIBS += -lAwUtilities -lAwMath -lAwFilter -lAwLayout -lAwMapping -lAwGraphics -lAwHDF5 -lAwMATLAB -lAwWidgets -lAwEpoch
LIBS += -lAwRW -lAwProcess -lhdf5 -lhdf5_hl  $$VTK_LIBRARIES -lmatio

install_extra.path = $$DESTDIR
macx{
install_extra.files += ../../resources/macx/Anywave.sh 
}
unix:!macx{
install_extra.files += ../../resources/linux/anywave.sh 
}

INSTALLS += install_extra


DISTFILES += \
    anywave_en.qm \
    anywave_fr.qm \
    anywave_en.ts \
    anywave_fr.ts
