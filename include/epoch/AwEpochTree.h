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

#include <AwDataClient.h>
#include <AwChannel.h>
#include <epoch/AwEpoch.h>
#include <epoch/AwEpochDataBuffer.h>
#include <epoch/AwEpochAverageChannel.h>
#include <epoch/AwEpochComputeSettings.h>
#include <aw_armadillo.h>
//#include <filter/AwFilterSettings.h>
#include <epoch/AwAvgEpoch.h>

class AW_EPOCH_EXPORT AwEpochTree : public AwDataClient
{
	Q_OBJECT
public:
	/** Create a tree from markers */
	AwEpochTree(const QString& name, const AwChannelList& channels, float totalDuration = 0., QObject *parent = 0);
	~AwEpochTree();

	inline int zeroSample() { return m_zeroSample;  }
	inline void setZeroSample(int sample) { m_zeroSample = sample; }
	inline qint64 nSamples() { return m_nSamples;  }
	inline float zeroPos() { return m_zeroPos;  }
	inline void setZeroPos(float pos) { m_zeroPos = pos; }
	inline int numberOfEpochs() { return m_epochs.count(); }
	int numberOfGoodEpochs();
	inline void setTotalDuration(float dur) { m_totalDuration = dur; }
	inline AwEpochList& epochs() { return m_epochs; }
	inline QString& name() { return m_name;  }
	inline int type() { return m_modality;  }
	inline AwChannelList& channels() { return m_channels; }
	inline float totalDuration() { return m_totalDuration; }
	inline bool isAvgAvailable() { return m_avgIsDone; }
	inline int modality() { return m_modality; }
	inline float preLatency() { return m_before; }
	inline float postLatency() { return m_after; }
	inline float epochDuration() { return m_epochDuration; }
	inline QString& markerLabel() { return m_markerLabel; }

	AwAvgEpoch *averagedResult() { return m_averaged; }
	/** Build epochs using the markers, around a specific marker, giving pre and post latencies **/
	int buildEpochs(const AwMarkerList& markers, const QString& label,
		float pre, float post, AwMarkerList& artefacts = AwMarkerList());

	/** Compute the average. Verbose will send a signal each time an epoch is loaded **/
	int doAverage(bool verbose = false);
	void average();
	int loadEpoch(int index);
	int loadEpoch(AwEpoch *epoch);
	void setComputeSettings(AwEpochComputeSettings& settings);
//	void setFilterSettings(const AwFilterSettings& settings) { m_filterSettings = settings; }

	/** Create a data buffer containing the averaged channels data **/
	//AwEpochDataBuffer *createAVGBuffer();
	//AwEpochAverageChannel *createAVGChannel(const QString& label);
signals:
	void averageChannelsCreated(AwChannelList *channels);
	void epochLoaded(int epoch);
	void progressChanged(int percent);
	void dataLoaded();
protected:
	void clearEpochs();

	AwChannelList m_channels, m_avgChannels;
	AwMarkerList m_artefacts;	// may contain markers for artefacted data.
	AwEpochComputeSettings m_computeSettings;
//	AwFilterSettings m_filterSettings;
	AwEpochList m_epochs;
	AwAvgEpoch *m_averaged;	// result of the averaging
	float m_zeroPos;		// position of zero in the epochs
	int m_zeroSample;		// offset in sample of zero position
	qint64 m_nSamples;		// total number of samples in each epochs
	float m_totalDuration;	// total duration in seconds of the data file.
	fmat m_matrix;			// matrix containing the data for all the epochs.
	fmat m_errorType;		// the error type of samples toward epochs.
	fmat m_std;				// keep a copy of std deviation of samples toward epochs
	QString m_name;			// condition name associated with the epochs
	QString m_markerLabel;	// label of markers used to build epochs.
	int m_modality;			// type of channels used to epoch.
	float m_before, m_after;	// pre and post latencies to build epochs.
	float m_epochDuration;	// duration in seconds of each epochs.
	bool m_avgIsDone;		// Flag indicating if the averaging is done.
	// Threading
	QMutex m_mutex;
};

using AwConditionList = QList<AwEpochTree *>;