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
	typedef std::basic_stringbuf<char16_t> StringBuf;
	std::shared_ptr<StringBuf> output = std::make_shared<StringBuf>();
	std::shared_ptr<StringBuf> error = std::make_shared<StringBuf>();
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
			m_matlabPtr->eval(u"args=jsondecode(args);", output, error);
		}
		else {
			m_matlabPtr->setVariable(u"args", factory.createEmptyArray(), matlab::engine::WorkspaceType::BASE);
			emit progressChanged("args is empty.");
		}

		QString mexDir = settings.value(matlab_interface::matlab_mex_dir).toString();
		cppString = mexDir.toStdString();
		m_matlabPtr->feval(u"addpath", factory.createCharArray(cppString), output, error);
		QString pluginPath = settings.value(matlab_interface::matlab_plugin_dir).toString();
		cppString = pluginPath.toStdString();
		m_matlabPtr->feval(u"addpath", factory.createCharArray(cppString), output, error);
		m_matlabPtr->eval(u"main", output, error);

		String output_ = output.get()->str();
		emit progressChanged(QString::fromStdU16String(output_));
	}
	catch (const matlab::engine::EngineException& e)
	{
		emit progressChanged(QString("MATLAB Support error: %1").arg(QString(e.what())));
		return;
	}
	catch (const matlab::engine::MATLABExecutionException& e)
	{
		emit progressChanged(QString("MATLAB Support error: %1").arg(QString(e.what())));

	}
	catch (matlab::engine::MATLABSyntaxException& e)
	{
		emit progressChanged(QString("MATLAB Syntax Error: %1").arg(QString(e.what())));
	}

}
