/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwProcessLib.h"
#include <QWidget>
#include <QtDebug>
#include <AwProcessInterface.h>
#include <QThread>
#include <QTranslator>
#include <QApplication>
#include <AwKeys.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////:
// AwProcess

AwBaseProcess::AwBaseProcess() : AwDataClient(0) 
{
	m_flags = 0x00000000; 
	m_inputFlags = 0x000000;
	m_runMode = 0;
	m_plugin = NULL;
	m_endOfData = m_abort = false;
}

AwBaseProcess::~AwBaseProcess() { 
	qApp->removeTranslator(&m_translator); 
}


void AwBaseProcess::abort() 
{
	QMutexLocker locker(&m_lock);
	m_abort = true;
}

bool AwBaseProcess::isAborted()
{
	QMutexLocker locker(&m_lock);
	return m_abort;
}

void AwBaseProcess::connectClient(AwDataClient *client)
{
	emit dataConnectionRequested(client);
}

///
/// loadLanguage()
/// try to load the language file based on the prefix file provided by the plugin
/// and the current selected language.
void AwBaseProcess::loadLanguage()
{
	QString file = QString("%1_%2.qm").arg(m_langFilePrefix).arg(m_locale);
	if (m_translator.load((file)))
		qApp->installTranslator(&m_translator);
}

void AwBaseProcess::addMarkers(AwMarkerList *markers)
{
	emit sendMarkers(markers);
	m_mutexMarkersReceived.lock();
 	m_wcMarkersReceived.wait(&m_mutexMarkersReceived);
	m_mutexMarkersReceived.unlock();
}

void AwBaseProcess::addMarker(AwMarker *marker)
{
	emit sendMarker(marker);
	m_mutexMarkersReceived.lock();
 	m_wcMarkersReceived.wait(&m_mutexMarkersReceived);
	m_mutexMarkersReceived.unlock();
}

void AwBaseProcess::setMarkersReceived()
{
	m_wcMarkersReceived.wakeAll();
}

AwProcess::AwProcess() : AwBaseProcess()
{
	m_executionTime = 0;
	m_status = 0;
}

void AwProcess::start()
{
	m_status = AwProcess::Running;
	emit started();
	if (m_executionTime == 0)
		m_timer.start();
	else	{
		m_executionTime = 0;
		m_timer.restart();
	}
	run();
	m_executionTime = m_timer.elapsed();
	if (wasAborted())	{
		emit aborted();
		return;
	}

	if (m_runMode == AwProcessPlugin::Display)	{
		m_status = AwProcess::Idle;	
		emit idle();
		return;
	}

	if (m_flags & Aw::ProcessFlags::ProcessHasOutputUi) {
		m_status = AwProcess::Idle;	
		emit idle();
		return;
	}

	m_status = AwProcess::Finished;	
	emit finished();
}

void AwProcess::stop()
{
	// try to interrupt process execution (if running)
	if (isRunning()) {
		m_status = AwProcess::Aborted;
		m_endOfData = true;
	}
	else if (isIdle())	{
		// process was idle, set it to finished
		m_status = AwProcess::Finished;
		emit finished();
	}
	else if (isFinished())	{
		// should not happen. Can't stop a process that is already finished.
	}
}

void AwGUIProcess::registerGUIWidget(AwProcessGUIWidget *widget)
{
	AwProcessGUIWidget *w = static_cast<AwProcessGUIWidget *>(widget);
	if (w == NULL)
		return;
	connect(w, SIGNAL(closed()), this, SLOT(stop()));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AwProcessPlugin

void AwProcessPlugin::addLanguageTranslation(const QString& resourceFile)
{
	QTranslator *translator = new QTranslator;
	if (translator->load(resourceFile))
		qApp->installTranslator(translator);
	else
		delete translator;
}


bool AwProcessPlugin::hasDeclaredArgs()
{
	return m_batchHash.contains("parameters");
}

bool AwProcessPlugin::isBatchGUICompatible()
{
	auto defaults = m_batchHash.value(cl::batch_defaults).toHash();
	auto ui = m_batchHash.value(cl::batch_ui).toHash();
	auto inputs = m_batchHash.value(cl::batch_inputs).toHash();

	bool ok = !defaults.isEmpty() && !ui.isEmpty() && !inputs.isEmpty();

	return m_flags & Aw::ProcessFlags::CanRunFromCommandLine && ok; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AwProcessDataInterface

void AwProcessDataInterface::addInputChannel(int type, int min, int max)
{
	m_inputChannels[type] = QPair<int, int>(min, max);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// builtin proces
AwBuiltInProcess::AwBuiltInProcess(AwBuiltInPlugin *plugin)
{
	m_plugin = plugin;
	plugin->setParent(this);
}
