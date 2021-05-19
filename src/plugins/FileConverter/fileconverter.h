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
#ifndef FILECONVERTER_H
#define FILECONVERTER_H

#include "fileconverter_global.h"
#include <AwProcessInterface.h>
#include <AwFileIO.h>
#include "settings.h"
#include <QtCore>

class FILECONVERTER_EXPORT FileConverter : public AwProcess
{
	Q_OBJECT
public:
	FileConverter();
	~FileConverter();

	bool showUi();
	void run();

private:
	QHash<AwFileIOPlugin *, QStringList> m_inputs;
	settings *m_ui;
};

class FILECONVERTER_EXPORT FileConverterPlugin : public AwProcessPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
#endif
	Q_INTERFACES(AwProcessPlugin)
public:
	FileConverterPlugin();
	AW_INSTANTIATE_PROCESS(FileConverter)

};

#endif // FILECONVERTER_H
