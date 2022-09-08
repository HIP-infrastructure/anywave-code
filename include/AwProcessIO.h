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
	AwProcessIO() { m_reader = nullptr; }
	~AwProcessIO();

	inline bool containsChannels() { return !m_channels.isEmpty(); }
	inline bool containsMarkers() { return !m_markers.isEmpty(); }
	inline bool containsWidgets() { return !m_widgets.isEmpty(); }
	inline bool containsCustomData() { return !customData.isEmpty(); }
	inline bool launchedUsingQST() { return timeSelection.duration() > 0.; }
	bool isEmpty();
	inline AwFileIO *reader() { return m_reader; }
	void setReader(AwFileIO* reader);
	void clearChannels();
	void clearMarkers();
	void clearWidgets();


	AwChannelList& channels() { m_regularChannelList = AwChannel::toChannelList(m_channels);  return m_regularChannelList; }
	QList<QSharedPointer<AwChannel>>& sharedChannels() { return m_channels; }
	inline AwMarkerList& markers() { return m_markers; }
	AwSharedMarkerList& sharedMarkers() { return m_sMarkers; }
	inline AwMarkerList& modifiedMarkers() { return m_modifiedMarkers; }


	inline QList<QWidget*>& widgets() { return m_widgets; }
	/** Set a new list of channels. Previous channels will be deleted!!. duplicate indicates that the list will be duplicated and then set as the new list. **/
	void setNewChannels(const AwChannelList& channels, bool duplicate = false);
	void setNewChannels(const AwSharedChannelList& channel);
	/** Set a new list of markers. Previous markers will be deleted!!  duplicate indicates that the list will be duplicated and then set as the new list. **/
	void setNewMarkers(const AwMarkerList& markers, bool duplicate = false);
	/** set the new list, markers are not duplicated and previous markers are not deleted **/
	void setMarkers(const AwMarkerList& markers) { m_markers = markers; }
	/** Set the resulting marker list after filtering input markers with use_markers or skip_markers **/
	void setModifiedMarkers(const AwMarkerList& markers);
	/** Append markers **/
	void addMarkers(const AwMarkerList& markers, bool duplicate = false);
	void addMarker(AwMarker *marker);
	/** apend widgets **/
	void addWidget(QWidget* widget);
	/** Append channels **/
	void addChannels(const AwChannelList& channels, bool duplicate = false);
	void addChannel(AwChannel *channel);
	QList<AwFileIOPlugin *> readers, writers;

	AwMarker timeSelection;		// optional marker used in Quick Time Selection mode in AnyWave (contains the timing the user has selected before launching the process).
	QVariantList customData;

	AwFilterSettings filterSettings;
	QVariantMap settings;

	void lock() { m_mutex.lock(); }
	void unlock() { m_mutex.unlock(); }
protected:
	AwMarkerList m_markers;
	AwMarkerList m_modifiedMarkers;	// this list contains markers filtered by --use_markers and --skip_markers options

	QList<QSharedPointer<AwChannel>> m_channels;
	AwSharedMarkerList m_sMarkers;
	AwChannelList m_regularChannelList;

	QList<QWidget*> m_widgets;
	AwFileIO *m_reader;
	QMutex m_mutex;
};