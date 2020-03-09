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
#ifndef AW_REQUEST_SERVER_H
#define AW_REQUEST_SERVER_H
#include <QObject>
#include <QTcpServer>
class AwScriptProcess;
#include <AwDataClient.h>
#include "Data/AwDataSet.h"
class AwFileIO;


class AwRequestServer : public AwDataClient
{
	Q_OBJECT
public:
	// default constructor
	explicit AwRequestServer(QObject *parent = 0);
	// constructor that spawn a dedicated data server for the specified file. if port is specified, listen on that port.
	AwRequestServer(const QString& dataPath, QObject *parent = 0);
	~AwRequestServer();
	inline bool isListening() { return m_isListening; }
	inline quint16 serverPort() { return m_serverPort; }
	void addProcess(AwScriptProcess *process);
	/** add a request handler **/
	void addHandler(AwRequestServer* const object, void(AwRequestServer::* const mf)(QTcpSocket *, AwScriptProcess*), int request);
public slots:
	void handleNewConnection();
	void dataReceived();
	void clientDisconnected();
signals:
	void log(const QString& message);
	void markersAdded(AwMarkerList *markers);
	void beamformerAvailable(QString path);
protected:
	void handleRequest(int request, QTcpSocket *client, int pid);

	QTcpServer *m_server;
	QMutex m_mutex;
	QThread *m_thread;
	bool m_isListening;
	quint16 m_serverPort;
	AwDataSet *m_ds;
	int m_pidCounter;

private:
	void handleGetMarkers2(QTcpSocket *client, AwScriptProcess *process);
	void handleGetProperties(QTcpSocket *client, AwScriptProcess *process);
	void handleGetData3(QTcpSocket *client, AwScriptProcess *process);
	void handleGetDataEx(QTcpSocket *client, AwScriptProcess *process);
	void handleGetMarkersEx(QTcpSocket *client, AwScriptProcess *process);
	void handleAddMarkers(QTcpSocket *client, AwScriptProcess *process);
	void handleGetPluginInfo(QTcpSocket *client, AwScriptProcess *process);
	void handleGetPluginIO(QTcpSocket *client, AwScriptProcess *process);
	void handleGetFileInfo(QTcpSocket *client, AwScriptProcess *process);
	void handleIsTerminated(QTcpSocket *client, AwScriptProcess *process);
	void handleSendMessage(QTcpSocket *client, AwScriptProcess *process);
	void handleSendCommand(QTcpSocket *client, AwScriptProcess *process);
	void handleGetScreenCapture(QTcpSocket *client, AwScriptProcess *process);
	void handleGetICAPanelCapture(QTcpSocket *client, AwScriptProcess *process);
	void handleSetBeamFormer(QTcpSocket *client, AwScriptProcess *process);
	void handleGetTriggers(QTcpSocket *client, AwScriptProcess *process);
	void handleOpenNewFile(QTcpSocket *client, AwScriptProcess *process);
	void handleRunAnyWave(QTcpSocket *client, AwScriptProcess *process);
	void unusedHandler(QTcpSocket *client, AwScriptProcess *process) {}

	void setHandlers();

	QHash<int, std::function<void(QTcpSocket*, AwScriptProcess *)>> m_handlers;
	AwMarkerList m_markers;	// hold the markers added by process
};


#endif