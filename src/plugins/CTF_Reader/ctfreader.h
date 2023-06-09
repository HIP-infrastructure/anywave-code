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
#ifndef CTFREADER_H
#define CTFREADER_H

#include "ctfreader_global.h"

#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>

typedef struct 
{
	double x;
	double y;
	double z;
	double ox;
	double oy;
	double oz;
} coil;

#define MAX_COILS 8
#define SENSOR_LABEL 31
#define MAX_NUM_COEFS 50
#define MAX_AVERAGE_BINS 8
#define MAX_BALANCING MAX_NUM_COEFS
#define GENERALRESID 30000
#define G1BRINDEX 1 /* Define index for the coefficients. */
#define G2BRINDEX 2
#define G3BRINDEX 3
#define G2OIINDEX 4
#define G3OIINDEX 5
#define EDDYINDEX 6
#define G1OIINDEX 7 

typedef struct
{
	quint16 index;
	double proper_gain; // sensor gain in Phi/Tesla
	double q_gain;		// qxx gain (usually 2^20 for Q20)
	double io_gain;		// i/o gain of special sensors (usually 1.0)
	double io_offset;
	qint16 numCoils;
	qint16 grad_order_no;
	coil coils[MAX_COILS];
	coil hcoils[MAX_COILS];
} sensor_info;

class CTFREADER_EXPORT CTFFileReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	CTFFileReader(const QString& filename = QString()); 
	~CTFFileReader() { cleanUpAndClose(); }

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
	AwChannelList triggerChannels();
public slots:
	void writeTriggerChannel(AwMarkerList&) {}
private:
	QFile m_file;
	QDataStream m_stream;
	AwChannelList m_triggers;
	QString m_res4Path;
	QString m_sensorFileName;
	double m_sampleRate;
	QList<sensor_info *> m_sensors_info;
	qint16 m_nbTrials;
	qint32 m_samplesByTrial;
	qint32 m_dataSize;
	QStringList getFiles(const QString& path);
};

class CTFREADER_EXPORT CTFReader : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
public:
	CTFReader();
	AW_INSTANTIATE_FILEIO_PLUGIN(CTFFileReader)
};




#endif // CTFREADER_H
