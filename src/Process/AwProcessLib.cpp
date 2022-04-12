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
	m_modifiersFlags = 0;
	m_runMode = 0;
	m_plugin = NULL;
	m_endOfData = m_abort = false;
}

AwBaseProcess::~AwBaseProcess() { 
	
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

int AwBaseProcess::applyUseSkipMarkersKeys()
{
	auto fd = pdi.input.settings.value(keys::file_duration).toFloat();

	// filter markers  considering the optional arguments use_markers and skip_markers
   // init markers based on input.settings arguments (if any)
	QStringList usedMarkers, skippedMarkers;
	bool useMarkers = false, skipMarkers = false;
	bool allDataFlag = false;
	if (pdi.input.settings.contains(keys::use_markers)) {
		usedMarkers = pdi.input.settings.value(keys::use_markers).toStringList();
		// handle special case : if use_markers contains all_data
		// that will force the input to be only one marker marking all the data.
		// other marker flags will be ignored
		if (usedMarkers.first().simplified().toLower() == "all_data") {
			pdi.input.clearMarkers();
			// add a whole marker to the marker list
			pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
			allDataFlag = true;
		}
		else {
			// check that used markers really exist...
			auto labels = AwMarker::getUniqueLabels(pdi.input.markers());
			usedMarkers.erase(std::remove_if(usedMarkers.begin(), usedMarkers.end(), [labels](const QString& l) { return !labels.contains(l); }), usedMarkers.end());
			useMarkers = usedMarkers.size() > 0;
			if (useMarkers)
				pdi.input.settings[keys::use_markers] = usedMarkers;
			else
				pdi.input.settings.remove(keys::use_markers);
		}
	}
	if (pdi.input.settings.contains(keys::skip_markers)) {
		skippedMarkers = pdi.input.settings.value(keys::skip_markers).toStringList();
		auto labels = AwMarker::getUniqueLabels(pdi.input.markers());
		// check that labels in skip markers list really exists.
		skippedMarkers.erase(std::remove_if(skippedMarkers.begin(), skippedMarkers.end(), [labels](const QString& l) { return !labels.contains(l); }), skippedMarkers.end());
		skipMarkers = skippedMarkers.size() > 0;
		if (skipMarkers)
			pdi.input.settings[keys::skip_markers] = skippedMarkers;
		else
			pdi.input.settings.remove(keys::skip_markers);
	}

	if (!allDataFlag) {
		if (skipMarkers || useMarkers) {
			auto markers = AwMarker::duplicate(pdi.input.markers());
			auto inputMarkers = AwMarker::getInputMarkers(markers, skippedMarkers, usedMarkers, fd);
			// Set modified markers !!!
			pdi.input.setModifiedMarkers(markers);
			addModifiers(Aw::ProcessIO::modifiers::UseOrSkipMarkersApplied);
			if (inputMarkers.isEmpty()) {
				pdi.input.clearMarkers();
				pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
			}
			else
				pdi.input.setNewMarkers(inputMarkers);
		}
		return 0;
	}
	return 1;
}

void AwBaseProcess::connectClient(AwDataClient *client)
{
	emit dataConnectionRequested(client);
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

void AwBaseProcess::sendEventAsynch(QSharedPointer<AwEvent> e)
{
	emit sendEvent(e);
	m_lock.lock();
	m_wcEventProcessed.wait(&m_lock);
	m_lock.unlock();
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

	if (m_flags & Aw::ProcessFlags::HasOutputUi) {
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

bool AwProcessPlugin::hasDeclaredArgs()
{
	return m_batchMap.contains("parameters");
}

bool AwProcessPlugin::isBatchGUICompatible()
{
	auto defaults = m_batchMap.value(keys::batch_defaults).toHash();
	auto ui = m_batchMap.value(keys::batch_ui).toHash();
	auto inputs = m_batchMap.value(keys::batch_inputs).toHash();

	bool ok = !defaults.isEmpty() && !ui.isEmpty() && !inputs.isEmpty();

	return m_flags & Aw::ProcessFlags::CanRunFromCommandLine && ok; 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AwProcessDataInterface

QString AwProcessDataInterface::outputDirectory()
{
	return input.settings.value(keys::output_dir).toString();
}

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
