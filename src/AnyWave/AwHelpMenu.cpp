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

#include "AnyWave.h"
#include "Plugin/AwPluginManager.h"
#include <AwProcessInterface.h>


/// <summary>
/// init help menu sections for plugins
/// The plugin must provide a helpUrl string.
/// </summary>
void AnyWave::initPluginsHelpMenu()
{
	QStringList sections;
	QList<QMenu*> menus; 
	menuHelp->clear();

	QMenu* menuPluginHelp = nullptr;
	for (auto plugin : AwPluginManager::getInstance()->processes()) {
		if (plugin->helpUrl().isEmpty())
			continue;
		// we've got a help url
		if (menuPluginHelp == nullptr)
			menuPluginHelp = menuHelp->addMenu("Plugins");

		QString helpUrl = plugin->helpUrl();
		QStringList tokens = helpUrl.split("::");
		QAction* action = nullptr;
		// several cases: no : character => url help is provided without sub menu or caption format.
		if (tokens.size() == 1) { // ation text default to plugin name
			action = menuPluginHelp->addAction(plugin->name, this, &AnyWave::openPluginHelpUrl);
			action->setData(plugin->helpUrl());
			continue;
		}
		else if (tokens.size() == 2) { // a caption text is provided 
			QString caption = tokens.first();
			if (caption.isEmpty())
				caption = plugin->name;
			action = menuPluginHelp->addAction(caption, this, &AnyWave::openPluginHelpUrl);
			action->setData(tokens.last());
		}
		else if (tokens.size() == 3) { // a caption and a sub menu is provided
			QString caption = tokens.first();
			if (caption.isEmpty())
				caption = plugin->name;
			QString subMenu = tokens.at(1);
			if (!subMenu.isEmpty()) {
				int index = sections.indexOf(subMenu.toLower().simplified());
				QMenu* menu = nullptr;
				if (index == -1) {
					sections.append(subMenu.toLower().simplified());
					menus.append(menuPluginHelp->addMenu(subMenu));
					menu = menus.last();
				}
				else 
					menu = menus.at(index);
				action = menu->addAction(caption, this, &AnyWave::openPluginHelpUrl);
				action->setData(tokens.last());
			}
		}
	}
}