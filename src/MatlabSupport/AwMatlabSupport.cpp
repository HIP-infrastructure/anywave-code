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
#include "MatlabDataArray.hpp"

constexpr auto AW_MATLAB_OUTPUT_BUFFER_SIZE = 1024 * 20;	// 20Kbytes buffer


void AwMatlabSupport::printFromBuf(const std::shared_ptr<SBuf> buf)
{
	//Get text from buf
	auto text_ = buf->str();
	if (!text_.empty())
		emit progressChanged(QString::fromStdU16String(text_));
/*	std::cout << "*" << convertUTF16StringToUTF8String(text_)
		<< "*" << std::endl*/;
}

/** run()
- path: path to the script to be executed.
- dep: path to dependency folder to add to the path.
- pid: process id of the script.
**/
void AwMatlabSupport::run(const QVariantMap& settings)
{
	//	m_eng = engOpen(nullptr); 

	emit progressChanged("Opening MATLAB Connection...");
	try {
		m_matlabPtr = matlab::engine::startMATLAB();
		emit progressChanged("Connection to MATLAB established.");
		QString command;
		std::string tmp;
		matlab::data::String matlabString;
		auto outBuf = std::make_shared<SBuf>();
		auto errBuf = std::make_shared<SBuf>();
		// Create matlab data array factory
		matlab::data::ArrayFactory factory;
		int pid = settings.value(matlab_interface::pid).toInt();
		quint16 port = static_cast<quint16>(settings.value(matlab_interface::port).toUInt());
		// Create data variable
		m_matlabPtr->setVariable(u"pid", factory.createScalar<int>(pid), matlab::engine::WorkspaceType::GLOBAL);

		m_matlabPtr->setVariable(u"port", factory.createScalar<int>(port), matlab::engine::WorkspaceType::GLOBAL);

		m_matlabPtr->setVariable(u"host", factory.createCharArray("127.0.0.1"), matlab::engine::WorkspaceType::GLOBAL);
		QString args = settings.value(matlab_interface::json).toString();
		if (!args.isEmpty()) {
			tmp = args.toStdString();
			m_matlabPtr->setVariable(u"args", factory.createCharArray(tmp), matlab::engine::WorkspaceType::GLOBAL);
			command = QString("args=jsondecode(args);");
			emit progressChanged("evaluating args = jsondecode(args);");
			//mp = command.toStdString();
			matlabString = command.toStdU16String();
			// matlab::engine::convertUTF8StringToUTF16String(tmp);
			m_matlabPtr->eval(matlabString, outBuf, errBuf);
			printFromBuf(outBuf);
			printFromBuf(errBuf);
		}
		else {
			m_matlabPtr->setVariable(u"args", factory.createCharArray(""), matlab::engine::WorkspaceType::GLOBAL);
			emit progressChanged("args is empty.");
		}

		
		QString mexDir = settings.value(matlab_interface::matlab_mex_dir).toString();
		matlabString = mexDir.toStdU16String();
		//m_matlabPtr->feval<void>(u"addpath", tmp);
		m_matlabPtr->feval(u"addpath", factory.createCharArray(matlabString), outBuf, errBuf);
		//m_matlabPtr->eval(matlabString, outBuf, errBuf);
		printFromBuf(outBuf);
		printFromBuf(errBuf);
		//m_matlabPtr->eval(u"path", outBuf, errBuf);
		//printFromBuf(outBuf);
		//printFromBuf(errBuf);
	
		
		QString pluginPath = settings.value(matlab_interface::matlab_plugin_dir).toString();
		matlabString = pluginPath.toStdU16String();
		m_matlabPtr->feval(u"addpath", factory.createCharArray(matlabString), outBuf, errBuf);
		//emit progressChanged("evaluating cd plugin_path");
		//command = QString("cd '%1'").arg(QDir::toNativeSeparators(pluginPath));
		//matlabString = command.toStdU16String();
		//m_matlabPtr->eval(matlabString, outBuf, errBuf);
		printFromBuf(outBuf);
		printFromBuf(errBuf);
		QString matlabApiDir = settings.value(matlab_interface::matlab_api_dir).toString();
		//command = QString("addpath(genpath('%1'))").arg(QDir::toNativeSeparators(matlabApiDir));
		matlabString = matlabApiDir.toStdU16String();
		m_matlabPtr->feval(u"addpath", factory.createCharArray(matlabString), outBuf, errBuf);
		printFromBuf(outBuf);
		printFromBuf(errBuf);

		emit progressChanged("init python");
		// add all python paths
		// set 
		QString pyVenc = settings.value(matlab_interface::python_venv_dir).toString();
		matlabString = pyVenc.toStdU16String();
		m_matlabPtr->setVariable(u"pyVenvDir", factory.createCharArray(matlabString), matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->eval(u"addpath(genpath(pyVenvDir));", outBuf, errBuf);
		QString pyExec = settings.value(matlab_interface::python_exe).toString();
		//command = QString("init_python(%1, %2, '%3')").arg(pid).arg(port).arg(QDir::toNativeSeparators(pyExec));
		matlabString = pyExec.toStdU16String();
		m_matlabPtr->setVariable(u"pyExec", factory.createCharArray(matlabString), matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->eval(u"pyExec", outBuf, errBuf);
		printFromBuf(outBuf);
		printFromBuf(errBuf);
		m_matlabPtr->eval(u"pyenv('Version', pyExec)", outBuf, errBuf);
		printFromBuf(outBuf);
		printFromBuf(errBuf);
	//	m_matlabPtr->eval(u"py.anywave.pid", outBuf, errBuf);
		//m_matlabPtr->eval(u"py.anywave.pid = py.int(pid);", outBuf, errBuf);
		//printFromBuf(outBuf);
		//printFromBuf(errBuf);
		return;


		tmp = pyExec.toStdString();
		std::vector<matlab::data::Array> params = {
			factory.createScalar<int>(pid), factory.createScalar<int>(port), factory.createCharArray(tmp)
		}; 
		const size_t numReturned = 0;
		m_matlabPtr->feval(u"init_python", numReturned, params);
		//printFromBuf(outBuf);
		//printFromBuf(errBuf);
		emit progressChanged("python init done.");
		emit progressChanged("running main()");
	//	m_matlabPtr->eval(u"main()", outBuf, errBuf);
	//	m_matlabPtr->feval(u"main", {}, outBuf, errBuf);
		printFromBuf(outBuf);
		printFromBuf(errBuf);
		//emit progressChanged(command);
		//m_matlabPtr->eval(command.toStdU16String(), outBuf
	}
	catch (const matlab::engine::EngineException& e)
	{
		emit progressChanged(QString("MATLAB Support error: %1").arg(QString(e.what())));
		return;
	}
	//if (m_eng != nullptr) {
	//	emit progressChanged("Connection to MATLAB established.");
	//	char *buffer = new char[AW_MATLAB_OUTPUT_BUFFER_SIZE + 1];
	//	buffer[AW_MATLAB_OUTPUT_BUFFER_SIZE] = '\0';
	//	engOutputBuffer(m_eng, buffer, AW_MATLAB_OUTPUT_BUFFER_SIZE);
	//	QString command;
	//	QString envPath;
	//	std::string tmp;
	//	quint16 port = static_cast<quint16>(settings.value(matlab_interface::port).toUInt());
	//	int pid = settings.value(matlab_interface::pid).toInt();
	//	QString args = settings.value(matlab_interface::json).toString();
	//	
	//	// set global variables host, pid, port and json_args
	//	engEvalString(m_eng, "evalin('base', 'global host')");
	//	engEvalString(m_eng, "evalin('base', 'global pid')");
	//	engEvalString(m_eng, "evalin('base', 'global port')");
	//	engEvalString(m_eng, "evalin('base', 'global args')");
	//	// send pid
	//	command = "evalin('base', 'pid = " + QString::number(pid) + "')";
	//	emit progressChanged(command);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	// send port
	//	command = QString("evalin('base', 'port = %1')").arg(port);
	//	emit progressChanged(command);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	// send host
	//	engEvalString(m_eng, "evalin('base', 'host = ''127.0.0.1''')");

	//	if (!args.isEmpty()) {
	//		// send json args
	//		command = QString("evalin('base', 'args=jsondecode(''%1'')')").arg(args.simplified());
	//		emit progressChanged(command);
	//	}
	//	else {
	//		command = QString("evalin('base', 'args = []')");
	//		emit progressChanged(command);
	//	}

	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());

	//	// add path to MATLAB
	//	// mex files
	//	QString mexDir = settings.value(matlab_interface::matlab_mex_dir).toString();
	//	command = QString("evalin('base', 'mexDir=''%1'')").arg(mexDir);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
	//	command = QString("evalin('base', 'addpath(genpath(mexDir))')");
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
	//	// now set current dir to be the plugin dir
	//	QString pluginPath = settings.value(matlab_interface::matlab_plugin_dir).toString();
	//	command = QString("evalin('base', 'cd %1')").arg(pluginPath);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
	//	// init Python
	//	QString pyExec = settings.value(matlab_interface::python_exe).toString();
	//	QString matlabApiDir = settings.value(matlab_interface::matlab_api_dir).toString();
	//	command = QString("evalin('base', 'pyExec=''%1'')").arg(pyExec);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
	//	command = QString("evalin('base', 'matlabApiDir=''%1'')").arg(matlabApiDir);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
	//	command = QString("evalin('base', 'addpath(genpath(matlabApiDir))')");
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
	//	command = QString("evalin('base', 'init_python(%1, %2, pyExec)'").arg(pid).arg(port);
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(command);
		

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



	//	command = "evalin('base','main')";
	//	tmp = command.toStdString();
	//	engEvalString(m_eng, tmp.data());
	//	emit progressChanged(QString(buffer));
	//	engClose(m_eng);
	//	delete[] buffer;
	//}
	//else
	//	emit progressChanged(tr("Failed to connect to MATLAB!"));
}
