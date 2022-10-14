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
#ifndef AW_REQUEST_SERVER_H
#define AW_REQUEST_SERVER_H
#include <QObject>
#include <QTcpServer>
class AwScriptProcess;
#include <AwDataClient.h>
#include "Data/AwDataSet.h"
class AwDataManager;

class AwRequestServer : public AwDataClient
{
	Q_OBJECT
public:
	// default constructor
	explicit AwRequestServer(quint16 port = 0, QObject *parent = 0);
	// constructor that spawn a dedicated data server for the specified file. if port is specified, listen on that port.

	~AwRequestServer();
	inline bool isListening() { return m_isListening; }
	inline quint16 serverPort() { return m_serverPort; }
	/** add a request handler **/
	void addHandler(AwRequestServer* const object, void(AwRequestServer::* const mf)(QTcpSocket *, AwScriptProcess*), int request);

	inline void setDebugMode(bool flag) { m_debugMode = flag; }
	void setDataManager(AwDataManager* dm) { m_dataManager = dm; }
	QString& errorString() { return m_errorString; }
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
	
	void initDebugProcess(AwScriptProcess*);
	AwScriptProcess* newDebugProcess();
	AwDataManager* m_dataManager;
	QTcpServer *m_server;
	QMutex m_mutex;
	QThread *m_thread;
	bool m_isListening;
	quint16 m_serverPort;
	int m_pidCounter;
	bool m_debugMode;
	QString m_errorString;
	QList<AwScriptProcess*> m_processes;	// this list will contain instanciated process when plugin_debug option is active

private:
	void handleGetMarkers2(QTcpSocket *client, AwScriptProcess *process);
	void handleGetProperties(QTcpSocket *client, AwScriptProcess *process);
	void handleGetData3(QTcpSocket *client, AwScriptProcess *process);
	void handleGetDataEx(QTcpSocket *client, AwScriptProcess *process);
	void handleGetData2_5_10(QTcpSocket* client, AwScriptProcess* process);
	void handleGetMarkersEx(QTcpSocket *client, AwScriptProcess *process);
	void handleAddMarkers(QTcpSocket *client, AwScriptProcess *process);
	void handleGetPluginInfo(QTcpSocket* client, AwScriptProcess* process);
	void handleGetFileInfo(QTcpSocket *client, AwScriptProcess *process);
	void handleIsTerminated(QTcpSocket *client, AwScriptProcess *process);
	void handleSendMessage(QTcpSocket *client, AwScriptProcess *process);
	void handleSendCommand(QTcpSocket *client, AwScriptProcess *process);
	void handleGetScreenCapture(QTcpSocket *client, AwScriptProcess *process);
	void handleGetICAPanelCapture(QTcpSocket *client, AwScriptProcess *process);
	void handleSetBeamFormer(QTcpSocket *client, AwScriptProcess *process);
	void handleGetTriggers(QTcpSocket *client, AwScriptProcess *process);
	void handleGetPluginIO(QTcpSocket* client, AwScriptProcess* process);
	void handleOpenNewFile(QTcpSocket *client, AwScriptProcess *process);
	void handleRunAnyWave(QTcpSocket *client, AwScriptProcess *process);
	void handleConnectDebug(QTcpSocket* client, AwScriptProcess* process);
	void handleSendMarkers(QTcpSocket* client, AwScriptProcess* process);
	void handleRun(QTcpSocket* client, AwScriptProcess* process);
	void unusedHandler(QTcpSocket *client, AwScriptProcess *process) {}

	void setHandlers();

	QHash<int, std::function<void(QTcpSocket*, AwScriptProcess *)>> m_handlers;
	AwMarkerList m_markers;	// hold the markers added by process
};


#endif