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
#ifndef AWMATPYSERVER_H
#define AWMATPYSERVER_H

class AwRequestServer;
class AwScriptProcess;
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
//	bool start(const QString& filePath, AwScriptProcess *process, quint16 port = 0);
	void stop();
	bool isListening();
	quint16 serverPort(); 

	// removes a duplicated instance from the list and delete it.
	void deleteDuplicatedInstance(AwMATPyServer *instance);
	void addDuplicatedInstance(AwMATPyServer *instance);
signals:
	void log(const QString& message);
	void error(const QString& error);
protected:
	AwMATPyServer();

	static AwMATPyServer *m_instance;
	AwRequestServer *m_rs;
	// every time an instance is duplicated the reference is stored here. All instances will be deleted when singleton instance is deleted.
	QList<AwMATPyServer *> m_duplicatedInstances;
};

#endif // AWMATLABSERVER_H
