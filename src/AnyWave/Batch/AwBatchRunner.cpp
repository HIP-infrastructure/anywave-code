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

	AwCommandLineManager::instance()->initProcessPDI(process);
	delete AwCommandLineManager::instance();
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
				//if (process) {
				//	auto reader = process->pdi.input.reader();
				//	if (reader)
				//		reader->plugin()->deleteInstance(reader);
				//	process->plugin()->deleteInstance(process);
				//}
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
		//	process->plugin()->deleteInstance(process);
			delete process;
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