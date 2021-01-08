/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <QApplication>
#include "AnyWave.h"
#include <AwException.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include <QString>
#include <QSettings>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include <QGuiApplication>
#include <QScreen>
#endif

#include <vtkVersion.h>
#if VTK_MAJOR_VERSION >= 8
#include <QVTKOpenGLWidget.h>
#include <vtkOpenGLRenderWindow.h>
#include <QSurfaceFormat>
#endif

//#include <qstylefactory.h>
//#include <qtextstream.h>
#include "CL/CommandLineParser.h"


int main(int argc, char *argv[])
{
#if VTK_MAJOR_VERSION >= 8
	// init surface map for further use in VTK 8.1
	vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
#endif
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
//#if defined(Q_OS_MAC)
	QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
//#endif
#endif
//#if defined(Q_OS_WIN)
//#include <shellscalingapi.h>
////	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
//	SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);
//#endif
#if defined(Q_OS_MAC)
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
	QApplication app(argc, argv);
#ifndef Q_OS_WIN
	Q_INIT_RESOURCE(layouts);
    Q_INIT_RESOURCE(amplitudes);
#endif
	QCoreApplication::setOrganizationName("INSERM U1106");
	QCoreApplication::setOrganizationDomain("INS.org");
	QCoreApplication::setApplicationName("AnyWave");

	QSettings settings(QSettings::SystemScope, "INSERM U1106", "AnyWave");
	settings.setValue("general/secureMode", false);
	settings.setValue("general/buildDate", QString(__DATE__));

	// check if arguments
	AnyWave window(app.arguments());
	return app.exec();
}
