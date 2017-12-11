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
#ifndef AWREADWRITELIB_H
#define AWREADWRITELIB_H
#include <AwGlobal.h>
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
	inline AwMarkerList& markers() { return m_markers; }	
	/** Returns the number of markers present in the block. **/
	inline qint32 markersCount() { return m_markers.size(); }
	/** Adds a marker to the block. The marker passed as parameter is duplicated before insertion. **/
	AwMarker* addMarker(AwMarker& marker);
	/** Adds a marker to the block. The marker passed as parameter is duplicated before insertion. **/
	AwMarker* addMarker(AwMarker *marker);
	/** Adds a complete list of markers to the block. Markers are cloned. **/
	void setMarkers(const AwMarkerList& markers);
	void clear();
protected:
	float m_duration;	
	qint64 m_samples;	
	float m_start;		
	int m_id;			
	AwMarkerList m_markers;
};

typedef QList<AwBlock *> AwBlockList;  ///< Define a list of blocks.

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

class AW_RW_EXPORT AwDataInfo
{
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
	inline QString& patientName() { return m_patientName; }
	/** Gets the recording date. It could be an empty string. **/
	inline QString& recordingDate() { return m_date; }
	/** Gets the recording time. It could be an empty string. **/
	inline QString& recordingTime() { return m_time; }
	/** Get the name of the manufacturer. It could be an empty string. **/
	inline QString& manufacturer() { return m_manufacturer; }

	/** Adds a new block and returns the pointer to the newly created object. **/
	AwBlock *newBlock();

	/** Returns the total duration in seconds for the whole file. This is calculated by adding the duration of all blocks present in the file**/
	float totalDuration();				
	/** Returns the total number of samples for the whole file. **/
	qint64 totalSamples();	

	/** Returns true if the file contains more than one block. The file is then considered as Epoched. **/
	inline bool isEpoched() { return m_blocks.size() > 1; }

	/** Returns the full path to the current open data file. **/
	inline QString& fileName() { return m_fileName; }
	
	/** Sets the full path to data file. **/
	inline void setFileName(const QString& name) { m_fileName = name; }
	/** Sets the manufacturer name. **/
	inline void setManufacturer(const QString& man) { m_manufacturer = man; }
	/** Sets the recording time. **/
	inline void setTime(const QString& time) { m_time = time; }
	/** Sets the recording date. **/
	inline void setDate(const QString& date) { m_date = date; }
	/** Sets the patient's name. **/
	inline void setPatientName(const QString& name) { m_patientName = name; }
	inline void setChannels(AwChannelList& channels) { m_channels = channels; }
	virtual void clear();

	/** Returns the index of a channel in the file base on the channel's name.
	If the channel is not found, returns -1 **/
	int indexOfChannel(const QString& name);
	/** Adds a new channel as As Recorded channel.
	The As Recorded channels are the channels found in the data file. **/
	AwChannel* addChannel(AwChannel *channel);
	/** Adds a new channel as As Recorded channel.
	The As Recorded channels are the channels found in the data file. **/
	AwChannel* addChannel(AwChannel& channel);
	/** Allows to change the name of a previously inserted channel.
	This will do nothing if the channel does not exist. **/
	void changeChannelName(const QString& oldName, const QString& newName);
protected:
	AwBlockList m_blocks;
	AwChannelList m_channels;
	QString m_patientName;
	QString m_date;	
	QString m_time;	
	QString m_manufacturer;
	QString m_fileName;	
	quint32 m_channelsCount;
	// Hash table to store index of channels and their name.
	QHash<QString, int> m_labelToIndex;
};

#endif // AWREADWRITELIB_H
