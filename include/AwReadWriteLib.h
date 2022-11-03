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
#ifndef AWREADWRITELIB_H
#define AWREADWRITELIB_H
#include <AwIO.h>
#include <QtGlobal>
#include <QList>
#include <AwChannel.h>
#include <AwMarker.h>


/*!
 * \brief
 * AwBlock defines a block of data.
 * 
 * A block has a duration in seconds and a number of samples.
 * A position in seconds defines where the block begins in the file.
 * 
 * \remarks
 *	 A block of data may contains markers.
 * 
 * \see
 * AwMarker AwMarkerList
 */
class AW_RW_EXPORT AwBlock
{
public:
	AwBlock();
	AwBlock(int index);
	~AwBlock();
	/** Gets the total duration in seconds. **/
	inline float duration() { return m_duration; }
	/** Sets the duration in seconds. **/
	inline void setDuration(float duration) { m_duration = duration; }
	/** Returns the number of samples for a channel. **/
	inline qint64 samples() { return m_samples; }
	/** Sets the number of samples by channels. **/
	inline void setSamples(qint64 samples) { m_samples = samples; }
	/** Defines the offset position of the block in the current data file. **/
	inline void setStartPosition(float pos) { m_start = pos; }
	/** Returns the position from the beginning of file. **/
	inline float start() { return m_start; }
	/** Returns the ID of the block. **/
	inline int index() { return m_id; }
	/** Returns the markers present in the block. The list can be empty if no markers are present. **/
//	inline AwMarkerList& markers() { return m_markers; }	
	inline AwSharedMarkerList markers() { return m_markers; }
	/** Returns the number of markers present in the block. **/
	inline qint32 markersCount() { return m_markers.size(); }
	/** Adds a marker to the block. The marker passed as parameter is duplicated before insertion. **/
	AwMarker* addMarker(const AwMarker& marker);
	/** Adds a marker to the block. The marker passed as parameter is duplicated before insertion. **/
	//AwMarker* addMarker(AwMarker *marker);
	/** Adds a complete list of markers to the block. Markers are cloned. **/
	void setMarkers(const AwSharedMarkerList& markers);
	void clear();

protected:
	float m_duration;	
	qint64 m_samples;	
	float m_start;		
	int m_id;			
	AwSharedMarkerList m_markers;
//	AwSharedMarkerList m_sMarkers;
};

typedef QList<AwBlock *> AwBlockList;  ///< Define a list of blocks.

struct AW_RW_EXPORT AwVideoSynch {
	AwVideoSynch() {
		shift = drift = 0;
	}
	qint32 shift;	// expressed in ms
	qint32 drift;	// expressed in ms
};

/*!
 * \brief
 * This is the base class to manage informations about Reader or Writer Plugins.
 * 
 * This class is used by reader and writer plugins to store low level information.
 * This class contains the list of data blocks of a file and some attributes related to 
 * patient and manufacturer.
 * 
 * \remarks
 * Reader plug-ins must update this object by adding block of data and by setting useful informations.
 * 
 * \see
 * AwBlock | AwDataInfoReader | AwDataInfoWriter 
 */

namespace data_info {
	constexpr auto data_path = "data_path";
	constexpr auto data_dir = "data_dir";
	constexpr auto first_name = "first_name";
	constexpr auto last_name = "last_name";
	constexpr auto manufacturer = "manufacturer";
	constexpr auto time = "time";
	constexpr auto date = "date";
	constexpr auto iso_date = "iso_date";
	constexpr auto total_duration = "total_duration";
	constexpr auto max_sr = "max_sr";
	constexpr auto channels_count = "channels_count";
	constexpr auto samples = "samples";
	constexpr auto marker_file = "marker_file";
	constexpr auto bad_file = "bad_file";
	constexpr auto montage_file = "montage_file";
}


class AW_RW_EXPORT AwDataInfo
{
	friend class AwFileIO;
public: 
	AwDataInfo();
	virtual ~AwDataInfo();

	/** Returns the number of data blocks in the file. **/
	inline AwBlockList& blocks() { return m_blocks; }
	/** Returns the list of channels present in the file. **/
	inline AwChannelList& channels() { return m_channels; }
	/** Returns the number of channels present in the file. **/
	inline qint32 channelsCount() { return m_channels.size(); }	
	/** Gets the patient's name. It could be an empty string. **/
	inline QString firstName() { return m_settings.value(data_info::first_name).toString(); }
	inline QString lastName() { return m_settings.value(data_info::last_name).toString(); }
	/** Gets the recording date. It could be an empty string. **/
	inline QString recordingDate() { return m_settings.value(data_info::date).toString(); }
	/** Gets the recording time. It could be an empty string. **/
	inline QString recordingTime() { return m_settings.value(data_info::time).toString(); }
	/** Get the date and time in ISO format **/
	inline QString isoDate() { return m_settings.value(data_info::iso_date).toString(); }
	/** Get the name of the manufacturer. It could be an empty string. **/
	inline QString manufacturer() { return m_settings.value(data_info::manufacturer).toString(); }

	/** Adds a new block and returns the pointer to the newly created object. **/
	AwBlock *newBlock();

	/** Returns the total duration in seconds for the whole file. This is calculated by adding the duration of all blocks present in the file**/
	float totalDuration();				
	/** Returns the total number of samples for the whole file. **/
	qint64 totalSamples();	

	/** Returns true if the file contains more than one block. The file is then considered as Epoched. **/
	inline bool isEpoched() { return m_blocks.size() > 1; }

	///** Returns the full path to the current open data file. **/
	//inline QString& fileName() { return m_fileName; }
	
	///** Sets the full path to data file. **/
	//inline void setFileName(const QString& name) { m_fileName = name; }
	/** Sets the manufacturer name. **/
	inline void setManufacturer(const QString& man) { m_settings[data_info::manufacturer] = man; }
	/** Sets the recording time. **/
	inline void setTime(const QString& time) { m_settings[data_info::time] = time; }
	/** Sets the recording date. **/
	inline void setDate(const QString& date) { m_settings[data_info::date] = date; }
	/** Sets the patient's name. **/
	void setFirstName(const QString& firstName) { m_settings[data_info::first_name] = firstName; }
	void setLastName(const QString& lastName) { m_settings[data_info::last_name] = lastName; }
	inline void setChannels(const AwChannelList& channels) { m_channels = channels; }

	// sidecar files
	void setMrkFile(const QString& file) { m_settings[data_info::marker_file] = file; }
	void setBadFile(const QString& file) { m_settings[data_info::bad_file] = file; }
	void setMtgFile(const QString& file) { m_settings[data_info::montage_file] = file; }
	QString badFile() { return m_settings.value(data_info::bad_file).toString(); }
	QString mtgFile() { return m_settings.value(data_info::montage_file).toString(); }
	QString mrkFile() { return m_settings.value(data_info::marker_file).toString(); }

	virtual void clear();

	/** Returns the index of a channel in the file base on the channel's name.
	If the channel is not found, returns -1 **/
	int indexOfChannel(const QString& name);
	/** Adds a new channel as As Recorded channel.
	The As Recorded channels are the channels found in the data file. **/
	AwChannel* addChannel(AwChannel *channel);
	/** Allows to change the name of a previously inserted channel.
	This will do nothing if the channel does not exist. **/
	void changeChannelName(const QString& oldName, const QString& newName);
	/** set the date and time in ISO format **/
	void setISODate(const QString& dateTime) { m_settings[data_info::iso_date] = dateTime; }
	/** get the structure for video synchronization **/
	inline AwVideoSynch& videoSynch() { return m_videoSynch; }
	inline QVariantMap& settings() { return m_settings; }
protected:
	QMap<QString, QVariant> m_settings;
	AwBlockList m_blocks;
	AwChannelList m_channels;
	quint32 m_channelsCount;
	// Hash table to store index of channels and their name.
	QHash<QString, int> m_labelToIndex;
	AwVideoSynch m_videoSynch;
};

namespace AwIO
{
	void AW_RW_EXPORT rescaleDataToExport(const AwChannelList& channels);
}

#endif // AWREADWRITELIB_H
