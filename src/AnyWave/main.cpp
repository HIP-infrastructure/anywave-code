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

#include "CL/CommandLineParser.h"
#include "AwComponents.h"
#include <iostream>

constexpr auto version = "21.04.14";

int main(int argc, char *argv[])
{
#ifdef _WIN32
	if (AttachConsole(ATTACH_PARENT_PROCESS)) {
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}
#endif
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

#if defined(Q_OS_MAC)
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
	QApplication app(argc, argv);
	QVariantMap arguments;

#ifndef Q_OS_WIN
	Q_INIT_RESOURCE(layouts);
    Q_INIT_RESOURCE(amplitudes);
#endif
	QCoreApplication::setOrganizationName("INSERM U1106");
	QCoreApplication::setOrganizationDomain("INS.org");
	QCoreApplication::setApplicationName("AnyWave");
	app.setApplicationVersion(version);

	QSettings settings(QSettings::SystemScope, "INSERM U1106", "AnyWave");
	settings.setValue("general/secureMode", false);
	settings.setValue("general/buildDate", QString(__DATE__));
	AwComponents components;
	if (components.init() != 0) {
		std::cerr << "Error while initialising AnyWave components" << std::endl;
		return -1;
	}
	if (argc > 1) {
		int operation = aw::commandLine::doLowLevelParsing(app.arguments(), arguments);
		if (operation == aw::commandLine::NoOperation)
			return 0;
		if (operation == -1)
			return -1;

		try {
			operation = aw::commandLine::doParsing(app.arguments(), arguments);
		}
		catch (const AwException& e) {
			std::cerr << e.errorString().toStdString() << std::endl;
			//		quit();
			//		exit(0);
			return -1;
		}
		if (operation == aw::commandLine::NoOperation)
			return 0;
		if (operation == aw::commandLine::BatchOperation) {
			aw::commandLine::doCommandLineOperation(arguments);
			return 0;
		}
	}

	// command line arguments parsing is done while building anywave.
	AnyWave window(app.arguments());
	return app.exec();
}
