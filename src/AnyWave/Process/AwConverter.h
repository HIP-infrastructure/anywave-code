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
#ifndef AW_CONVERTER_H
#define AW_CONVERTER_H

#include <AwProcessInterface.h>
#include <QtCore>
#include <QDataStream>
#include <AwFileIO.h>

class AwConverter : public AwProcess
{
	Q_OBJECT
	Q_INTERFACES(AwProcess)

public:
	AwConverter();
	~AwConverter();
	void setPath(const QString& path) { m_path = path; }
	void setWriter(AwFileIO *writer ) { m_writer = writer; }
	void setMarkers(const AwMarkerList& markers) { m_markers = markers; }

	void run();

private:
	QString m_path;
	AwFileIO *m_writer;
	QList<AwMarker *> m_markers;	// copy of all markers
};

class AwConverterPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwConverterPlugin();

	AwConverter *newInstance() { return new AwConverter(); }
};

#endif