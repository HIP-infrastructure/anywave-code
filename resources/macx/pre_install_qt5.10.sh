#!/bin/sh

#  pres_install_qt5.sh
#  
#
#  Created by bruno colombet on 11/04/2016.
#
if [[ -d /Library/Frameworks/QtConcurrent.framework ]]; then
   rm -rf /Library/Frameworks/QtConcurrent.framework
fi
if [[ -d /Library/Frameworks/QtCore.framework ]]; then
    rm -rf /Library/Frameworks/QtCore.framework
fi
if [[ -d /Library/Frameworks/QtDBus.framework ]]; then
    rm -rf /Library/Frameworks/QtDBus.framework
fi
if [[ -d /Library/Frameworks/QtGui.framework ]]; then
    rm -rf /Library/Frameworks/QtGui.framework
fi
if [[ -d /Library/Frameworks/QtNetwork.framework ]]; then
    rm -rf /Library/Frameworks/QtNetwork.framework
fi
if [[ -d /Library/Frameworks/QtOpenGL.framework ]]; then
    rm -rf /Library/Frameworks/QtOpenGL.framework
fi
if [[ -d /Library/Frameworks/QtPrintSupport.framework ]]; then
    rm -rf /Library/Frameworks/QtPrintSupport.framework
fi
if [[ -d /Library/Frameworks/QtQml.framework ]]; then
rm -rf /Library/Frameworks/QtQml.framework
fi
if [[ -d /Library/Frameworks/QtCharts.framework ]]; then
rm -rf /Library/Frameworks/QtCharts.framework
fi
if [[ -d /Library/Frameworks/QtSql.framework ]]; then
    rm -rf /Library/Frameworks/QtSql.framework
fi
if [[ -d /Library/Frameworks/QtSvg.framework ]]; then
    rm -rf /Library/Frameworks/QtSvg.framework
fi
if [[ -d /Library/Frameworks/QtWidgets.framework ]]; then
    rm -rf /Library/Frameworks/QtWidgets.framework
fi
if [[ -d /Library/Frameworks/qwt.framework ]]; then
    rm -rf /Library/Frameworks/qwt.framework
fi
if [[ -d /Library/Frameworks/QtXml.framework ]]; then
    rm -rf /Library/Frameworks/QtXml.framework
fi
