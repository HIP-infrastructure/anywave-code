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
#pragma once

#include "fiffio_global.h"
#include <AwFileIO.h>
#include <QFile>
#include <QDataStream>
#include "fiff_types.h"

class fiffTreeNode 
{  // tree node is similar to a FIFF block and can contain tags.
public:
	explicit fiffTreeNode(int k = 0, fiff_dir_entry_t *d = nullptr, fiffTreeNode *p = nullptr) { dir = d;  parent = p; kind = k; }

	fiffTreeNode *parent;
	int kind;
	fiff_dir_entry_t *dir;
	QHash<int, qint64> tags;
};

typedef struct {
	int startSample;
	int endSample;
	qint64 filePosition;
} data_buffer;

class FIFFIO_EXPORT FIFFIO : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)
public:
	FIFFIO(const QString& fileName);
	~FIFFIO();

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList) override;
	AwFileIO::FileStatus openFile(const QString &path) override;
	AwFileIO::FileStatus canRead(const QString &path) override;

	qint64 writeData(QList<AwChannel *> *channels) override;
	FileStatus createFile(const QString& path, int flags = 0) override;
	bool hasHeadShapeFile() override;
	void cleanUpAndClose() override;
protected:
	void initTag(fiff_tag_t *tag, int kind, int type);
	void writeTag(fiff_tag_t *tag, qint64 pos = 0);
	template<typename T>
	void writeTagData(T data);
	void writeBlockStart(fiff_tag_t *tag, int kind, qint64 pos = 0);
	void writeBlockEnd(fiff_tag_t *tag, int kind, qint64 pos = 0);
	void readTag(fiff_tag_t *tag, qint64 pos = 0);
	QString readTagString(fiff_tag_t *tag);
	void writeTagString(fiff_tag_t *tag, int kind, const QString& string);
	void readFileIDTag();
	fiff_dir_entry_t *addEntry(int kind, int type, int size);
	template<typename T>
	T readTagData();
	template<typename T>
	T* allocateBuffer(int nSamples);
	void buildNodes();
	fiffTreeNode *findBlock(int kind);
	void convert_loc(float oldloc[9], float r0[3], float *ex, float *ey, float *ez);
	void convertOldChinfo(oldChInfoRec *old, fiffChInfoRec *newChinfo);
	int findBuffer(int samplePosition, int left, int right);
	bool checkForFileStart();
	bool checkForCompatibleFile(const QString& path);

	QFile m_file;
	QDataStream m_stream;
	fiffId m_fileID;
	fiff_dir_entry_t *m_dir;
	qint32 m_nchan, m_dataPack, m_firstSample, m_nSamples, m_dataType, m_sampleSize;
	float m_sfreq;
	QList<fiff_dir_entry_t *> m_dirEntries;
	QList<fiffChInfo> m_chInfos;
	QMultiHash<int, fiffTreeNode *> m_blocks; // list of blocks from dirEntries.
	QList<data_buffer *> m_buffers;
	qint64 m_dirPosition; // offset in file where to write the dir entries 
	bool m_containsHeadShape;	// true if the file contains the hpi_result block and dig_point tags.
};

class FIFFIO_EXPORT FIFFIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
public:
	FIFFIOPlugin();
	AW_INSTANTIATE_FILEIO_PLUGIN(FIFFIO)
};

