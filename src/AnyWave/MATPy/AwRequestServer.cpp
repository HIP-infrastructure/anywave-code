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
#include "Process/AwProcessManager.h"

AwRequestServer::AwRequestServer(quint16 port, QObject *parent) : AwDataClient(parent)
{
	m_thread = new QThread(this);
	m_server = new QTcpServer(this);
	m_serverPort = 0;
	m_pidCounter = 0;

	AwDebugLog::instance()->connectComponent("MATPy Listener", this);
	if (m_server->listen(QHostAddress::Any, port)) {
		m_serverPort = m_server->serverPort();
		AwDataServer::getInstance()->openConnection(this);
		connect(m_server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
		m_isListening = true;
	}
	else {
		m_errorString = m_server->errorString();
		emit log(QString("Failed to listen on tcp port: %1").arg(m_errorString));
		m_isListening = false;

	}
	connect(this, SIGNAL(beamformerAvailable(QString)), AwSourceManager::instance(), SLOT(load(QString)));

	setHandlers();
	if (m_isListening) {
		moveToThread(m_thread);
		m_thread->start();
	}
	m_debugMode = false;
	m_dataManager = AwDataManager::instance();
	connect(this, SIGNAL(markersAdded(AwMarkerList*)), m_dataManager->markerManager(), SLOT(addMarkers(AwMarkerList*)));
}

AwRequestServer::~AwRequestServer()
{
	m_thread->exit();
	m_thread->wait();
	m_processes.erase(m_processes.begin(), m_processes.end());
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
	addHandler(this, &AwRequestServer::handleGetData2_5_10, AwRequest::GetData2_5_10);
	addHandler(this, &AwRequestServer::handleGetMarkersEx, AwRequest::GetMarkersEx);	
	addHandler(this, &AwRequestServer::handleOpenNewFile, AwRequest::OpenNewFile);		
	addHandler(this, &AwRequestServer::handleRunAnyWave, AwRequest::RunAnyWave);			
	addHandler(this, &AwRequestServer::handleGetProperties, AwRequest::GetProperties);		
	addHandler(this, &AwRequestServer::handleConnectDebug, AwRequest::ConnectDebug);
	addHandler(this, &AwRequestServer::handleSendMarkers, AwRequest::SendMarkers);
	addHandler(this, &AwRequestServer::handleRun, AwRequest::Run);
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
	QMutexLocker lock(&m_mutex);
	// who is sending data?
	QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
	if (client == nullptr) {
		emit log("null socket client detected.... Aborting");
		return;
	}
	while (client->bytesAvailable() < sizeof(int) * 2)
		client->waitForReadyRead();
	int pid, size;
	QDataStream stream(client);
	stream.setVersion(QDataStream::Qt_4_4);
	stream >> pid >> size;
	while (client->bytesAvailable() < size)
		client->waitForReadyRead();
	int request;
	stream >> request;
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
	AwScriptProcess* p = nullptr;
	if (request == AwRequest::ConnectDebug) {
		emit log(QString("Debug connection: create a fake process for debuging purposes"));
		p = newDebugProcess();
		auto h = m_handlers.value(request);
		h(client, p);
		emit log(QString("Debug connection: ok"));
		return;
	}

	QByteArray size;
	QDataStream stream_size(&size, QIODevice::WriteOnly);
	stream_size.setVersion(QDataStream::Qt_4_4);
	int status = 0;
	emit log(tr("Received request ") + QString::number(request));

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
	AwProcessManager::instance()->initProcessSettings(p);

	if (dm->isFileOpen()) {
		p->pdi.input.setReader(AwDataManager::instance()->reader());
		p->pdi.input.setNewChannels(AwDataManager::instance()->montage(), true);
		p->pdi.input.settings = dm->settings();
		connect(p, SIGNAL(sendMarkers(AwMarkerList*)), dm->markerManager(), SLOT(addMarkers(AwMarkerList*)));
		connect(p, SIGNAL(sendMarker(AwMarker *)), dm->markerManager(), SLOT(addMarker(AwMarker*)));
	}
}

AwScriptProcess* AwRequestServer::newDebugProcess()
{
	auto p = new AwScriptProcess;
	p->setPlugin(nullptr);
	AwPidManager::instance()->createNewPid(p);
	initDebugProcess(p);
	m_processes << p;
	return p;
}