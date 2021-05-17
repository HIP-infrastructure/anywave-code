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
#include "AwOpenFileDialog.h"
#include "Plugin/AwPluginManager.h"
#include "AwFileIO.h"

AwOpenFileDialog::AwOpenFileDialog(QWidget *parent, const QString& caption, const QString& directory, const QString& filter)
	: QFileDialog(parent, caption, directory, filter)
{
	connect(this, &QFileDialog::directoryEntered, this, &AwOpenFileDialog::directoryCheck);

	QString filterString;
	auto pluginManager = AwPluginManager::getInstance();
	for (auto plugin : pluginManager->readers()) 
		for (auto extension : plugin->fileExtensions) {
			filterString += extension + " ";
			// only check directory extension if the plugin is set to open a directory(bundle).
			if (plugin->flags() & FileIO::IsDirectory)
				m_extensions.append(extension.remove("*"));
		}
	setNameFilter(filterString);
}



void AwOpenFileDialog::directoryCheck(const QString& directory)
{
	if (m_extensions.isEmpty())
		return;
	QDir dir(directory);
	QString name = dir.dirName();
	for (auto ext : m_extensions)
		if (name.endsWith(ext))
			accept();
}