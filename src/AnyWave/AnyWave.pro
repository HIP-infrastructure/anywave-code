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
    LIBS += -lvtkCommonCore-$$VTK_VERSION_SUFFIX -lvtkCommonDataModel-$$VTK_VERSION_SUFFIX -lvtkCommonMisc-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersCore-$$VTK_VERSION_SUFFIX -lvtkGUISupportQt-$$VTK_VERSION_SUFFIX -lvtkFiltersGeometry-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersModeling-$$VTK_VERSION_SUFFIX -lvtkRenderingCore-$$VTK_VERSION_SUFFIX -lvtkInteractionStyle-$$VTK_VERSION_SUFFIX -lvtkCommonExecutionModel-$$VTK_VERSION_SUFFIX \
    -lvtkFiltersSources-$$VTK_VERSION_SUFFIX -lvtkRenderingOpenGL2-$$VTK_VERSION_SUFFIX -lvtkRenderingFreeType-$$VTK_VERSION_SUFFIX -lvtkRenderingAnnotation-$$VTK_VERSION_SUFFIX  \
    -lvtkIOCore-$$VTK_VERSION_SUFFIX -lvtkIOXML-$$VTK_VERSION_SUFFIX

   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/libvtkCommonCore-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkCommonDataModel-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkCommonMisc-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkFiltersCore-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkGUISupportQt-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkFiltersGeometry-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkFiltersModeling-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkRenderingCore-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkInteractionStyle-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkCommonExecutionModel-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkFiltersSources-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkRenderingOpenGL2-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkRenderingFreeType-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkRenderingAnnotation-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkIOCore-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}
   # QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/vtkIOXML-$$VTK_VERSION_SUFFIX.$${QMAKE_EXTENSION_SHLIB}

  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwLayout.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwUtilities.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwFilter.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwMapping.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwGraphics.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwMATLAB.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwWidgets.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwEpoch.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwProcess.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwMath.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwRW.$${QMAKE_EXTENSION_SHLIB}
  #  QMAKE_LFLAGS_SONAME += -Wl,-install_name,@executable_path/../Frameworks/AwHDF5.$${QMAKE_EXTENSION_SHLIB}

#QMAKE_LFLAGS_SONAME = -Wl,-rpath @executable_path/../Frameworks/
QMAKE_RPATHDIR += @executable_path/../../../Anywave_plugins
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
