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

#include "fiffio_global.h"
#include <AwFileIO.h>
#include <QFile>
#include <QDataStream>
#include "fiff_types.h"


// fiffTreeNode
// node of a tree. The tree has a node for each blocks in the file.
// a block should not be present several times but if it is, nent is incremented.
// A node can contain childrens (sub blocks) and tags.
// A tag can be present several times (ch_info, data_buffer) 
class fiffTreeNode {
public:
	explicit fiffTreeNode(fiff_dir_entry_t *d = nullptr, fiffTreeNode *p = nullptr) { dir = d;  parent = p; }

	void clear() { blocks.clear(); tags.clear(); parent = nullptr; }
	fiffTreeNode *findBlock(int kind);
	QHash<int, fiffTreeNode *> blocks; 
	QMap<int, qint64> tags; // tag refered by kind and value is the position in file
	fiffTreeNode *parent;
	fiff_dir_entry_t *dir;
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

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	AwFileIO::FileStatus openFile(const QString &path);
	AwFileIO::FileStatus canRead(const QString &path);

	//qint64 writeData(QList<AwChannel *> *channels);
	//FileStatus createFile(const QString& path, int flags = 0);
	//FileStatus writeMarkers();

	void cleanUpAndClose();
protected:
	void readTag(fiff_tag_t *tag, qint64 pos = 0);
	void readFileIDTag(fiff_tag_t *tag);
	template<typename T>
	T readTagData();
	template<typename T>
	T* allocateBuffer(int nSamples);
	void buildTree();
	void convert_loc(float oldloc[9], float r0[3], float *ex, float *ey, float *ez);
	void convertOldChinfo(oldChInfoRec *old, fiffChInfoRec *newChinfo);
	int findBuffer(int samplePosition, int left, int right);

	bool checkForFileStart();
	bool checkForCompatibleFile(const QString& path);
	QFile m_file;
	QDataStream m_stream;
	fiffId *m_fileID;
	fiff_dir_entry_t *m_dir;
	qint32 m_nchan, m_dataPack, m_firstSample, m_nSamples, m_dataType, m_sampleSize;
	float m_sfreq;
	QList<fiff_dir_entry_t *> m_dirEntries;
	QList<fiffChInfo> m_chInfos;
	fiffTreeNode m_root;
	QList<fiffTreeNode *> m_children; // list of node inserted in tree as children of root node.
	QList<data_buffer *> m_buffers;
};

class FIFFIO_EXPORT FIFFIOPlugin : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
public:
	FIFFIOPlugin();
	AW_INSTANTIATE_PLUGIN(FIFFIO)
};

