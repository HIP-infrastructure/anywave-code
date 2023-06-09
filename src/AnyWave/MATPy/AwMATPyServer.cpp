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
#include "AwMATPyServer.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDir>
#include <QThread>
#include "Debug/AwDebugLog.h"
#include "Marker/AwMarkerManager.h"
#include "Montage/AwMontageManager.h"
#include "AwPidManager.h"
#include "AwRequestServer.h"
#include "Plugin/AwPluginManager.h"
#include "AwCore.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// statics

AwMATPyServer *AwMATPyServer::m_instance = NULL;


AwMATPyServer *AwMATPyServer::instance()
{
	if (!m_instance) {
		// instantiate server and move it to another thread.
		m_instance = new AwMATPyServer();
	}
	return m_instance;
}

AwMATPyServer *AwMATPyServer::newInstance()
{	
	auto instance = new AwMATPyServer;
	AwMATPyServer::instance()->addDuplicatedInstance(instance);
	return instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AwMATPyServer::AwMATPyServer()
{
	m_rs = nullptr;
}

AwMATPyServer::~AwMATPyServer()
{
	// destroy possible duplicated instances
	stop();
	AW_DESTROY_LIST(m_duplicatedInstances);
}

void AwMATPyServer::addDuplicatedInstance(AwMATPyServer *instance)
{
	m_duplicatedInstances << instance;
}

void AwMATPyServer::deleteDuplicatedInstance(AwMATPyServer *instance)
{
	m_duplicatedInstances.removeAll(instance);
	delete instance;
}

void AwMATPyServer::setDebugMode(bool flag)
{
	if (m_rs)
		m_rs->setDebugMode(flag);
}

//bool AwMATPyServer::start(const QString& dataPath, AwScriptProcess *p, quint16 port)
//{
//	if (m_rs) {
//		delete m_rs;
//		m_rs = nullptr;
//	}
//	AwPidManager::instance()->createNewPid(p);
//	auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(dataPath);
//	if (reader == nullptr)
//		return false;
//	m_rs = new AwRequestServer(dataPath, port, p);
//	if (!m_rs->isListening()) { // failed to listen on TCP port
//		delete m_rs;
//		m_rs = nullptr;
//		return false;
//	}
//	return true;
//}

bool AwMATPyServer::isListening()
{
	if (m_rs)
		return m_rs->isListening();
	return false;
}

bool AwMATPyServer::start(quint16 port)
{
	if (m_rs) // already instantianted
		return true;

	m_rs = new AwRequestServer(port);
	if (!m_rs->isListening()) { // failed to listen on TCP port
		delete m_rs;
		m_rs = nullptr;
		return false;
	}
	return true;
}	


void AwMATPyServer::stop()
{
	if (m_rs)  {
		delete m_rs;
		m_rs = nullptr;
	}
}

quint16 AwMATPyServer::serverPort()
{
	if (m_rs == nullptr)
		return 0;
	return m_rs->serverPort();
}