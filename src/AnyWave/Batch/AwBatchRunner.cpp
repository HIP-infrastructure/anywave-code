#include "AwBatchRunner.h"
#include "Process/AwProcessManager.h"
#include "Process/AwProcessLogManager.h"
#include "Montage/AwMontageManager.h"
#include "Plugin/AwPluginManager.h"
#include <utils/json.h>
#include <AwFileInfo.h>
#include "CL/AwCommandLineManager.h"
#include <QFile>
#include <AwException.h>
#include <AwCore.h>
#include "Data/AwDataServer.h"

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
		m_items << new AwBatchModelItem(item);
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

AwBaseProcess *AwBatchRunner::createAndInitProcess(QVariantHash& dict, const QString& pluginName)
{
	auto pm = AwPluginManager::getInstance();
	auto plugin = pm->getProcessPluginByName(pluginName);
	
	if (plugin == nullptr) {
		throw AwException(QString("error: %& not found in plugins collection.").arg(pluginName));
		return nullptr;
	}
	auto process = plugin->newInstance();
	process->setPlugin(plugin);
	// quick case: the process does not require an input file but an input dir
	if (!dict.contains("input_file")) {
		if (!dict.contains("input_dir")) {
			throw AwException(QString("error: input_dir not specified."));
			return process;
		}
		process->pdi.input.setArguments(dict);
		return process;
	}

	auto file = dict.value("input_file").toString();
	auto reader = pm->getReaderToOpenFile(file);
	if (reader == nullptr) {
		throw AwException(QString("plugin: %1 file: %2 no reader plugin found.").arg(pluginName).arg(file));
		return process;
	}
	process->pdi.input.setReader(reader);
	if (reader->openFile(file) != AwFileIO::NoError) {
		throw AwException(QString("plugin: %1 file: %2 reader could not open the file.").arg(pluginName).arg(file));
		return process;
	}
	
	AwFileInfo fi(reader, file);
	process->pdi.input.settings[processio::data_dir] = fi.dirPath();
	process->pdi.input.settings[processio::file_duration] = reader->infos.totalDuration();
	process->pdi.input.settings[processio::data_path] = file;
	process->pdi.input.setReader(reader);
	// add extras settings based on input file
	auto badFile = reader->getSideFile(".bad");
	if (QFile::exists(badFile)) {
		dict["bad_file"] = badFile;
		process->pdi.input.settings[processio::bad_labels] = AwMontageManager::loadBad(badFile);
	}
	else
		dict.remove("bad_file");
	// montage step : if a montage file if set, use is a input for the process
	auto mtgFile = reader->getSideFile(".mtg");
	if (QFile::exists(mtgFile))
		dict["montage_file"] = mtgFile;
	else
		dict.remove("montage_file");
	AwChannelList montage;
	if (dict.contains("montage_file")) {
		montage = AwMontageManager::instance()->loadAndApplyMontage(reader->infos.channels(), dict.value("montage_file").toString(),
			process->pdi.input.settings[processio::bad_labels].toStringList());
		if (montage.isEmpty()) {
			sendMessage(QString("error: %1 file could not be applied.").arg(mtgFile));
			montage = AwChannel::duplicateChannels(reader->infos.channels());
		}
	}
	else { // no montage file, use as recorded montage
		montage = AwChannel::duplicateChannels(reader->infos.channels());
	}
	if (!AwCommandLineManager::buildPDI(process, montage, reader->infos.channels())) {
		sendMessage(QString("input channels cannot be set").arg(file));
		reader->plugin()->deleteInstance(reader);
		AW_DESTROY_LIST(montage);
	}
	AW_DESTROY_LIST(montage);
	auto mrkFile = reader->getSideFile(".mrk");
	if (!dict.contains("marker_file"))
		if (QFile::exists(mrkFile))
			dict["marker_file"] = mrkFile;
	if (dict.contains("marker_file"))
		process->pdi.input.setNewMarkers(AwMarker::load(dict.value("marker_file").toString()));
	else
		process->pdi.input.setNewMarkers(reader->infos.blocks().first()->markers(), true);
	// handle skipping markers and/or use specific markers
	QStringList skippedLabels, usedLabels;
	if (dict.contains("skip_markers"))
		skippedLabels = dict.value("skip_markers").toStringList();
	if (dict.contains("use_markers"))
		usedLabels = dict.value("use_markers").toStringList();
	bool skipMarkers = !skippedLabels.isEmpty();
	bool useMarkers = !usedLabels.isEmpty();
	if (skipMarkers || useMarkers) {
		auto markers = AwMarker::applySelectionFilter(process->pdi.input.markers(), skippedLabels, usedLabels,
			process->pdi.input.settings[processio::file_duration].toDouble());
		if (!markers.isEmpty()) {
			process->pdi.input.setNewMarkers(markers);
		}
	}
	// check if we have input markers after all
	 // if no markers set as input => add the GLOBAL ONE
	if (process->pdi.input.markers().isEmpty())
		process->pdi.input.addMarker(new AwMarker("global", 0., process->pdi.input.settings[processio::file_duration].toDouble()));
	AwCommandLineManager::applyFilters(process->pdi.input.channels(), dict);
	return process;
}


void AwBatchRunner::run()
{
	for (auto item : m_items) {
		auto pluginName = item->pluginName();
		QString error;

		auto dict = item->jsonParameters();
		sendMessage(QString("running process %1").arg(pluginName));

		AwBaseProcess *process = nullptr;

		if (item->inputType() == AwBatchModelItem::Directory) {
			dict["input_dir"] = item->inputDir();
			try {
				process = createAndInitProcess(dict, pluginName);
			}
			catch (const AwException& e) {
				sendMessage(e.errorString());
				if (process) {
					process->plugin()->deleteInstance(process);
				}
				continue;
			}
			process->pdi.input.setArguments(dict);
			QObject::connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
			process->runFromCommandLine();
			sendMessage(QString("process %1 has finished").arg(pluginName));
			auto reader = process->pdi.input.reader();
			reader->plugin()->deleteInstance(reader);
			process->plugin()->deleteInstance(process);
		}
		else {
			for (auto file : item->files()) {
				dict["input_file"] = file;
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
					continue;
				}
				process->pdi.input.setArguments(dict);
				auto reader = process->pdi.input.reader();
				auto ds = AwDataServer::getInstance()->duplicate(reader);
				QObject::connect(process, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
				ds->openConnection(process);
				process->runFromCommandLine();
				sendMessage(QString("process %1 has finished").arg(pluginName));
				delete ds;
				// deleting DataServer will also delete the reader instance.
				process->plugin()->deleteInstance(process);
			}
		}
	}
}