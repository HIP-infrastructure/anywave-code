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
#include "AwScriptPlugin.h"
#include <QFile>
#include <QTextStream>
#include <AwKeys.h>
#include "Plugin/AwPluginManager.h"
#include "Data/AwDataManager.h"

void AwScriptPlugin::initProcess(AwScriptProcess *p)
{
	// set it a copy of all settings from data manager
	AwUniteMaps(p->pdi.input.settings, AwDataManager::instance()->settings());
	// merge also settings proper to plugin
	AwUniteMaps(p->pdi.input.settings, m_settings);
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
	version = map.value("version");
	if (version.isEmpty())
		version = "1.0.0"; 	// force a version string

	m_inputFlags = 0;
	if (map.contains("input_flags")) {
		const auto &inputFlagsMap = AwPluginManager::getInstance()->inputFlagsMap();
		QStringList tokens = map.value("input_flags").split(":");
		for (auto t : tokens) {
			auto lowerT = t.toLower();
			if (inputFlagsMap.contains(lowerT))
				m_inputFlags |= inputFlagsMap.value(lowerT);
		}
	}
	m_flags = 0;
	if (map.contains("flags")) {
		const auto &flagsMap = AwPluginManager::getInstance()->flagsMap();
		QStringList tokens = map.value("flags").split(":");
		for (auto t : tokens) {
			auto lowerT = t.toLower();
			if (flagsMap.contains(lowerT))
				m_flags |= flagsMap.value(lowerT);
		}
	}
	m_modifiersFlags = 0;
	if (map.contains("modifiers_flags")) {
		const auto &modsMap = AwPluginManager::getInstance()->modifiersFlagsMap();
		QStringList tokens = map.value("flags").split(":");
		for (auto t : tokens) {
			auto lowerT = t.toLower();
			if (modsMap.contains(lowerT))
				m_modifiersFlags |= modsMap.value(lowerT);
		}
	}
	// add the desc map from desct.txt has values in the plugin settings map
	for (auto const& key : map.keys())
		m_settings.insert(key, map.value(key));
}