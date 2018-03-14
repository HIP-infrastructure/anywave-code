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
#include "AwScript.h"
#if QT_VERSION <= QT_VERSION_CHECK(5,8,0)
#include <QScriptSyntaxCheckResult>
#endif
#include "Montage/AwMontageManager.h"
#include "Process/AwProcessManager.h"
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataServer.h"
#include "AwUtilities.h"
#include "Script/AwScriptProcessFileInput.h"
#include "Script/AwScriptLogManager.h"
#include <AwProcessInterface.h>
#include <QElapsedTimer>
#include <QThread>
#include <QtDebug>

AwScript::AwScript(QSENGINE *engine, const QString& scriptPath, QObject *parent)
	: QObject(parent)
{
	m_engine = engine;
	engine->setParent(this);
	m_scriptPath = scriptPath;
	// adding this object as anywave in engine globals
	QSVALUE value = m_engine->newQObject(this);
	m_engine->globalObject().setProperty("anywave", value);
	m_processFileInput = NULL;
}

AwScript::~AwScript()
{

}


/////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

QSVALUE AwScript::getProcess(QString pluginName)
{
	AwBaseProcess *process = AwProcessManager::instance()->newProcessFromPluginName(pluginName);
	if (process == NULL)
#if QT_VERSION <= QT_VERSION_CHECK(5,8,0)
		return m_engine->nullValue();
#else
		return QJSValue(QJSValue::NullValue);
#endif
	if (!(process->flags() & Aw::ProcessFlags::ProcessIsScriptable))
#if QT_VERSION <= QT_VERSION_CHECK(5,8,0)
		return m_engine->nullValue();
#else
		return QJSValue(QJSValue::NullValue);
#endif

	m_processName = pluginName;
	return m_engine->newQObject(process);
}

void AwScript::run()
{
	QFile file(m_scriptPath);
	if (file.open(QIODevice::ReadOnly|QIODevice::Text))	{
		QString script = file.readAll();
#if QT_VERSION <= QT_VERSION_CHECK(5,8,0)
		if (m_engine->checkSyntax(script).state() == QScriptSyntaxCheckResult::Valid) {
			m_engine->evaluate(script);
			if (m_engine->hasUncaughtException())
				emit error(m_engine->uncaughtException().toString());
		}
#else
		QJSValue result = m_engine->evaluate(script);
		if (result.isError()) {
			emit error(QString("error in %1 at line %2 : %3").arg(m_scriptPath).arg(result.property("lineNumber").toInt()).arg(result.toString()));
			emit finished();
		}
#endif
	}
	emit finished();
}

QSVALUE AwScript::getFileInput()
{
	if (!m_processFileInput)
		m_processFileInput = new AwScriptProcessFileInput(this);

	return m_engine->newQObject(m_processFileInput);
}
  


void AwScript::runProcess(QSVALUE sprocess, QSVALUE fileInput)
{
	QObject *p = sprocess.toQObject();
	QObject *fi = fileInput.toQObject();
	AwPluginManager *pm = AwPluginManager::getInstance();
	AwMontageManager *mm = AwMontageManager::instance();
	QElapsedTimer timer;

	if (p && fi) {
		AwScriptLogManager *slm = AwScriptLogManager::instance();
		slm->connectScript(this);
		AwProcess *process = (AwProcess *)p;
		AwScriptProcessFileInput *finput = (AwScriptProcessFileInput *)fi;
		// parse input files
		finput->parseFiles();
		AwPIElements inputs = finput->inputElements();
		if (inputs.isEmpty()) {
			emit error(tr("No input files for process."));
			return;
		}

		AwFileIO *reader = NULL;
		connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(processMessage(const QString&)));

		foreach(AwPFIElement *ifelem, inputs) {
			QString filePath = ifelem->dataPath();
			reader = pm->getReaderToOpenFile(filePath);
			if (!reader) {
				emit error(QString("No reader found to open the file %1.").arg(filePath));
				continue;
			}
			if (reader->openFile(filePath) != AwFileIO::NoError) {
				emit error(QString("Failed to open the file %1").arg(filePath));
				pm->deleteReaderInstance(reader);
				return;
			}
			emit message(QString("Processing file %1 ...").arg(filePath));
			// apply montage
			if (ifelem->montagePath().isEmpty()) { // No Montage Defined => default montage
				emit warning("No montage file specified. Using default montage.");
				process->pdi.input.channels = AwChannel::duplicateChannels(reader->infos.channels());
			}
			else {
				emit message("Applying montage file " + ifelem->montagePath());
				process->pdi.input.channels = mm->loadAndApplyMontage(reader->infos.channels(), ifelem->montagePath());
			}

			// apply filters
			for (auto c : process->pdi.input.channels) {
				switch (c->type())
				{
				case AwChannel::EEG:
				case AwChannel::SEEG:
					c->setHighFilter(finput->filters().eegHP);
					c->setLowFilter(finput->filters().eegLP);
					break;
				case AwChannel::MEG:
					c->setHighFilter(finput->filters().megHP);
					c->setLowFilter(finput->filters().megLP);
					break;
				case AwChannel::ECG:
				case AwChannel::EMG:
					c->setHighFilter(finput->filters().emgHP);
					c->setLowFilter(finput->filters().emgLP);
					break;
				}
			} // end foreach (AwChannel *c, process->pdi.input.channels)

			if (finput->filters().eegHP || finput->filters().eegLP)
				emit message(QString("EEG/SEEG: Low pass filtering at %1Hz, High pass at %2Hz.").arg(finput->filters().eegLP).arg(finput->filters().eegHP));
			if (finput->filters().megHP || finput->filters().megLP)
				emit message(QString("MEG: Low pass filtering at %1Hz, High pass at %2Hz.").arg(finput->filters().megLP).arg(finput->filters().megHP));
			if (finput->filters().emgHP || finput->filters().emgLP)
				emit message(QString("EMG: Low pass filtering at %1Hz, High pass at %2Hz.").arg(finput->filters().emgLP).arg(finput->filters().emgHP));

			// apply markers (if any)
			if (!ifelem->markerPath().isEmpty()) {
				emit message("Using marker file " + ifelem->markerPath());
				AwMarkerList markers = AwMarker::load(ifelem->markerPath());
				if (!markers.isEmpty()) {
					process->pdi.input.markers = markers;
					bool skipMarkers = !finput->skippedMarkers().isEmpty();
					bool useMarkers = !finput->usedMarkers().isEmpty();
					if (skipMarkers) 
						markers.append(new AwMarker("Global", 0, reader->infos.totalDuration()));
					if (skipMarkers || useMarkers) {
						AwMarkerList filtered = AwMarker::applySelectionFilter(markers, finput->skippedMarkers(), finput->usedMarkers());
						if (filtered.isEmpty()) 
							emit warning("no markers were kept after applying usedMarkers or skippedMarkers filters");
						process->pdi.input.markers = filtered;
						// Destroy loaded markers as applySelection duplicates them.
						while (!markers.isEmpty())
							delete markers.takeFirst();
					}
				}
				else
					emit warning("No markers could be loaded from the marker file.");
			}
			else
				emit warning("No marker file specified.");

			process->pdi.input.dataPath = filePath;
			QFileInfo fi(filePath);
			process->pdi.input.dataFolder = fi.absolutePath();
			process->pdi.input.setReader(reader);
			process->pdi.input.fileDuration = reader->infos.totalDuration();
			// Add the scripted flags before execution
			process->setRunMode(process->runMode() | AwProcessPlugin::Scripted);
			process->init();
			// redirect process output to this object's output.
			connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(message(const QString&)));

			// connect the process as a client of a DataServer thread.
			AwDataServer::getInstance()->openConnection(process, reader);

			emit message("Starting process " + process->plugin()->name);
			// calling run() directly is correct as we are already running in a worker thread here.
			// Calling run() directly will not measure execution time, so do it here explicitly
			timer.restart();
			m_runningProcesses.append(process);
			process->run();

			emit message(QString("Process %1 finished in %2").arg(process->plugin()->name).arg(AwUtilities::hmsTime(timer.elapsed())));
			// process finished
			AwDataServer::getInstance()->closeConnection(process);
			m_runningProcesses.removeAll(process);
			disconnect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(message(const QString&)));

			// clean pdi.input for another usage
			while (!process->pdi.input.channels.isEmpty())
				delete process->pdi.input.channels.takeFirst();
			while (!process->pdi.input.markers.isEmpty())
				delete process->pdi.input.markers.takeFirst();
		} // end foreach (AwInputFile *ifile, inputs)
	} // end if (p && m_taskFileInput)
	else
		emit error(tr("Invalid parameters in function run(process, fileInput)"));

	emit finished();
}