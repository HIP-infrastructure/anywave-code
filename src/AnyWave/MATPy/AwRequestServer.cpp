/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QTcpSocket>
#include <QDataStream>
#include "Data/AwDataServer.h"
#include "Data/AwDataSet.h"
#include "Debug/AwDebugLog.h"
#include <QThread>
#include "Marker/AwMarkerManager.h"
#include "Source/AwSourceManager.h"
#include "Plugin/AwPluginManager.h"
#include "AwPidManager.h"
#include <AwFileIO.h>
#include "Prefs/AwSettings.h"
#include "Data/AwDataManager.h"

AwRequestServer::AwRequestServer(quint16 port, QObject *parent) : AwDataClient(parent)
{
	m_thread = new QThread(this);
	m_server = new QTcpServer(this);
	m_serverPort = 0;
	//m_ds = nullptr;
	m_pidCounter = 0;

	if (m_server->listen(QHostAddress::Any, port)) {
		m_serverPort = m_server->serverPort();
		AwDebugLog::instance()->connectComponent("MATPy Listener", this);
		AwDataServer::getInstance()->openConnection(this);
		connect(m_server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
		m_isListening = true;
	}
	else
		m_isListening = false;

	connect(this, SIGNAL(markersAdded(AwMarkerList *)), AwMarkerManager::instance(), SLOT(addMarkers(AwMarkerList *)));
	connect(this, SIGNAL(beamformerAvailable(QString)), AwSourceManager::instance(), SLOT(load(QString)));

	setHandlers();
	if (m_isListening) {
		moveToThread(m_thread);
		m_thread->start();
	}
}

//AwRequestServer::AwRequestServer(const QString& dataPath, quint16 port, QObject *parent) : AwDataClient(parent)
//{
//	m_thread = new QThread(this);
//	m_server = new QTcpServer(this);
//	m_serverPort = 0;
//	//m_ds = nullptr;
//	m_pidCounter = 0;
//
//	if (m_server->listen(QHostAddress::Any, port)) {
//		m_serverPort = m_server->serverPort();
//		AwDebugLog::instance()->connectComponent(QString("MATPy Listener:%1").arg(m_serverPort), this);
//		auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(dataPath);
//		if (reader->openFile(dataPath) != AwFileIO::NoError) {
//			emit log(QString("Unable to open %1").arg(dataPath));
//			m_isListening = false;
//			return;
//		}
//		m_ds = new AwDataSet(reader, this);
//		//m_ds = AwDataServer::getInstance()->duplicate(reader);
//		//m_ds->openConnection(this);
//		m_ds->connect(this);
//		connect(m_server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
//		m_isListening = true;
//	}
//	else 
//		m_isListening = false;
//
//	connect(this, SIGNAL(markersAdded(AwMarkerList *)), AwMarkerManager::instance(), SLOT(addMarkers(AwMarkerList *)));
//	connect(this, SIGNAL(beamformerAvailable(QString)), AwSourceManager::instance(), SLOT(load(QString)));
//
//	setHandlers();
//	if (m_isListening) {
//		moveToThread(m_thread);
//		m_thread->start();
//	}
//}

AwRequestServer::~AwRequestServer()
{
	m_thread->exit();
	m_thread->wait();
}

void AwRequestServer::setDebugMode()
{
	auto aws = AwSettings::getInstance();
	m_debugMode = aws->value(aws::plugin_debug_mode).toBool();
}

void AwRequestServer::setHandlers()
{
	addHandler(this, &AwRequestServer::handleAddMarkers, AwRequest::AddMarkers);		
	addHandler(this, &AwRequestServer::handleGetPluginInfo, AwRequest::GetPluginInfo);		
	addHandler(this, &AwRequestServer::handleIsTerminated, AwRequest::IsTerminated);		
	addHandler(this, &AwRequestServer::handleSendMessage, AwRequest::SendMessage);		
	addHandler(this, &AwRequestServer::handleSendCommand, AwRequest::SendCommand);		
	addHandler(this, &AwRequestServer::handleGetMarkers2, AwRequest::GetMarkers2);		
	addHandler(this, &AwRequestServer::handleGetData3, AwRequest::GetData3);			
	addHandler(this, &AwRequestServer::handleGetFileInfo, AwRequest::GetFileInfo);		
	addHandler(this, &AwRequestServer::handleGetScreenCapture, AwRequest::GetScreenCapture);	
	addHandler(this, &AwRequestServer::handleGetICAPanelCapture, AwRequest::GetICAPanelCapture);	
	addHandler(this, &AwRequestServer::handleSetBeamFormer, AwRequest::SetBeamFormer);		
	addHandler(this, &AwRequestServer::handleGetTriggers, AwRequest::GetTriggers);		
	addHandler(this, &AwRequestServer::handleGetPluginIO, AwRequest::GetPluginIO);		
	addHandler(this, &AwRequestServer::handleGetDataEx, AwRequest::GetDataEx);			
	addHandler(this, &AwRequestServer::handleGetMarkersEx, AwRequest::GetMarkersEx);	
	addHandler(this, &AwRequestServer::handleOpenNewFile, AwRequest::OpenNewFile);		
	addHandler(this, &AwRequestServer::handleRunAnyWave, AwRequest::RunAnyWave);			
	addHandler(this, &AwRequestServer::handleGetProperties, AwRequest::GetProperties);		
}




void AwRequestServer::addHandler(AwRequestServer *rs, void (AwRequestServer::* func)(QTcpSocket *, AwScriptProcess*), int request)
{
	using namespace std::placeholders;
	m_handlers[request] = std::bind(func, rs, _1, _2);
}

void AwRequestServer::handleNewConnection()
{
	QTcpSocket *client = m_server->nextPendingConnection();
	connect(client, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	connect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	emit log("New connection established with client.");
}

void AwRequestServer::dataReceived()
{
//	emit log("dataReceived called before mutex");
	QMutexLocker lock(&m_mutex);
//	emit log("dataReceived called after mutex");

	// who is sending data?
	QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
	if (client == nullptr) {
		emit log("null socket client detected.... Aborting");
		return;
	}

	//emit log("dataReceived waiting for 8 bytes to be available");
	while (client->bytesAvailable() < sizeof(int) * 2)
		client->waitForReadyRead();
	//emit log("dataReceived at least 8 bytes received.\nReading pid and data size");

	int pid, size;
	
	QDataStream stream(client);
	stream.setVersion(QDataStream::Qt_4_4);
	stream >> pid >> size;
	//emit log(QString("dataReceived got pid %1 and size is  %2 bytes").arg(pid).arg(size));
	//emit log(QString("dataReceived waiting for %1 bytes to be available").arg(size));
	while (client->bytesAvailable() < size)
		client->waitForReadyRead();

	//emit log("dataReceived got all bytes. Reading request id...");

	int request;
	stream >> request;
	//emit log(QString("dataReceived request id is %1").arg(request));
	handleRequest(request, client, pid);
}

void AwRequestServer::clientDisconnected()
{
	QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
	disconnect(client, SIGNAL(readyRead()), this, SLOT(dataReceived()));
	disconnect(client, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	client->deleteLater();
}


void AwRequestServer::handleRequest(int request, QTcpSocket *client, int pid)
{
	if (m_handlers.isEmpty() || !m_handlers.contains(request)) {
		emit log(QString("received unknown request: %2. Nothing done.").arg(request));
		return;
	}

	QByteArray size;
	QDataStream stream_size(&size, QIODevice::WriteOnly);
	stream_size.setVersion(QDataStream::Qt_4_4);
	int status = 0;
	emit log(tr("Received request ") + QString::number(request));
	AwScriptProcess *p = nullptr;
	// get the matchin process if pid is valid
	if (m_debugMode && pid < 0) {
		p = newDebugProcess();
	}
	else if (pid >= 0) {
		p = AwPidManager::instance()->process(pid);
		if (!p) {
			// write a bad status
			status = -1;
			stream_size << status;
			client->write(size);
			emit log(QString("No corresponding process found for pid %1.").arg(pid));
			client->waitForBytesWritten();
			emit log(tr("Done."));
			return;
		}
		// p may be instantiated but if we are in debug mode, the pdi.input may be empty if no reader is active
		// calling initDebugProcess() may solve this issue
		if (m_debugMode)
			initDebugProcess(p);
	}
	if (p == nullptr) {
		emit log("Received request but process is null. Skipped.");
		return;
	}
	
	// WARNING: p can be nullptr if the pid was negative. 
	// a nullptr p means that we are running in dedicated data server mode : AnyWave was launched by a Python/MATLAB plugin with a specified file.
	auto h = m_handlers.value(request);
	h(client, p);
}

void AwRequestServer::initDebugProcess(AwScriptProcess* p)
{
	auto dm = AwDataManager::instance();
	if (dm->isFileOpen()) {
		p->pdi.input.setReader(AwDataManager::instance()->reader());
		p->pdi.input.setNewChannels(AwDataManager::instance()->rawChannels(), true);
		p->pdi.input.settings = dm->settings();
		connect(p, SIGNAL(sendMarkers(AwMarkerList*)), dm->markerManager(), SLOT(addMarkers(AwMarkerList*)));
		connect(p, SIGNAL(sendMarker(AwMarker *)), dm->markerManager(), SLOT(addMarker(AwMarker*)));
	}
}

AwScriptProcess* AwRequestServer::newDebugProcess()
{
	auto p = new AwScriptProcess;
	AwPidManager::instance()->createNewPid(p);
	initDebugProcess(p);
	m_processes << p;
	return p;
}