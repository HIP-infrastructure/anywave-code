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

#define AW_MATLAB_OUTPUT_BUFFER_SIZE	1024 * 20	// 20Kbytes buffer

/** run()
- path: path to the script to be executed.
- dep: path to dependency folder to add to the path.
- pid: process id of the script.
**/
void AwMatlabSupport::run(const QString& path, const QString& dep, int pid)
{
	m_eng = engOpen(NULL);
	emit progressChanged("Opening MATLAB Connection...");
	if (m_eng != NULL) {
		emit progressChanged("Connection to MATLAB established.");
		char *buffer = new char[AW_MATLAB_OUTPUT_BUFFER_SIZE + 1];
		buffer[AW_MATLAB_OUTPUT_BUFFER_SIZE] = '\0';
		QString command;
		QString envPath;
		
		// set global variables host and pid
		engEvalString(m_eng, "evalin('base', 'global host')");
		engEvalString(m_eng, "evalin('base', 'global pid')");
		engEvalString(m_eng, "evalin('base', 'global port')");
		// send pid
		command = "evalin('base', 'pid = " + QString::number(pid) + "')";
		engEvalString(m_eng, command.toLatin1().data());
		// send port
		command = "evalin('base', 'port = 50222')";
		engEvalString(m_eng, command.toLatin1().data());
		// send host
		engEvalString(m_eng, "evalin('base', 'host = ''127.0.0.1''')");
		engOutputBuffer(m_eng, buffer, AW_MATLAB_OUTPUT_BUFFER_SIZE);

		// add path to AnyWave parent folder
		QDir awPath(QCoreApplication::applicationDirPath());

		// add dependencies paths
		command = QString("addpath(genpath('%1'))").arg(dep);
		engEvalString(m_eng, command.toLatin1().data()); 
#ifdef Q_OS_WIN
		command = "addpath('" + awPath.absolutePath() + "')";
		envPath = "setenv('PATH', '" + awPath.absolutePath() + "')";
		engEvalString(m_eng, envPath.toLatin1().data()); // setenv('PATH', 'Path to AnyWave directory');
		engEvalString(m_eng, command.toLatin1().data()); 
#endif
#ifdef Q_OS_MAC
		awPath.cdUp();
		awPath.cdUp();
		awPath.cdUp();
		awPath.cd("Anywave_Plugins/Matlab/AnyWave");
#else
		awPath.cd("Plugins/Matlab/AnyWave");
#endif
		command = "addpath('" + awPath.absolutePath() + "')";
		engEvalString(m_eng, command.toLatin1().data());
		// add a variable named aw_pid which is the index of the process in AwMatlabServer
		command = "cd " + path;
		engEvalString(m_eng, command.toLatin1().data());
		// launch main function with aw_pid parameter
		// the function is evaluated by the 'base' workspace (required for GUI intensive applications)
		command = "evalin('base','main')";
		engEvalString(m_eng, command.toLatin1().data());
		emit progressChanged(QString(buffer));
		engClose(m_eng);
		delete[] buffer;
	}
	else
		emit progressChanged(tr("Failed to connect to MATLAB!"));
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(AwMatlabSupport, AwMatlabSupport)
#endif
