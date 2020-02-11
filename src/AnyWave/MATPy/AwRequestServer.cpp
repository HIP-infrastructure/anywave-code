/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwRequestServer.h"
#include "Process/AwScriptPlugin.h"
#include <QTcpSocket>
#include <QDataStream>
#include "AwPidManager.h"
#include "Data/AwDataServer.h"
#include "Debug/AwDebugLog.h"
#include <QThread>
#include "Marker/AwMarkerManager.h"
#include "Source/AwSourceManager.h"
#include "Plugin/AwPluginManager.h"

AwRequestServer::AwRequestServer(QObject *parent) : AwDataClient(parent)
{
	m_thread = new QThread(this);
	m_server = new QTcpServer(this);
	m_serverPort = 0;
	m_ds = nullptr;
	m_pm = nullptr;

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
	moveToThread(m_thread);
	m_thread->start();
}

AwRequestServer::AwRequestServer(const QString& dataPath, quint16 port, QObject *parent) : AwDataClient(parent)
{
	m_thread = new QThread(this);
	m_server = new QTcpServer(this);
	m_serverPort = 0;
	m_ds = nullptr;
	m_pm = nullptr;

	if (m_server->listen(QHostAddress::Any, port)) {
		m_serverPort = m_server->serverPort();
		AwDebugLog::instance()->connectComponent("MATPy Listener", this);
		//AwDataServer::getInstance()->openConnection(this);
		auto reader = AwPluginManager::getInstance()->getReaderToOpenFile(dataPath);
		m_ds = AwDataServer::getInstance()->duplicate(reader);
		m_ds->openConnection(this);
		connect(m_server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
		m_isListening = true;
	}
	else
		m_isListening = false;
}

AwRequestServer::~AwRequestServer()
{
	m_thread->exit();
	m_thread->wait();
	// is there a decicated data server?
	if (m_ds)
		delete m_ds;
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
	QByteArray size;
	QDataStream stream_size(&size, QIODevice::WriteOnly);
	stream_size.setVersion(QDataStream::Qt_4_4);
	int status = 0;
	emit log(tr("Received request ") + QString::number(request));
	AwScriptProcess *p = nullptr;
	// get the matchin process if pid is valid
	if (pid >= 0) {
		if (m_pm == nullptr) { // SHOULD NEVER HAPPENS
			emit log(tr("ERROR: request server has no connection to a pid manager instance."));
			return;
		}
		p = m_pm->process(pid);

		if (p == NULL) {// no process found, send a bad status as response
			// write a bad status
			status = -1;
			stream_size << status;
			client->write(size);
			emit log(tr("No corresponding process found for the request.."));
			emit log(tr("Writing status code to socket..."));
			client->waitForBytesWritten();
			emit log(tr("Done."));
			return;
		}
	}
	
	// WARNING: p can be nullptr if the pid was negative. 
	// a nullptr p means that we are running in dedicated data server mode : AnyWave was launched by a Python/MATLAB plugin with a specified file.

	switch (request)
	{
	case AwRequest::OpenNewFile:
		handleOpenNewFile(client, p);
		break;
	case AwRequest::GetMarkers2:
		handleGetMarkers2(client, p);
		break;
	case AwRequest::GetData3:
		handleGetData3(client, p);
		break;
	case AwRequest::GetDataEx:
		handleGetDataEx(client, p);
		break;
	case AwRequest::GetMarkersEx:
		handleGetMarkersEx(client, p);
		break;
	case AwRequest::AddMarkers:
		handleAddMarkers(client, p);
		break;
	case AwRequest::GetPluginInfo:
		handleGetPluginInfo(client, p);
		break;
	case AwRequest::GetPluginIO:
		handleGetPluginIO(client, p);
		break;
	case AwRequest::GetFileInfo:
		handleGetFileInfo(client, p);
		break;
	case AwRequest::IsTerminated:
		handleIsTerminated(client, p);
		break;
	case AwRequest::SendMessage:
		handleSendMessage(client, p);
		break;
	case AwRequest::SendCommand:
		handleSendCommand(client, p);
		break;
	case AwRequest::GetScreenCapture:
		handleGetScreenCapture(client, p);
		break;
	case AwRequest::GetICAPanelCapture:
		handleGetICAPanelCapture(client, p);
		break;
	case AwRequest::SetBeamFormer:
		handleSetBeamFormer(client, p);
		break;
	case AwRequest::GetTriggers:
		handleGetTriggers(client, p);
		break;
	default:
		emit log("Unknown request received!");
		break;
	}
}
