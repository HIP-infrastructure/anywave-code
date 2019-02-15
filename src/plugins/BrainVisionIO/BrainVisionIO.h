/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "brainvisionio_global.h"
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>

class BRAINVISIONIO_EXPORT BrainVisionIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	BrainVisionIO(const QString& filename);

	~BrainVisionIO();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	AwFileIO::FileStatus openFile(const QString &path);
	AwFileIO::FileStatus canRead(const QString &path);

	qint64 writeData(QList<AwChannel *> *channels);
	FileStatus createFile(const QString& path, int flags = 0);
	FileStatus writeMarkers();

	void cleanUpAndClose();

	enum dataType { INT16, UINT16, FLOAT32 };
	enum dataOrientation { Multiplexed, Vectorized };

private:
	QString m_dataFile;
	QString m_markerFile;
	QString m_binFileName;
	QString m_markerPath;
	QString m_markerFileName;
	int m_dataFormat;	//0 = Binary, 1 = ASCII
	int m_dataOrientation;	// 0 = MULTIPLEXED, 1 = VECTORIZED
	int m_dataType;	// 0 = TIMEDOMAIN, 1 = FREQUENCYDOMAIN
	int m_binaryData;	// 0 = FLOAT32, 1 = INT16, 2 = UINT16
	QVector<float> m_scales;	// values by which multiply data stored in channel to get real µV values.
	int m_channelOffset;
	int m_dataOffset;
	float m_sr;	// sampling rate
	QFile m_binaryFile;
	QDataStream m_streamBinary;
	float m_sampleRate;
	void readMarkerFile();
};

class BRAINVISIONIO_EXPORT BrainVisionIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
		Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
public:
	BrainVisionIOPlugin();
	AW_INSTANTIATE_PLUGIN(BrainVisionIO)
};
