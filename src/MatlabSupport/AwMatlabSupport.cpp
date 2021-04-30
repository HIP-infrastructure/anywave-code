/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
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
void AwMatlabSupport::run(const QVariantMap& settings)
{
	m_eng = engOpen(nullptr); 
	emit progressChanged("Opening MATLAB Connection...");
	if (m_eng != nullptr) {
		emit progressChanged("Connection to MATLAB established.");
		char *buffer = new char[AW_MATLAB_OUTPUT_BUFFER_SIZE + 1];
		buffer[AW_MATLAB_OUTPUT_BUFFER_SIZE] = '\0';
		QString command;
		QString envPath;
		std::string tmp;
		quint16 port = static_cast<quint16>(settings.value(matlab_interface::port).toUInt());
		int pid = settings.value(matlab_interface::pid).toInt();
		QString args = settings.value(matlab_interface::json).toString();
		
		// set global variables host, pid, port and json_args
		engEvalString(m_eng, "evalin('base', 'global host')");
		engEvalString(m_eng, "evalin('base', 'global pid')");
		engEvalString(m_eng, "evalin('base', 'global port')");
		engEvalString(m_eng, "evalin('base', 'global args')");
		// send pid
		command = "evalin('base', 'pid = " + QString::number(pid) + "')";
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		// send port
		command = QString("evalin('base', 'port = %1')").arg(port);
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		// send host
		engEvalString(m_eng, "evalin('base', 'host = ''127.0.0.1''')");

		if (!args.isEmpty()) {
			// send json args
			command = QString("evalin('base', 'args=jsondecode(''%1'')')").arg(args.simplified());
		}
		else
			command = QString("evalin('base', 'args = []')");

		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());

		// add path to MATLAB
		// mex files
		QString mexDir = settings.value(matlab_interface::matlab_mex_dir).toString();
		command = QString("evalin('base', 'mexDir='%1')").arg(mexDir);
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		command = QString("evalin('base', 'addpath(mexDir)')");
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		// now set current dir to be the plugin dir
		QString pluginPath = settings.value(matlab_interface::matlab_plugin_dir).toString();
		command = QString("evalin('base', 'cd %1')").arg(pluginPath);
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		// init Python
		QString pyExec = settings.value(matlab_interface::python_exe).toString();
	//	QString pyPackageDir = settings.value(matlab_interface::python_package_dir).toString();
		QString matlabApiDir = settings.value(matlab_interface::matlab_api_dir).toString();
		command = QString("evalin('base', 'pyExec='%1')").arg(pyExec);
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		//command = QString("evalin('base', 'pyenv(''Version'', pyExec))'");
		//tmp = command.toStdString();
		//engEvalString(m_eng, tmp.data());
		//command = QString("evalin('base', 'pyPackageDir=''%1'')").arg(pyPackageDir);
		//tmp = command.toStdString();
		//engEvalString(m_eng, tmp.data());
		//command = QString("evalin('base', 'insert(py.sys.path, int64(0), pyPackageDir);'");
		//tmp = command.toStdString();
		//engEvalString(m_eng, tmp.data());
		command = QString("evalin('base', 'matlabApiDir=''%1'')").arg(matlabApiDir);
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		command = QString("evalin('base', 'addpath(matlabApiDir)')");
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		command = QString("evalin('base', 'init_python(%1, %2, pyExec)'").arg(pid).arg(port);
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		engOutputBuffer(m_eng, buffer, AW_MATLAB_OUTPUT_BUFFER_SIZE);

		// add path to AnyWave parent folder
//		QDir awPath(QCoreApplication::applicationDirPath());

		// add dependencies paths
//	command = QString("addpath(genpath('%1'))").arg(dep);
//		engEvalString(m_eng, command.toLatin1().data()); 

		

//#ifdef Q_OS_WIN
//	//	command = "addpath('" + awPath.absolutePath() + "')";
//		//envPath = "setenv('PATH', '" + awPath.absolutePath() + "')";
//		//engEvalString(m_eng, envPath.toLatin1().data()); // setenv('PATH', 'Path to AnyWave directory');
//		//engEvalString(m_eng, command.toLatin1().data()); 
//        awPath.cd("Plugins/MATLAB/AnyWave");
//#endif
//#ifdef Q_OS_MAC
//		awPath.cdUp();
//		//QString dyld = QString("setenv('DYLD_LIBRARY_PATH', '%1/dylibs')").arg(awPath.absolutePath());
//	//	engEvalString(m_eng, dyld.toLatin1().data()); 
//	//	awPath.cdUp();
//	//	awPath.cdUp();
//		awPath.cd("Plugins/MATLAB/AnyWave");
//#endif
//#ifdef Q_OS_LINUX
//        //QString ld = QString("setenv('LD_LIBRARY_PATH', '/lib/x86_64-linux-gnu:/usr/lib/x86_64-linux-gnu:%1/lib')").arg(awPath.absolutePath());
//        //QByteArray ld = qgetenv("LD_LIBRARY_PATH");
//        //QString setld = QString("setenv('%1')").arg(QString(ld));
//        //engEvalString(m_eng, setld.toLatin1().data());
//        awPath.cd("Plugins/MATLAB/AnyWave");
//#endif
//		command = "addpath('" + awPath.absolutePath() + "')";
//		tmp = command.toStdString();
//	//	engEvalString(m_eng, command.toLatin1().data());
//		engEvalString(m_eng, tmp.data());
//		// add a variable named aw_pid which is the index of the process in AwMatlabServer
//	//	command = "cd " + path;
//
//	//	engEvalString(m_eng, command.toLatin1().data());
//		// launch main function with aw_pid parameter
//		// the function is evaluated by the 'base' workspace (required for GUI intensive applications)
//		//// search for main.m or main.mlapp
//		//if (QFile::exists(QString("%1/main.m").arg(path)))
		command = "evalin('base','main')";
		tmp = command.toStdString();
		engEvalString(m_eng, tmp.data());
		emit progressChanged(QString(buffer));
		engClose(m_eng);
		delete[] buffer;
	}
	else
		emit progressChanged(tr("Failed to connect to MATLAB!"));
}
