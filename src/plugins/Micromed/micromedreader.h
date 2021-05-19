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
#ifndef MICROMEDREADER_H
#define MICROMEDREADER_H

//typedef int boolean;

#include "micromedreader_global.h"
#include "EEG_Structure.h"
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>

class MICROMEDREADER_EXPORT MicromedReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	MicromedReader(const QString& filename = QString());
	~MicromedReader();
	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
private:
	int readHead0();
	int readHead1();
	int readHead2();
	int readHead3();
	int readHead4();
	void setDescriptors();
	float getRealValue(float value, int electrodeIndex);
	int checkHeader();

	QFile m_file;
	QDataStream m_stream;
	int m_type;	// MICROMED HEADER TYPE (2 or 4)

	Micromed_Header_Type_4		m_Head;
	Micromed_New_Code			m_Code[MAX_CAN];
	Micromed_New_Electrode		m_Electrode[MAX_LAB];
	Micromed_New_Annotation		m_Note[MAX_NOTE];
	Micromed_New_Marker_Pair	m_Flag[MAX_FLAG];
	Micromed_New_Event			m_EventA;
	Micromed_New_Event			m_EventB;
	Micromed_New_Trigger		m_Trigger[MAX_TRIGGER];
};


class MICROMEDREADER_EXPORT MicromedReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
	Q_INTERFACES(AwFileIOPlugin)
public:
	MicromedReaderPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(MicromedReader)
};



#endif // MICROMEDREADER_H
