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
#endif

#include <qcommandlineparser.h>
#include "AwException.h"
#include "IO/BIDS/AwBIDSManager.h"
#include <qstylefactory.h>
#include <qtextstream.h>

int main(int argc, char *argv[])
{
#if VTK_MAJOR_VERSION >= 7
	// init surface map for further use in VTK 8.1
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
	
	QCommandLineParser parser;
	parser.setApplicationDescription("AnyWave");
	parser.addVersionOption();
	parser.addHelpOption();
	parser.addPositionalArgument("file", "The file to open.");
	QCommandLineOption seegBIDSOpt("seegBIDS", "SEEG file to BIDSify.\nRequires task and sub options.", "file", QString());
	QCommandLineOption BIDSSidecarsOpt("bids_output", "sidecars : Only generates sidecar files. all : full conversion", "sidecars", QString());
	QCommandLineOption BIDSTaskOpt("bids_task", "BIDS task", "task", QString());
	QCommandLineOption BIDSSubjectOpt("bids_sub", "BIDS subject", "subject", QString());
	QCommandLineOption BIDSSessionOpt("bids_ses", "BIDS session", "session", QString());
	QCommandLineOption BIDSRunOpt("bids_run", "BIDS run", "run", QString());
	QCommandLineOption BIDSDestOpt("bids_dir", "BIDS destination folder", "dir", QString());
	QCommandLineOption BIDSFormatOpt("bids_format", "data format for output EDF (default) or VHDR", "format", QString());
	QCommandLineOption BIDSAcqOpt("bids_acq", "acquisition method", "acq", QString());
	QCommandLineOption BIDSProcOpt("bids_proc", "proc", "proc", QString());

	parser.addOption(seegBIDSOpt);
	parser.addOption(BIDSSidecarsOpt);
	parser.addOption(BIDSTaskOpt);
	parser.addOption(BIDSSubjectOpt);
	parser.addOption(BIDSSessionOpt);
	parser.addOption(BIDSRunOpt);
	parser.addOption(BIDSDestOpt);
	parser.addOption(BIDSFormatOpt);
	parser.addOption(BIDSAcqOpt);
	parser.addOption(BIDSProcOpt);
	parser.process(QCoreApplication::arguments());
	QStringList args = parser.positionalArguments();

	AnyWave window;
	if (parser.isSet(seegBIDSOpt)) {
		if (!parser.isSet(BIDSTaskOpt) || !parser.isSet(BIDSSubjectOpt)) {
			parser.showHelp();
			exit(-1);
		}
		// Session option is not required
		QString file = parser.value(seegBIDSOpt);
		QString subj = parser.value(BIDSSubjectOpt);
		QString task = parser.value(BIDSTaskOpt);
		QString session = parser.value(BIDSSessionOpt);
		QString run = parser.value(BIDSRunOpt);
		QString dir = parser.value(BIDSDestOpt);
		QString format = parser.value(BIDSFormatOpt);
		QString output = parser.value(BIDSSidecarsOpt);
		QString acq = parser.value(BIDSAcqOpt);
		QString proc = parser.value(BIDSProcOpt);

		if (file.isEmpty() || subj.isEmpty() || task.isEmpty()) {
			parser.showHelp();
			exit(-1);
		}

		if (output.toUpper() != "SIDECARS") {
			if (format.isEmpty())
				format = "EDF";
			bool formatOK = format.toUpper() == "EDF" || format.toUpper() == "VHDR";
			if (!formatOK) {
				parser.showHelp();
				exit(-1);
			}
		}
		
		QList<AwArgument> arguments;
		// first argument must be the kind of file to convert (here SEEG)
		arguments << AwArgument("SEEGFile", file);
		// subject is mandatory and should be the second argument.
		arguments << AwArgument("subject", subj);
		if (!task.isEmpty())
			arguments << AwArgument("task", task);
		if (!session.isEmpty())
			arguments << AwArgument("session", session);
		if (!run.isEmpty())
			arguments << AwArgument("run", run);
		if (!dir.isEmpty())
			arguments << AwArgument("dir", dir);
		if (!format.isEmpty())
			arguments << AwArgument("format", format);
		if (!output.isEmpty())
			arguments << AwArgument("output", output);
		if (!acq.isEmpty())
			arguments << AwArgument("acq", acq);
		if (!proc.isEmpty())
			arguments << AwArgument("proc", proc);

		try {
			AwBIDSManager::instance()->toBIDS(arguments);

		}
		catch (const AwException& e)
		{
			exit(-1);
		}
		exit(0);
	}

	if (args.count() == 1)
		window.openFile(args.at(0));
		
	window.showMaximized();
	return app.exec();
}
