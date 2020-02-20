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
#include "Debug/AwDebugLog.h"
#include <QThread>
#include "Marker/AwMarkerManager.h"
#include "Source/AwSourceManager.h"
#include "Plugin/AwPluginManager.h"
#include "AwPidManager.h"
#include <AwFileIO.h>


AwRequestServer::AwRequestServer(QObject *parent) : AwDataClient(parent)
{
	m_thread = new QThread(this);
	m_server = new QTcpServer(this);
	m_serverPort = 0;
	m_ds = nullptr;
	m_pidCounter = 0;

	if (m_server->listen(QHostAddress::Any, 0)) {
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

AwRequestServer::AwRequestServer(const QString& dataPath, QObject *parent) : AwDataClient(parent)
{
	m_thread = new QThread(this);
	m_server = new QTcpServer(this);
	m_serverPort = 0;
	m_ds = nullptr;
	m_pidCounter = 0;

	if (m_server->listen(QHostAddress::Any, 0)) {
		m_serverPort = m_server->serverPort();
		AwDebugLog::instance()->connectComponent(QString("MATPy Listener:%1").arg(m_serverPort), this);
		auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(dataPath);
		if (reader->openFile(dataPath) != AwFileIO::NoError) {
			emit log(QString("Unable to open %1").arg(dataPath));
			m_isListening = false;
			return;
		}
		m_ds = AwDataServer::getInstance()->duplicate(reader);
		m_ds->openConnection(this);
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

AwRequestServer::~AwRequestServer()
{
	m_thread->exit();
	m_thread->wait();
	// is there a decicated data server?
	if (m_ds)
		delete m_ds;
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

AwFileIO *AwRequestServer::reader()
{
	if (m_ds)
		return m_ds->reader();
	return nullptr;
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
	// handling requests
	emit log("Request received");
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
	if (pid >= 0) {
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
	
	// WARNING: p can be nullptr if the pid was negative. 
	// a nullptr p means that we are running in dedicated data server mode : AnyWave was launched by a Python/MATLAB plugin with a specified file.
	auto h = m_handlers[request];
	h(client, p);
}
