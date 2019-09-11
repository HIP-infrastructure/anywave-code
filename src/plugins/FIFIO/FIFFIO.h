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

class fiffTreeNode {
public:
	explicit fiffTreeNode(fiff_dir_entry_t *d = nullptr, fiffTreeNode *p = nullptr) { dir = d;  parent = p; }

	void clear() { blocks.clear(); tags.clear(); parent = nullptr; }
	QList<fiffTreeNode *> findBlock(int kind);

	QMultiMap<int, fiffTreeNode *> blocks;
	QMap<int, qint64> tags; // tag refered by kind and value is the position in file
	fiffTreeNode *parent;
	fiff_dir_entry_t *dir;
};

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
	T readTagData(fiff_tag_t *tag);
	void buildTree();


	bool checkForFileStart();
	bool checkForCompatibleFile(const QString& path);
	QFile m_file;
	QDataStream m_stream;
	fiffId *m_fileID;
	fiff_dir_entry_t *m_dir;
	qint32 m_nchan, m_dataPack, m_firstSample, m_lastSample, m_skipSample;
	qint32 m_nSamples;	// total number of samples in data
	float m_sfreq;
	QList<fiff_dir_entry_t *> m_dirEntries;
	QList<fiffChInfo> m_chInfos;
	fiffTreeNode m_root;
	QList<fiffTreeNode *> m_children; // list of node inserted in tree as children of root node.
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

