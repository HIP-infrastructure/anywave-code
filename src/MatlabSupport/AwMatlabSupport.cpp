/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwMatlabSupport.h"
#include <QtPlugin>
#include <QDir>
#include <QSettings>
#include <QtCore>


constexpr auto AW_MATLAB_OUTPUT_BUFFER_SIZE = 1024 * 20;	// 20Kbytes buffer

/** run()
- path: path to the script to be executed.
- dep: path to dependency folder to add to the path.
- pid: process id of the script.
**/
void AwMatlabSupport::run(const QString& path, const QString& dep, int pid, quint16 serverPort, const QString& jsonArgs)
{
	m_eng = engOpen(NULL); 
	emit progressChanged("Opening MATLAB Connection...");
	if (m_eng != NULL) {
		emit progressChanged("Connection to MATLAB established.");
		char *buffer = new char[AW_MATLAB_OUTPUT_BUFFER_SIZE + 1];
		buffer[AW_MATLAB_OUTPUT_BUFFER_SIZE] = '\0';
		QString command;
		QString envPath;
		
		// set global variables host, pid, port and json_args
		engEvalString(m_eng, "evalin('base', 'global host')");
		engEvalString(m_eng, "evalin('base', 'global pid')");
		engEvalString(m_eng, "evalin('base', 'global port')");
		engEvalString(m_eng, "evalin('base', 'global args')");
		// send pid
		command = "evalin('base', 'pid = " + QString::number(pid) + "')";
		engEvalString(m_eng, command.toLatin1().data());
		// send port
		command = QString("evalin('base', 'port = %1')").arg(serverPort);
		engEvalString(m_eng, command.toLatin1().data());
		// send host
		engEvalString(m_eng, "evalin('base', 'host = ''127.0.0.1''')");

		if (!jsonArgs.isEmpty())
			// send json args
			command = "evalin('base', 'args =  jsondecode(''" + jsonArgs.simplified() + "'')')";
		else
			command = QString("evalin('base', 'args = ''''''')");

		engEvalString(m_eng, command.toLatin1().data());
	
		engOutputBuffer(m_eng, buffer, AW_MATLAB_OUTPUT_BUFFER_SIZE);

		// add path to AnyWave parent folder
		QDir awPath(QCoreApplication::applicationDirPath());

		// add dependencies paths
		command = QString("addpath(genpath('%1'))").arg(dep);
		engEvalString(m_eng, command.toLatin1().data()); 
#ifdef Q_OS_WIN
	//	command = "addpath('" + awPath.absolutePath() + "')";
		//envPath = "setenv('PATH', '" + awPath.absolutePath() + "')";
		//engEvalString(m_eng, envPath.toLatin1().data()); // setenv('PATH', 'Path to AnyWave directory');
		//engEvalString(m_eng, command.toLatin1().data()); 
        awPath.cd("Plugins/MATLAB/AnyWave");
#endif
#ifdef Q_OS_MAC
		awPath.cdUp();
		//QString dyld = QString("setenv('DYLD_LIBRARY_PATH', '%1/dylibs')").arg(awPath.absolutePath());
	//	engEvalString(m_eng, dyld.toLatin1().data()); 
	//	awPath.cdUp();
	//	awPath.cdUp();
		awPath.cd("Plugins/MATLAB/AnyWave");
#endif
#ifdef Q_OS_LINUX
        //QString ld = QString("setenv('LD_LIBRARY_PATH', '/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:%1/lib')").arg(awPath.absolutePath());
        QByteArray ld = qgetenv("LD_LIBRARY_PATH");
        QString setld = QString("setenv('%1')").arg(QString(ld));
        engEvalString(m_eng, setld.toLatin1().data());
        awPath.cd("Plugins/MATLAB/AnyWave");
#endif
		command = "addpath('" + awPath.absolutePath() + "')";
		engEvalString(m_eng, command.toLatin1().data());
		// add a variable named aw_pid which is the index of the process in AwMatlabServer
		command = "cd " + path;

		engEvalString(m_eng, command.toLatin1().data());
		// launch main function with aw_pid parameter
		// the function is evaluated by the 'base' workspace (required for GUI intensive applications)
		//// search for main.m or main.mlapp
		//if (QFile::exists(QString("%1/main.m").arg(path)))
			command = "evalin('base','main')";
		engEvalString(m_eng, command.toLatin1().data());
		emit progressChanged(QString(buffer));
		engClose(m_eng);
		delete[] buffer;
	}
	else
		emit progressChanged(tr("Failed to connect to MATLAB!"));
}
