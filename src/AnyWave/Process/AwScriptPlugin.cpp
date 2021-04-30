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
#include "AwScriptPlugin.h"
#include <QFile>
#include <QTextStream>
#include <AwKeys.h>
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataManager.h"

void AwScriptPlugin::initProcess(AwScriptProcess *p)
{
	// set it a copy of all settings from data manager
	p->pdi.input.settings.unite(AwDataManager::instance()->settings());
	// merge also settings proper to plugin
	p->pdi.input.settings.unite(m_settings);
	// Fixed input as any channels by default
	if (!(m_flags & Aw::ProcessFlags::ProcessDoesntRequireData)) {
		p->pdi.addInputChannel(-1, 1, 0);
		p->setInputFlags(m_inputFlags);
	}
	p->setPlugin(this);
}

void AwScriptPlugin::init(const QMap<QString, QString>& map)
{
	name = map.value("name");
	description = map.value("description");
	category = map.value("category");
	m_inputFlags = 0;
	if (map.contains("input_flags")) {
		auto inputFlagsMap = AwPluginManager::getInstance()->inputFlagsMap();
		QStringList tokens = map.value("input_flags").split(":");
		for (auto t : tokens) {
			auto lowerT = t.toLower();
			if (inputFlagsMap.contains(lowerT))
				m_inputFlags |= inputFlagsMap.value(lowerT);
		}
	}
	m_flags = 0;
	if (map.contains("flags")) {
		auto flagsMap = AwPluginManager::getInstance()->flagsMap();
		QStringList tokens = map.value("flags").split(":");
		for (auto t : tokens) {
			auto lowerT = t.toLower();
			if (flagsMap.contains(lowerT))
				m_flags |= flagsMap.value(lowerT);
		}
	}
	// add the desc map from desct.txt has values in the plugin settings map
	for (auto const& key : map.keys())
		m_settings.insert(key, map.value(key));
}