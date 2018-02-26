/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef BIOSEMIREADER_H
#define BIOSEMIREADER_H

#include "biosemireader_global.h"
#include "edflib.h"
#include <AwFileIO.h>
#include <QtCore>
#include <QDataStream>


class BIOSEMIREADER_EXPORT BiosemiFileReader : public AwFileIO
{
	Q_OBJECT
	Q_INTERFACES(AwFileIO)

public:
	BiosemiFileReader(const QString& filename);
	~BiosemiFileReader() { cleanUpAndClose(); }

	qint64 readDataFromChannels(float start, float duration, QList<AwChannel *> &channelList);
	FileStatus openFile(const QString &path);
	FileStatus canRead(const QString &path);
	AwChannelList triggerChannels() { return m_triggers; }

	void cleanUpAndClose();
private:
	long offset;
	int nBlocks;
	int nSamplePerBlock;
	int sample_size;
	QFile file;
	QDataStream stream;
	QHash<QString, int> labelsToIndex;
	AwChannelList m_triggers;

	struct edf_hdr_struct *m_hdr;
	qint32 getInt32(uchar *p, const bool little_endian = true);
};

class BIOSEMIREADER_EXPORT BiosemiReader : public AwFileIOPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwFileIOPlugin)
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
	Q_PLUGIN_METADATA(IID AwFileIOInterfacePlugin_IID)
#endif
public:
	BiosemiReader();
	BiosemiFileReader *newInstance(const QString& filename) { return new BiosemiFileReader(filename); }
};


#endif // BIOSEMIREADER_H
