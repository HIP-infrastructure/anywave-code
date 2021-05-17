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
#ifndef MERGEFILES_H
#define MERGEFILES_H

#include "mergefiles_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include <QtCore>
#include "settings.h"

class MERGEFILES_EXPORT MergeFile : public AwProcess
{
	Q_OBJECT
public:
	MergeFile();
	~MergeFile();

	void run();
	bool showUi();

private:
	AwFileIOPlugin *m_adesPlugin;	// ADES plugin to create result file.
	Settings *m_ui;

	void destroyFileReader(AwFileIO *reader);
};

class MERGEFILES_EXPORT MergeFilePlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	MergeFilePlugin();
	AW_INSTANTIATE_PROCESS(MergeFile)

};

#endif // MERGEFILES_H
