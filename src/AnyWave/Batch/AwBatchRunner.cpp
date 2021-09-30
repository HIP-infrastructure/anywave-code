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
#include "AwBatchRunner.h"
#include "Process/AwProcessManager.h"
#include "Process/AwProcessLogManager.h"
#include "Montage/AwMontageManager.h"
#include "Plugin/AwPluginManager.h"
#include <utils/json.h>
#include "CL/AwCommandLineManager.h"
#include <QFile>
#include <AwException.h>
#include <AwCore.h>
#include "Data/AwDataServer.h"
#include "Data/AwDataManager.h"
#include <AwKeys.h>

AwBatchRunnerPlugin::AwBatchRunnerPlugin()
{
	name = "Batch Runner";
	description = "execute process in batch.";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData);
}

AwBatchRunner::AwBatchRunner(AwBuiltInPlugin *plugin, const AwBatchItems& items) : AwBuiltInProcess(plugin)
{
	// duplicate items
	for (auto item : items)
		m_items << new AwBatchItem(item);
	auto plm = AwProcessLogManager::instance();
	plm->connectProcess(this);
}

AwBatchRunner::~AwBatchRunner()
{
	AW_DESTROY_LIST(m_items);
}

void AwBatchRunner::init()
{
	 
}

AwBaseProcess *AwBatchRunner::createAndInitProcess(QVariantMap& dict, const QString& pluginName)
{
	auto pm = AwPluginManager::getInstance();
	auto plugin = pm->getProcessPluginByName(pluginName);
	
	if (plugin == nullptr) {
		throw AwException(QString("error: %& not found in plugins collection.").arg(pluginName));
		return nullptr;
	}
	auto process = plugin->newInstance();
	process->setPlugin(plugin);

	AwCommandLineManager::initProcessPDI(process);


	//bool doNotRequiresData = plugin->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;
	//if (!doNotRequiresData && !dict.contains(keys::input_file)) {
	//	throw AwException(QString("error: input_dir not specified."));
	//	return process;
	//}
	//process->pdi.input.settings.unite(dict);
	//if (dict.contains(keys::input_file)) {
	//	auto file = dict.value(keys::input_file).toString();
	//	auto reader = pm->getReaderToOpenFile(file);
	//	if (reader == nullptr) {
	//		throw AwException(QString("plugin: %1 file: %2 no reader plugin found.").arg(pluginName).arg(file));
	//		return process;
	//	}
	//	process->pdi.input.setReader(reader);
	//	if (reader->openFile(file) != AwFileIO::NoError) {
	//		throw AwException(QString("plugin: %1 file: %2 reader could not open the file.").arg(pluginName).arg(file));
	//		return process;
	//	}
	//	auto dm = AwDataManager::instance();
	//	process->pdi.input.settings[keys::data_dir] = dm->dataDir();
	//	process->pdi.input.settings[keys::file_duration] = dm->totalDuration();
	//	process->pdi.input.settings[keys::data_path] = file;
	//	process->pdi.input.setReader(reader);
	//	// add extras settings based on input file
	//	auto badFile = reader->infos.badFile();
	//	if (QFile::exists(badFile)) {
	//		dict["bad_file"] = badFile;
	//		process->pdi.input.settings[keys::bad_labels] = AwMontageManager::loadBad(badFile);
	//	}
	//	else
	//		dict.remove("bad_file");
	//	// montage step : if a montage file if set, use is a input for the process
	//	auto mtgFile = reader->infos.mtgFile();
	//	if (QFile::exists(mtgFile))
	//		dict["montage_file"] = mtgFile;
	//	else
	//		dict.remove("montage_file");
	//	AwChannelList montage;
	//	if (dict.contains("montage_file")) {
	//		montage = AwMontageManager::instance()->loadAndApplyMontage(reader->infos.channels(), dict.value("montage_file").toString(),
	//			process->pdi.input.settings[keys::bad_labels].toStringList());
	//		if (montage.isEmpty()) {
	//			sendMessage(QString("error: %1 file could not be applied.").arg(mtgFile));
	//			montage = AwChannel::duplicateChannels(reader->infos.channels());
	//		}
	//	}
	//	else { // no montage file, use as recorded montage
	//		montage = AwChannel::duplicateChannels(reader->infos.channels());
	//	}
	//	if (!AwCommandLineManager::buildPDI(process, montage, reader->infos.channels())) {
	//		sendMessage(QString("input channels cannot be set").arg(file));
	//		reader->plugin()->deleteInstance(reader);
	//		AW_DESTROY_LIST(montage);
	//	}
	//	AW_DESTROY_LIST(montage);
	//	auto mrkFile = reader->infos.mrkFile();
	//	if (!dict.contains("marker_file"))
	//		if (QFile::exists(mrkFile))
	//			dict["marker_file"] = mrkFile;
	//	if (dict.contains("marker_file"))
	//		process->pdi.input.setNewMarkers(AwMarker::load(dict.value("marker_file").toString()));
	//	else
	//		process->pdi.input.setNewMarkers(reader->infos.blocks().first()->markers(), true);
	//	// handle skipping markers and/or use specific markers
	//	QStringList skippedLabels, usedLabels;
	//	if (dict.contains("skip_markers"))
	//		skippedLabels = dict.value("skip_markers").toStringList();
	//	if (dict.contains("use_markers"))
	//		usedLabels = dict.value("use_markers").toStringList();
	//	bool skipMarkers = !skippedLabels.isEmpty();
	//	bool useMarkers = !usedLabels.isEmpty();
	//	if (skipMarkers || useMarkers) {
	//		auto markers = AwMarker::applySelectionFilter(process->pdi.input.markers(), skippedLabels, usedLabels,
	//			process->pdi.input.settings.value(keys::file_duration).toFloat());
	//		if (!markers.isEmpty()) {
	//			process->pdi.input.setNewMarkers(markers);
	//		}
	//	}
	//	// check if we have input markers after all
	//	 // if no markers set as input => add the GLOBAL ONE
	//	if (process->pdi.input.markers().isEmpty())
	//		process->pdi.input.addMarker(new AwMarker("global", 0., process->pdi.input.settings.value(keys::file_duration).toFloat()));
	//	AwCommandLineManager::applyFilters(process->pdi.input.channels(), dict);
	//}
	return process;
}


void AwBatchRunner::run()
{
	for (auto item : m_items) {
		auto pluginName = item->pluginName();
		QString error;

		auto dict = item->params();

		AwBaseProcess *process = nullptr;
		auto inputs = item->inputs();
		auto outputs = item->outputs(); // may be empty

		// input keys must have the same number of items.
		// output keys are optional. But if there are output keys, their number must be equal to the number of inputs. 

		auto input_keys = inputs.keys();
		// we assume all input keys have the same number of files and this is also the case for output keys.

		int input_file_index = 0;
		for (int i = 0; i < inputs.value(input_keys.first()).size(); i++) {
			for (auto k : input_keys) 
				dict[k] = inputs.value(k).at(input_file_index);
			for (auto k : outputs.keys())
				dict[k] = outputs.value(k).at(input_file_index);
			try {
				process = createAndInitProcess(dict, pluginName);
			}
			catch (const AwException& e) {
				sendMessage(e.errorString());
				if (process) {
					auto reader = process->pdi.input.reader();
					if (reader)
						reader->plugin()->deleteInstance(reader);
					process->plugin()->deleteInstance(process);
				}
				input_file_index++;
				continue;
			}
			sendMessage(QString("running process %1 on file(s):\n").arg(pluginName));
			for (auto k : input_keys) 
				sendMessage(QString("%1\n").arg(dict.value(k).toString()));

		//	process->pdi.input.setArguments(dict);
		//	process->pdi.input.settings.unite(dict);
			AwUniteMaps(process->pdi.input.settings, dict);
			auto reader = process->pdi.input.reader();
			QObject::connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
			// if the process has a reader => input_file set and the process must connect to a data server using the specified reader.
			if (reader) {
				auto ds = AwDataServer::getInstance()->duplicate(reader);
				ds->openConnection(process);
				process->runFromCommandLine();
				delete ds;
			}
			else {
				process->runFromCommandLine();
			}
			sendMessage(QString("process %1 has finished").arg(pluginName));
			process->plugin()->deleteInstance(process);
			input_file_index++;
		} 
	}

	//	if (item->inputType() == AwBatchModelItem::Directory) {
	//		dict["input_dir"] = item->inputDir();
	//		try {
	//			process = createAndInitProcess(dict, pluginName);
	//		}
	//		catch (const AwException& e) {
	//			sendMessage(e.errorString());
	//			if (process) {
	//				process->plugin()->deleteInstance(process);
	//			}
	//			continue;
	//		}
	//		process->pdi.input.setArguments(dict);
	//		QObject::connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
	//		process->runFromCommandLine();
	//		sendMessage(QString("process %1 has finished").arg(pluginName));
	//		auto reader = process->pdi.input.reader();
	//		reader->plugin()->deleteInstance(reader);
	//		process->plugin()->deleteInstance(process);
	//	}
	//	else {
	//		for (auto file : item->files()) {
	//			dict["input_file"] = file;
	//			try {
	//				process = createAndInitProcess(dict, pluginName);
	//			}
	//			catch (const AwException& e) {
	//				sendMessage(e.errorString());
	//				if (process) {
	//					auto reader = process->pdi.input.reader();
	//					if (reader)
	//						reader->plugin()->deleteInstance(reader);
	//					process->plugin()->deleteInstance(process);
	//				}
	//				continue;
	//			}
	//			process->pdi.input.setArguments(dict);
	//			auto reader = process->pdi.input.reader();
	//			auto ds = AwDataServer::getInstance()->duplicate(reader);
	//			QObject::connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
	//			ds->openConnection(process);
	//			process->runFromCommandLine();
	//			sendMessage(QString("process %1 has finished").arg(pluginName));
	//			delete ds;
	//			// deleting DataServer will also delete the reader instance.
	//			process->plugin()->deleteInstance(process);
	//		}
	//	}
	//}
}