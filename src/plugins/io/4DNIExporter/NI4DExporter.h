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
#ifndef AW_4DNI_EXPORTER_H
#define AW_4DNI_EXPORTER_H
#include "4dniexporter_global.h"
#include <AwProcessInterface.h>
#include <QFile>

class Aw4DNIExporter : public AwProcess
{
	Q_OBJECT
	Q_INTERFACES(AwProcess)
public:
	explicit Aw4DNIExporter();

	void run() override;
	bool showUi() override;
protected:
	void alignFilePointer(QFile&);
	qint64 offsetFilePointer(const QFile& file);

	QString m_outputFileName;
};

class Aw4DNIExporterPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	Aw4DNIExporterPlugin();

	AW_INSTANTIATE_PROCESS(Aw4DNIExporter)
};
#endif