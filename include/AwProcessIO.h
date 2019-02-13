/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <AwProcess.h>
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwFileIO.h>
#include <QMutex>
#include <filter/AwFilterSettings.h>
#include <AwCommandLine.h>

/** AwProcessIO defines object used as input or output by AwProcess **/
class AW_PROCESS_EXPORT AwProcessIO
{
public:
	AwProcessIO() { m_reader = NULL; }
	~AwProcessIO();

	inline bool containsChannels() { return !m_channels.isEmpty(); }
	inline bool containsMarkers() { return !m_markers.isEmpty(); }
	inline bool containsWidgets() { return !m_widgets.isEmpty(); }
	inline bool containsCustomData() { return !customData.isEmpty(); }
	inline bool containsICA() { return !icaPath.isEmpty(); }
	bool isEmpty();
	inline AwFileIO *reader() { return m_reader; }
	void setReader(AwFileIO *reader) { m_reader = reader; }
	void clearChannels();
	void clearMarkers();
	void clearWidgets();
	void setArguments(const AwArguments& arguments) { m_arguments = arguments; }
	inline AwArguments& args() { return m_arguments; }

	inline AwChannelList& channels() { return m_channels; }
	inline AwMarkerList& markers() { return m_markers; }
	inline QList<QWidget *>& widgets() { return m_widgets; }
	/** Set a new list of channels. Previous channels will be deleted!!. duplicate indicates that the list will be duplicated and then set as the new list. **/
	void setNewChannels(const AwChannelList& channels, bool duplicate = false);
	/** Set a new list of markers. Previous markers will be deleted!!  duplicate indicates that the list will be duplicated and then set as the new list. **/
	void setNewMarkers(const AwMarkerList& markers, bool duplicate = false);
	/** Append markers **/
	void addMarkers(const AwMarkerList& markers, bool duplicate = false);
	void addMarker(AwMarker *marker);
	/** apend widgets **/
	void addWidget(QWidget *widget);
	/** Append channels **/
	void addChannels(const AwChannelList& channels, bool duplicate = false);
	void addChannel(AwChannel *channel);
	//AwChannelList channels;
	//AwMarkerList markers;
	QList<AwFileIOPlugin *> readers, writers;
	QString dataFolder;
	QString dataPath;
	QString workingDirPath;
	QString pluginDirPath;
	QString icaPath;			// contains the path to the .ica.h5 file or is empty if no ICA was computed.
	//QList<QWidget *> widgets;
	QVariantList customData;
	QStringList badLabels;		// contains channels marked as bad
	QStringList processPluginNames;
	AwFilterSettings filterSettings;

	// Display process specific
	float currentPosInFile;
	float pageDuration;
	float fileDuration;	// total duration of data in file

	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }

protected:
	AwChannelList m_channels;
	AwMarkerList m_markers;
	QList<QWidget *> m_widgets;
	AwFileIO *m_reader;
	AwArguments m_arguments;	// used for pluing launches from the command line
	QMutex m_mutex;
};