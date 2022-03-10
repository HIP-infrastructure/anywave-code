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
#include "AwDataServer.h"
#include <QMessageBox>
#include "AwDataClient.h"
#include "Montage/AwMontageManager.h"
#include "Process/AwProcessManager.h"
#include <AwProcessInterface.h>
#include "Debug/AwDebugLog.h"
#include "AwDataManager.h"

#include <QThread>

#ifndef NDEBUG
#include <QApplication>
#endif

// statics
AwDataServer *AwDataServer::m_instance = nullptr;
int AwDataServer::m_instanceCount = 1;

AwDataServer *AwDataServer::getInstance()
{
	if (!m_instance)
		m_instance = new AwDataServer();
	return m_instance;
}

AwDataServer* AwDataServer::newInstance()
{
	return new AwDataServer();
}

AwDataServer::AwDataServer()
{
	m_plugin = nullptr;
	m_reader = nullptr;
	m_sem = new QSemaphore(1);
	AwDebugLog::instance()->connectComponent(QString("Data Server:%1").arg(m_instanceCount), this);
	m_instanceCount++;
}

AwDataServer::~AwDataServer()
{
	closeAllConnections();
//	if (m_reader)
//		delete m_reader;
		//if (m_plugin)
		//	m_plugin->deleteInstance(m_reader);
	delete m_sem;
}

void AwDataServer::setMainReader(AwFileIO *fileReader)
{
	// MUST BE CALLED ONLY WHEN A NEW FILE IS OPEN BY ANYWAVE
	// If a reader is already set, close active connections if any and destroy the reader.
	if (m_reader)	{
		closeAllConnections();
		if (m_plugin)
			m_plugin->deleteInstance(m_reader);
	}
	m_reader = fileReader;
	m_plugin = fileReader->plugin();
}

AwDataServer *AwDataServer::duplicate(AwFileIO *fileReader)
{
	auto ds = new AwDataServer();
	ds->setMainReader(fileReader);
	return ds;
}

// SLOTS

///
/// openConnection()
/// Ouvre une connexion entre le dataserver et le client.
/// Verifie si le client n'est pas deja connecte a un thread, auquel cas la connexion precedente est fermee.
///
void AwDataServer::openConnection(AwDataClient *client)
{
	if (m_clientToConnection.contains(client))
		closeConnection(client);

	// Data Manager should be the parent of DataServer, but check it before using it
	AwDataConnection *dc = new AwDataConnection(this, client);
	
	QThread *t = new QThread();
	dc->moveToThread(t);
	connect(client, SIGNAL(needData(AwChannelList *, float, float, bool)), dc, SLOT(loadData(AwChannelList *, float, float, bool)));
	connect(client, SIGNAL(needData(AwChannelList *, AwMarker *,bool)), dc, SLOT(loadData(AwChannelList *, AwMarker *, bool)));
	connect(client, SIGNAL(needData(AwChannelList *, AwMarkerList *, bool)), dc, SLOT(loadData(AwChannelList *, AwMarkerList *, bool)));
	connect(dc, SIGNAL(outOfMemory()), this, SLOT(manageOutOfMemory()));
	m_clientToConnection.insert(client, dc);
	m_dataConnections.append(dc);
	client->setConnected();
	t->start();
}

// 
// openConnection()
// in: client, reader
// out: none
// Open a connection with a client and a specified reader.
// Do not raise any UI Widgets. 
// This method is called during task execution.
void AwDataServer::openConnection(AwDataClient *client, AwFileIO *reader)
{
	if (m_clientToConnection.contains(client))
		closeConnection(client);

	AwDataConnection *dc = new AwDataConnection(this, client, reader);
	connect(client, SIGNAL(needData(AwChannelList *, float, float, bool, bool)), dc, SLOT(loadData(AwChannelList *, float, float, bool, bool)));
	connect(client, SIGNAL(needData(AwChannelList *, AwMarker *, bool, bool)), dc, SLOT(loadData(AwChannelList *, AwMarker *, bool, bool)));
	connect(client, SIGNAL(needData(AwChannelList *, AwMarkerList *, bool)), dc, SLOT(loadData(AwChannelList *, AwMarkerList *, bool)));
	connect(dc, SIGNAL(outOfMemory()), this, SLOT(manageOutOfMemory()));
	QThread *t = new QThread();
	dc->moveToThread(t);
	m_clientToConnection.insert(client, dc);
	m_dataConnections.append(dc);
	client->setConnected();
	t->start();
}


void AwDataServer::closeConnection(AwDataClient *client)
{
	if (!m_clientToConnection.contains(client))
		return;

	AwDataConnection *dc = m_clientToConnection.value(client);
	
	dc->thread()->exit(0);
	dc->thread()->wait();
	delete dc->thread();

	m_clientToConnection.remove(client);
	m_dataConnections.removeAll(dc);
	delete dc;
	client->setConnected(false);
}

//
// closeAllConnections()
// force all clients to be disconnected and associated server threads to be terminated.
void AwDataServer::closeAllConnections()
{
	for (auto dc : m_dataConnections) {
		dc->thread()->exit(0);
		dc->thread()->wait();
		delete dc->thread();
		delete dc;
	}
	m_clientToConnection.clear();
	m_dataConnections.clear();
}


void AwDataServer::manageOutOfMemory()
{
	QMessageBox::critical(0, tr("Out Of Memory"), tr("Can't allocate enough memory to load data."));
}




