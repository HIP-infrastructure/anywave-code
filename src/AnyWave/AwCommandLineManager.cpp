#include "AwCommandLineManager.h"
#include "Plugin/AwPluginManager.h"
#include "Process/AwProcessManager.h"
#include <AwException.h>
#include <QTextStream>
#include "Data/AwDataServer.h"

void AwCommandLineManager::computeICA(AwArguments& arguments)
{	
	auto pm = AwPluginManager::getInstance();
	auto ica_plugin = pm->getProcessPluginByName("ICA extraction");
	auto file = arguments["ICA_File"].toString();
	QString origin = "AwCommandLineManager::computeICA";

	if (file.isEmpty()) {
		throw AwException(QString("Could not open the file %1").arg(file), origin);
		return;
	}

	if (ica_plugin) {
		auto *reader = pm->getReaderToOpenFile(file);
		if (reader == NULL) {
			throw AwException(QString("No reader found for file %1").arg(file), origin);
			return;
		}
		if (reader->openFile(file) != AwFileIO::NoError) {
			throw AwException(QString("Could not open the file %1").arg(file), origin);
			return;
		}
		auto markerFile = QString("%1.mrk").arg(file);
		auto process = ica_plugin->newInstance();
		auto process_manager = AwProcessManager::instance();
		process->pdi.input.channels = reader->infos.channels();
		process->pdi.input.markers = AwMarker::load(markerFile);
		// add bad channels to arguments
		auto badFile = QString("%1.bad").arg(file);
		QStringList badLabels;
		QFile tmp(badFile);
		if (tmp.open(QIODevice::Text | QIODevice::ReadOnly)) {
			QTextStream stream(&tmp);
			while (!stream.atEnd())
				badLabels << stream.readLine();
			tmp.close();
		}
		if (!badLabels.isEmpty())
			arguments["bad_labels"] = badLabels;

		process->pdi.input.setArguments(arguments);
		// DONT FORGET TO SET DATAPATH !
		process->pdi.input.dataPath = file;
		//process_manager->runProcess(process);
		AwDataServer::getInstance()->setMainReader(reader);
		AwDataServer::getInstance()->openConnection(process);
		process->runFromCommandLine();
		ica_plugin->deleteInstance(process);
		reader->plugin()->deleteInstance(reader);
	}
}