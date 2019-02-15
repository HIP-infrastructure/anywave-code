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
	}

	auto process = plugin->newInstance();
	if (!inputFile.isEmpty()) {
		// detect optional anywave files (.mrk, .mtg, .bad)
		QString tmp = QString("%1.mrk").arg(inputFile);
		if (QFile::exists(tmp))
			args["marker_file"] = tmp;
		tmp = QString("%1.mtg").arg(inputFile);
		if (QFile::exists(tmp))
			args["montage_file"] = tmp;
		tmp = QString("%1.bad").arg(inputFile);
		if (QFile::exists(tmp)) {
			args["bad_file"] = tmp;
			process->pdi.input.badLabels = AwMontageManager::loadBad(tmp);
		}

		float hp = 0., lp = 0., notch = 0.;
		process->pdi.input.dataPath = inputFile;
		process->pdi.input.setReader(reader);
		process->pdi.input.setNewChannels(reader->infos.channels(), true);
		process->pdi.input.setNewMarkers(reader->infos.blocks().first()->markers(), true);
		// check for optional filter settings
		if (args.contains("hp"))
			hp = args["hp"].toDouble();
		if (args.contains("lp"))
			lp = args["lp"].toDouble();
		if (args.contains("notch"))
			notch = args["notch"].toDouble();
		if (lp || notch || hp) {
			for (auto c : process->pdi.input.channels()) {
				c->setLowFilter(lp);
				c->setHighFilter(hp);
				c->setNotch(notch);
			}
		}
		AwDataServer::getInstance()->setMainReader(reader);
		AwDataServer::getInstance()->openConnection(process);
	}
	process->pdi.input.readers = pm->readers();
	process->pdi.input.writers = pm->writers();
	return process;
}