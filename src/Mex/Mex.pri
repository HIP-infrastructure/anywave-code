#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------

include(../common.pri)
QT       -= gui
QT += network
TEMPLATE = lib
CONFIG += plugin no_plugin_name_prefix
DESTDIR = $$PLUGIN_DIR/MATLAB/AnyWave
macx{
QMAKE_EXTENSION_SHLIB = mexmaci64
}
unix:!macx{
QMAKE_EXTENSION_SHLIB = mexa64
}
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$MATLAB_ROOT/extern/include
macx{
LIBS += -L$$MATLAB_ROOT/bin/maci64 -lmex -lmx -lmat
QMAKE_RPATHDIR += /Library/Frameworks

}
unix:!macx{
LIBS += -L$$MATLAB_ROOT/bin/glnxa64 -lmex -lmx -lmat
QMAKE_RPATHDIR += /usr/local/AnyWave/lib
}
DEFINES += MATLAB_MEX_FILE
DEF_FILE = configure.def
RC_FILE = /Applications/MATLAB_R2015a.app/extern/include/mexversion.rc
macx{
LIBS += -framework AwCore
}
unix:!macx{
LIBS += -lAwCore
}
