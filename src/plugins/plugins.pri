#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T17:00:19
#
#-------------------------------------------------
include(../common.pri)
macx {
    LIBS += -framework AwCore
    QMAKE_RPATHDIR += /Library/Frameworks
    QMAKE_RPATHDIR += @executable_path/../Frameworks
}

unix:!macx{
    LIBS += -lAwCore
    QMAKE_RPATHDIR += $ORIGIN/lib
}
