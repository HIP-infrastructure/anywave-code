// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include <AwVersion.h>

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
 //   Q_INIT_RESOURCE(amplitudes);
#endif
	QCoreApplication::setOrganizationName("INSERM U1106");
	QCoreApplication::setOrganizationDomain("INS.org");
	QCoreApplication::setApplicationName("AnyWave");
	app.setApplicationVersion(QString("%1.%2").arg(AW_MAJOR_VERSION).arg(AW_MINOR_VERSION));

	QSettings settings(QSettings::SystemScope, "INSERM U1106", "AnyWave");
	settings.setValue("general/secureMode", false);
	settings.setValue("general/buildDate", QString(__DATE__));
	AwComponents components;
	if (components.init() != 0) {
		std::cout << "Error while initializing AnyWave components";
		return -1;
	}
	if (argc > 1) {
		components.setGuiEnabled(false);
		int operation = -1;

		try {
			operation = aw::commandLine::doParsing(app.arguments(), arguments);
		}
		catch (const AwException& e) {
			std::cout << e.errorString().toStdString();
			return -1;
		}
		if (operation == aw::commandLine::NoOperation)  // if parsing returns NoOperation that means that nothing more should be processed neither the GUI should be
														// launched.
			return 0;
		if (operation == aw::commandLine::BatchOperation) {
			aw::commandLine::doCommandLineOperation(arguments);
			return 0;
		}
	}
	components.setGuiEnabled(true);  // if we get there, launch GUI even after some command line options have been processed (mostly -plugin_debug and -server_port)
	AnyWave window(arguments);
	return app.exec();
}
