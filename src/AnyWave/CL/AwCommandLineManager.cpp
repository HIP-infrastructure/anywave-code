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
#include "AwCommandLineManager.h"
#include "Plugin/AwPluginManager.h"
#include "Process/AwProcessManager.h"
#include <AwException.h>
#include "Montage/AwMontageManager.h"
#include "Data/AwDataServer.h"
#include <AwMontage.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <AwUtilities.h>
#include "Prefs/AwSettings.h"

void AwCommandLineManager::applyFilters(const AwChannelList& channels, const AwArguments& args)
{
	float hp = 0., lp = 0., notch = 0.;
	// check for optional filter settings
	if (args.contains("hp"))
		hp = args["hp"].toDouble();
	if (args.contains("lp"))
		lp = args["lp"].toDouble();
	if (args.contains("notch"))
		notch = args["notch"].toDouble();
	if (lp || notch || hp) {
		for (auto c : channels) {
			c->setLowFilter(lp);
			c->setHighFilter(hp);
			c->setNotch(notch);
		}
	}
}

AwBaseProcess *AwCommandLineManager::createAndInitNewProcess(const QString& pluginName, AwArguments& args, const QString& inputFile)
{
	auto pm = AwPluginManager::getInstance();
	auto plugin = pm->getProcessPluginByName(pluginName);
	const QString origin = "AwCommandLineManager::createNewProcess()";
	if (plugin == Q_NULLPTR) {
		throw AwException(QString("No plugin named %1 found.").arg(pluginName), origin);
		return Q_NULLPTR;
	}

	AwFileIO *reader = Q_NULLPTR;
	bool doNotRequiresData = plugin->flags() & Aw::ProcessFlags::ProcessDoesntRequireData;

	if (!doNotRequiresData && inputFile.isEmpty()) {
		throw AwException(QString("input_file must be specified."), origin);
		return Q_NULLPTR;
	}
	auto process = plugin->newInstance();
	process->setPlugin(plugin);

	if (!inputFile.isEmpty()) {
		reader = pm->getReaderToOpenFile(inputFile);
		if (reader == Q_NULLPTR) {
			throw AwException(QString("no reader can open %1").arg(inputFile), origin);
			return Q_NULLPTR;
		}
		if (reader->openFile(inputFile) != AwFileIO::NoError) {
			throw AwException(QString("Could not open %1").arg(inputFile), origin);
			reader->plugin()->deleteInstance(reader);
			return Q_NULLPTR;
		}
		AwFileInfo fi(reader, inputFile);
		process->pdi.input.dataFolder = fi.dirPath();
		process->pdi.input.fileDuration = reader->infos.totalDuration();
		process->pdi.input.badLabels = AwMontageManager::instance()->badLabels();
		process->pdi.input.dataPath = inputFile;
		process->pdi.input.setReader(reader);
	}
	// get arguments (could be a json file path or a json string)
	QString p_args = args["process_args"].toString();
	if (!p_args.isEmpty()) {
		// check for json file
		QJsonDocument doc;
		if (QFile::exists(p_args)) {
			doc = AwUtilities::readJsonFile(p_args);
			if (doc.isEmpty() || doc.isNull()) {
				throw AwException("json file is invalid.", origin);
				return Q_NULLPTR;
			}
		}
		else {
			doc = QJsonDocument::fromJson(p_args.toUtf8());
		}
		args.unite(doc.object().toVariantHash());
		// add the json string also for MATLAB/Python plugin
		args["json_args"] = QString(doc.toJson());

		// SPECIAL CASE for marker_file or montage_file.
		// if those arguments are specified within the json file, handle them as relative to the data file folder, not as an absolute file path.
		if (args.contains("marker_file")) {
			QString fullPath = QString("%1/%2").arg(process->pdi.input.dataFolder).arg(args["marker_file"].toString());
			args["marker_file"] = fullPath;
		}
		if (args.contains("montage_file")) {
			QString fullPath = QString("%1/%2").arg(process->pdi.input.dataFolder).arg(args["montage_file"].toString());
			args["montage_file"] = fullPath;
		}
	}

	if (!inputFile.isEmpty()) {
		// detect optional anywave files (.mrk, .mtg, .bad)
		
		QString tmp = QString("%1.mrk").arg(inputFile);
		// detect only if marker_file option is not specified by the user
		if (!args.contains("marker_file")) 
			if (QFile::exists(tmp))
				args["marker_file"] = tmp;
		// detect only if montager_file option is not specified by the user
		tmp = QString("%1.mtg").arg(inputFile);
		if (!args.contains("montage_file"))
			if (QFile::exists(tmp))
				args["montage_file"] = tmp;
		tmp = QString("%1.bad").arg(inputFile);
		if (QFile::exists(tmp)) {
			args["bad_file"] = tmp;
			process->pdi.input.badLabels = AwMontageManager::loadBad(tmp);
		}

		// if marker file is found => load markers and use them for the process
		if (args.contains("marker_file"))
			process->pdi.input.setNewMarkers(AwMarker::load(args["marker_file"].toString()));
		else
			process->pdi.input.setNewMarkers(reader->infos.blocks().first()->markers(), true);
		
		try {
			// same thing for montage
			if (args.contains("montage_file")) {
				auto channels = AwMontageManager::instance()->loadAndApplyMontage(reader->infos.channels(), args["montage_file"].toString(), process->pdi.input.badLabels);
				if (!channels.isEmpty())
					process->pdi.input.setNewChannels(channels);
				else 
					process->pdi.input.setNewChannels(reader->infos.channels(), true);
			}
			else
				process->pdi.input.setNewChannels(reader->infos.channels(), true);
		}
		catch (const AwException& e) {
			throw e;
			return Q_NULLPTR;
		}
		//process->pdi.input.dataFolder = AwSettings::getInstance()->fileInfo()->dirPath();
		//process->pdi.input.fileDuration = reader->infos.totalDuration();
		//process->pdi.input.badLabels = AwMontageManager::instance()->badLabels();
		//process->pdi.input.dataPath = inputFile;
		process->pdi.input.setReader(reader);
		AwCommandLineManager::applyFilters(process->pdi.input.channels(), args);
		// We can here change the reader for the main DataServer as the running mode is command line and AnyWave will close after finished.
		AwDataServer::getInstance()->setMainReader(reader);
		AwDataServer::getInstance()->openConnection(process);
	}
	process->pdi.input.readers = pm->readers();
	process->pdi.input.writers = pm->writers();

	return process;
}