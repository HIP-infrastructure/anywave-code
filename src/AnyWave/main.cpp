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
#if VTK_MAJOR_VERSION >= 7
#include <QVTKOpenGLWidget.h>
#include <vtkOpenGLRenderWindow.h>
#endif

#include <qstylefactory.h>
#include <qtextstream.h>
#include "CommandLineParser.h"

int main(int argc, char *argv[])
{
#if VTK_MAJOR_VERSION > 7
	// init surface map for further use in VTK 8.1
	vtkOpenGLRenderWindow::SetGlobalMaximumNumberOfMultiSamples(0);
	QSurfaceFormat::setDefaultFormat(QVTKOpenGLWidget::defaultFormat());
#endif

	QApplication app(argc, argv);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#ifdef Q_OS_WIN
	QApplication::setStyle(QStyleFactory::create("windowsvista"));
#endif
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

#ifdef _WIN32  // On Windows, add the subdir Plugins to the dll search path.
	QString appPath = QApplication::applicationDirPath() + "/Plugins";
	wchar_t *dllDir = new wchar_t[appPath.size() + 1];
	appPath.toWCharArray(dllDir);
	dllDir[appPath.size()] = '\0';
	for (int i = 0; i < appPath.size(); i++)	{
		if (dllDir[i] == '/') 
			dllDir[i] = '\\';
	}
	bool res = SetDllDirectory((LPCWSTR)dllDir);
#endif
	
	// check if at least one argument 

	// check if arguments 
	auto args = aw::commandLine::doParsing();
	AnyWave window(args.isEmpty()); // args not empty means something to do in command line mode => no gui mode on 
	int status = aw::commandLine::doCommandLineOperations(args);
	if (status == 0) 
		exit(0);

	QCommandLineParser parser;
	parser.addPositionalArgument("file", "data file to open");
	parser.process(app);
	auto positionalArgs = parser.positionalArguments();

	if (positionalArgs.count() == 1)
		window.openFile(positionalArgs.at(0));
		
	window.showMaximized();
	return app.exec();
}
