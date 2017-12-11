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
#include <AwGlobal.h>
#include <AwDataClient.h>
#include <AwChannel.h>
#include <epoch/AwEpoch.h>
#include <epoch/AwEpochDataBuffer.h>
#include <epoch/AwEpochAverageChannel.h>
#include <epoch/AwEpochComputeSettings.h>
#include <armadillo>
using namespace arma;

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
	float epochDuration();

	void setFilters(float *filters);
	inline float* filters() { return m_filters; }

	/** Build epochs using the markers, around a specific marker, giving pre and post latencies **/
	int buildEpochs(const AwMarkerList& markers, const QString& label, float pre, float post);

	/** Compute the average. Verbose will send a signal each time an epoch is loaded **/
	int doAverage(bool verbose = false);
	int loadEpoch(int index);
	int loadEpoch(AwEpoch *epoch);
	void setComputeSettings(AwEpochComputeSettings& settings);

	/** Create a data buffer containing the averaged channels data **/
	AwEpochDataBuffer *createAVGBuffer();

	AwEpochAverageChannel *createAVGChannel(const QString& label);
signals:
	void averageChannelsCreated(AwChannelList *channels);
	void epochLoaded(int epoch);
protected:
	void clearEpochs();

	AwChannelList m_channels, m_avgChannels;
	AwEpochComputeSettings m_computeSettings;
	AwEpochList m_epochs;
	float m_zeroPos;		// position of zero in the epochs
	int m_zeroSample;		// offset in sample of zero position
	qint64 m_nSamples;		// total number of samples in each epochs
	float m_totalDuration;	// total duration in seconds of the data file.
	fmat m_matrix;			// matrix containing the data for all the epochs.
	fmat m_errorType;		// the error type of samples toward epochs.
	fmat m_std;				// keep a copy of std deviation of samples toward epochs
	QString m_name;			// condition name associated with the epochs
	int m_modality;			// type of channels used to epoch.
	float m_filters[3];		// the low pass, high pass and notch filter applied to data.
	bool m_avgIsDone;		// Flag indicating if the averaging is done.
};
