/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
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
//  License along with this software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <AwProcess.h>
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwFileIO.h>
#include <QMutex>
#include <AwFilteringOptions.h>

/** AwProcessIO defines object used as input or output by AwProcess **/
class AW_PROCESS_EXPORT AwProcessIO
{
public:
	AwProcessIO() { m_reader = NULL; }
	~AwProcessIO();

	inline bool containsChannels() { return !channels.isEmpty(); }
	inline bool containsMarkers() { return !markers.isEmpty(); }
	inline bool containsWidgets() { return !widgets.isEmpty(); }
	inline bool containsCustomData() { return !customData.isEmpty(); }
	inline bool containsICA() { return !icaPath.isEmpty(); }
	bool isEmpty();
	inline AwFileIO *reader() { return m_reader; }
	void setReader(AwFileIO *reader) { m_reader = reader; }
	void clearChannels();
	void clearMarkers();
	void clearWidgets();

	AwChannelList channels;
	AwMarkerList markers;
	QList<AwFileIOPlugin *> readers, writers;
	QString dataFolder;
	QString dataPath;
	QString workingDirPath;
	QString pluginDirPath;
	QString icaPath;			// contains the path to the .ica.h5 file or is empty if no ICA was computed.
	QList<QWidget *> widgets;
	QVariantList customData;
	QStringList badLabels;		// contains channels marked as bad
	QStringList processPluginNames;
	AwFilteringOptions filteringOptions;

	// Display process specific
	float currentPosInFile;
	float pageDuration;
	float fileDuration;	// total duration of data in file

	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }

protected:
	AwFileIO *m_reader;
	QMutex m_mutex;
};