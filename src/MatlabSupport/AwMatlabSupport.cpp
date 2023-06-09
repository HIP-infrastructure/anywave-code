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
#include "AwMatlabSupport.h"
#include <QtPlugin>
#include <QDir>
#include <QSettings>
#include <QtCore>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"

constexpr auto AW_MATLAB_OUTPUT_BUFFER_SIZE = 1024 * 20;	// 20Kbytes buffer


/** run()
- path: path to the script to be executed.
- dep: path to dependency folder to add to the path.
- pid: process id of the script.
**/
void AwMatlabSupport::run(const QVariantMap& settings)
{
	emit progressChanged("Opening MATLAB Connection...");
	
	using namespace matlab::engine;
	using namespace matlab::data;
	typedef std::basic_stringbuf<char16_t> StringBuf;
	std::shared_ptr<StringBuf> output = std::make_shared<StringBuf>();
	
	try {
		std::unique_ptr<matlab::engine::MATLABEngine> m_matlabPtr = matlab::engine::startMATLAB();
		emit progressChanged("Connection to MATLAB established.");
		QString command;
		std::string cppString;
		String matlabString;

		// Create matlab data array factory
		matlab::data::ArrayFactory factory;
		int pid = settings.value(matlab_interface::pid).toInt();
		quint16 port = static_cast<quint16>(settings.value(matlab_interface::port).toUInt());
		// Create matlab variables
		std::string host = "127.0.0.1";
		double dpid = static_cast<double>(pid);
		double dport = static_cast<double>(port);
		m_matlabPtr->setVariable(u"pid", factory.createScalar<double>(dpid) , matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->setVariable(u"port", factory.createScalar<double>(dport) , matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->setVariable(u"host", factory.createCharArray(host) ,matlab::engine::WorkspaceType::BASE);
		QString args = settings.value(matlab_interface::json).toString();
		if (!args.isEmpty()) {
			cppString = args.toStdString();
			m_matlabPtr->setVariable(u"args", factory.createCharArray(cppString), matlab::engine::WorkspaceType::BASE);
		//	m_matlabPtr->eval(u"args=jsondecode(args);", output, error);
		}
		else {
			m_matlabPtr->setVariable(u"args", factory.createCharArray(""), matlab::engine::WorkspaceType::BASE);
			emit progressChanged("args is empty.");
		}

		QString mexDir = settings.value(matlab_interface::matlab_mex_dir).toString();
		cppString = mexDir.toStdString();
		m_matlabPtr->feval(u"addpath", factory.createCharArray(cppString), output);
		QString pluginPath = settings.value(matlab_interface::matlab_plugin_dir).toString();
		cppString = pluginPath.toStdString();
		m_matlabPtr->feval(u"addpath", factory.createCharArray(cppString), output);

		m_matlabPtr->eval(u"main", output);
		// create varargin 
		//cppString = args.toStdString();
		//CellArray varargin = factory.createCellArray({ 1, 4 });
		//varargin[0][0] = factory.createCharArray(host);
		//varargin[0][1] = factory.createScalar<double>(dport);
		//varargin[0][2] = factory.createScalar<double>(dpid);
		//varargin[0][3] = factory.createCharArray(cppString);
		//m_matlabPtr->setVariable(u"test", std::move(varargin), matlab::engine::WorkspaceType::BASE);
		//m_matlabPtr->feval(u"main", varargin, output, error);
		//m_matlabPtr->feval(u"disp", factory.createCharArray("varargin"), output, error);
	
		String output_ = output.get()->str();
		QString message = QString::fromStdString(convertUTF16StringToUTF8String((output_)));
		emit progressChanged(message);


	}
	catch (const matlab::engine::EngineException& e)
	{
		emit progressChanged(QString("MATLAB Engine Exception: %1").arg(QString(e.what())));
	}
	catch (const matlab::engine::MATLABExecutionException& e)
	{
		emit progressChanged(QString("MATLAB Engine Execution: %1").arg(QString(e.what())));
	}
	catch (matlab::engine::MATLABSyntaxException& e)
	{
		emit progressChanged(QString("MATLAB Syntax Error: %1").arg(QString(e.what())));
	}
	String output_ = output.get()->str();
	QString message = QString::fromStdString(convertUTF16StringToUTF8String((output_)));
	emit progressChanged(message);
}
