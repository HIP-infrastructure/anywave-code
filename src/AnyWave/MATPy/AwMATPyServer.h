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
#ifndef AWMATPYSERVER_H
#define AWMATPYSERVER_H

class AwRequestServer;
class AwScriptProcess;
class AwDataManager;
#include <QObject>

class AwMATPyServer : public QObject
{
	Q_OBJECT

public:
	static AwMATPyServer *instance();
	static bool isRunning() { return m_instance != nullptr; }
	~AwMATPyServer();

	// instantiate a new server: the instance must be deleted when finished.
	AwMATPyServer* newInstance();
	// start a thread in which the server will listen and connect to the current data server.
	bool start(quint16 port = 0);
	void stop();
	bool isListening();
	quint16 serverPort(); 
	void setDebugMode(bool flag);
	void setDataManager(AwDataManager* dm);

	// removes a duplicated instance from the list and delete it.
	void deleteDuplicatedInstance(AwMATPyServer *instance);
	void addDuplicatedInstance(AwMATPyServer *instance);
signals:
	void log(const QString& message);
	void error(const QString& error);
protected:
	AwMATPyServer();
	AwDataManager* m_dataManager;
	static AwMATPyServer *m_instance;
	AwRequestServer *m_rs;
	// every time an instance is duplicated the reference is stored here. All instances will be deleted when singleton instance is deleted.
	QList<AwMATPyServer *> m_duplicatedInstances;
};

#endif // AWMATLABSERVER_H
