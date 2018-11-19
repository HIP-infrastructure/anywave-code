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

#ifndef AW_DATA_SERVER_H
#define AW_DATA_SERVER_H

class AwDataClient;
class AwFileIO;
class AwFileIOPlugin;
#include <QSemaphore>
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

	void setMainReader(AwFileIO *reader);
	inline AwFileIO *reader() { return m_reader; }
	QSemaphore *getLock() { return m_sem; }
	AwDataServer *duplicate(AwFileIO *reader);
public slots:
	/** Open a connection between a client and the data server thread. If successful, a pointer to the instantiated reader is returned. **/
	void openConnection(AwDataClient *client);
	/** Open a connection using a specific File Reader :=> required for scripted task for example **/
	void openConnection(AwDataClient *client, AwFileIO *reader);

	void closeConnection(AwDataClient *client);
	void closeAllConnections();
	void manageOutOfMemory();
protected:
	QList<AwDataConnection *> m_dataConnections;
	QMap<AwDataClient *, AwDataConnection *> m_clientToConnection;
	AwFileIO *m_reader;
	AwFileIOPlugin *m_plugin;
	QSemaphore *m_sem;	// general semaphore for connected clients.
	static AwDataServer *m_instance;
};

#endif