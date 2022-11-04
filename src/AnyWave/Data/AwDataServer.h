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
#ifndef AW_DATA_SERVER_H
#define AW_DATA_SERVER_H

class AwDataClient;
class AwFileIO;
class AwFileIOPlugin;
#include <QSemaphore>
#include <QMutex>
#include "AwDataConnection.h"
#include <AwChannel.h>
class AwFilteringOptions;

class AwDataServer : public QObject
{
	Q_OBJECT
public:
	AwDataServer();
	~AwDataServer();
	inline int activeConnections() { return m_dataConnections.size(); }
	static AwDataServer *getInstance();
	static AwDataServer* newInstance();

	void setMainReader(AwFileIO *reader);
	inline AwFileIO *reader() { return m_reader; }
	QSemaphore *getLock() { return m_sem; }
	QMutex* getReadMutex() { return &m_mutex; }
	AwDataServer *duplicate(AwFileIO *reader);
public slots:
	/** Open a connection between a client and the data server thread. If successful, a pointer to the instantiated reader is returned. **/
	void openConnection(AwDataClient *client);
	void closeConnection(AwDataClient *client);
	void closeAllConnections();
	void manageOutOfMemory();
signals:
	void log(const QString& message);
protected:
	QList<AwDataConnection *> m_dataConnections;
	QMap<AwDataClient *, AwDataConnection *> m_clientToConnection;
	AwFileIO *m_reader;
	QSemaphore *m_sem;	// general semaphore for connected clients.
	QMutex m_mutex;
	static AwDataServer *m_instance;
	static int m_instanceCount;
};

#endif