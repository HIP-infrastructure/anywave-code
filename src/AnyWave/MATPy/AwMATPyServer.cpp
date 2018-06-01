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
#include "AwMATPyServer.h"
#include <QMessageBox>
#include <QTcpSocket>
#include <QDir>
#include <QThread>
#include "Debug/AwDebugLog.h"
#include "Marker/AwMarkerManager.h"
#include "Montage/AwMontageManager.h"
#include "AwPidManager.h"

#include "AwRequestServer.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// statics

AwMATPyServer *AwMATPyServer::m_instance = NULL;


AwMATPyServer *AwMATPyServer::instance()
{
	if (!m_instance) {
		// instantiate server and move it to another thread.
		m_instance = new AwMATPyServer();
	}
	return m_instance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AwMATPyServer::AwMATPyServer()
{
	// Get or Create AwPidManager
	AwPidManager::instance()->setParent(this);
	m_rs = NULL;
}

AwMATPyServer::~AwMATPyServer()
{
	if (m_rs)
		delete m_rs;
}

void AwMATPyServer::start()
{
	if (m_rs) // already instantianted
		return;

	m_rs = new AwRequestServer();
	if (!m_rs->isListening()) { // failed to listen on TCP port
		delete m_rs;
		m_rs = NULL;
	}
}	

void AwMATPyServer::stop()
{
	if (m_rs)  {
		delete m_rs;
		m_rs = NULL;
	}
}