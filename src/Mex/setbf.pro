#-------------------------------------------------
#
# Project created by QtCreator 2018-01-23T14:33:21
#
#-------------------------------------------------

include(Mex.pri)
TARGET = aw_setbf
SOURCES += \
      aw_setbf.cpp
LIBS += -lcommon





macx {
QMAKE_POST_LINK = \
  install_name_tool -change AwCore.framework/Versions/1/AwCore @rpath/AwCore.framework/Versions/1/AwCore $${DESTDIR}/$${TARGET}.$${QMAKE_EXTENSION_SHLIB}
}
