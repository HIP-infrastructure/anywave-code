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
#ifndef FIFF_READER_H
#define FIFF_READER_H

#include "fiff_reader_global.h"
#include <AwFileIO.h>
#include "fiff.h"
#include <QtGlobal>


typedef struct {
	fiffDirEntry ent;
	int first_sample;
	int last_sample;
	int nsamp;
	int pos;	// position in file
} data_buffer;

class FIFF_READER_EXPORT FIFF_Reader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	FIFF_Reader(const QString& fileName);
	~FIFF_Reader()  { cleanUpAndClose(); }

	qint64 readDataFromChannels(float start, float duration, AwChannelList &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	void cleanUpAndClose();
private:
	int readTag(int kind, fiffTag tag);
	bool check_entry(int n, int nent);
	QStringList readBadLabels();
	AwChannelList readChInfos();
	void readEvents();

	fiffFile m_fiffFile;
	QList<fiffChInfo> m_chanInfos;
	int m_sampleOffset;
	QList<data_buffer *> m_buffers;
	int m_firstSample, m_lastSample;
	float m_sampleRate;
	fiffCoordTrans m_coord;	// coord transformation from device to head. If NULL => no transformation available
};


class FIFF_READER_EXPORT FIFF_ReaderPlugin : public AwFileIOPlugin
{
	Q_OBJECT
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
	Q_INTERFACES(AwFileIOPlugin)
public:
	FIFF_ReaderPlugin();
	FIFF_Reader *newInstance(const QString& filename) { return new FIFF_Reader(filename); }
};
#endif // FIFF_READER_H
