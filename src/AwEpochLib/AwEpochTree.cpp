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
#include <epoch/AwEpochTree.h>
#include <epoch/AwEpochAverageChannel.h>
#include <math/AwMath.h>
#include <AwException.h>

/** The markers must have the same non null duration **/
AwEpochTree::AwEpochTree(const QString& name, const AwChannelList& channels, float totalDuration, QObject *parent) : AwDataClient(parent)
{
	m_channels = channels;
	m_nSamples = channels.first()->dataSize();
	m_totalDuration = totalDuration;
	m_zeroPos = 0;
	m_zeroSample = 0;
	m_name = name;
	m_modality = channels.first()->type();
	m_avgIsDone = false;
	m_averaged = Q_NULLPTR;
}

AwEpochTree::~AwEpochTree()
{
	if (m_averaged)
		delete m_averaged;
}

int AwEpochTree::numberOfGoodEpochs()
{
	int count = 0;
	for (auto e : m_epochs)
		if (!e->isRejected())
			count++;

	return count;
}

int AwEpochTree::buildEpochs(const AwMarkerList& markers, const QString& label, float pre, float post, 
	const AwMarkerList& artefacts)
{
	m_after = post;
	m_before = pre;
	if (m_totalDuration <= 0.) {
		throw AwException(QString("duration of data file is zero."), QString("AwEpochTree::buildEpochs()"));
		return -1;
	}
	clearEpochs();
	m_markerLabel = label;
	AwMarkerList tmp;
	for (AwMarker *m : markers) {
		if (m->label() != label)
			continue;
		float pos = m->start() - pre;
		// skip epochs that overlap starting and/or ending positions of data.
		if (pos < 0.) // skip this epoch 
			continue;
		if (m->start() + post > m_totalDuration)
			continue;

		//m_epochs << new AwEpoch(this, new AwMarker(label, pos, pre + post));
		tmp << new AwMarker(label, pos, pre + post);
	}

	if (!artefacts.isEmpty()) {
        auto artefactstemp = artefacts;
		auto dummy = AwMarker::cutAroundMarkers(tmp, artefactstemp);
		while (!tmp.isEmpty())
			delete tmp.takeFirst();
		tmp = dummy;
	}
	for (auto m : tmp) {
		m_epochs << new AwEpoch(this, m);
	}
	m_zeroPos = pre;
	m_zeroSample = (int)floor(pre * m_channels.first()->samplingRate());
	m_epochDuration = pre + post;
	return 0;
}

void AwEpochTree::clearEpochs()
{
	while (!m_epochs.isEmpty())
		delete m_epochs.takeFirst();
	m_zeroPos = 0;
	m_zeroSample = 0;
	m_avgIsDone = false;
}

void AwEpochTree::setComputeSettings(AwEpochComputeSettings& settings)
{
	m_computeSettings = settings;
	m_avgIsDone = false;
}

void AwEpochTree::average()
{
	if (!m_isConnected)
		return;
	if (m_epochs.isEmpty())
		return;

	AwEpochList epochs;
	for (auto e : m_epochs) { // remove rejected epochs before averaging
		if (!e->isRejected())
			epochs << e;
	}

	auto totalEpochs = epochs.size();

	// build a matric the size of the final averaged epoch
	mat avgEpochMat = arma::zeros(this->channels().size(), (uword)floor(this->m_epochDuration * this->channels().first()->samplingRate()));
	

	// precompute latencies to sample in case a baseline correction is required.
	int baselineLowerSample = (int)floor(m_computeSettings.latencyRange[0] * m_epochs.first()->channels().first()->samplingRate());
	int baselineHigherSample = (int)floor(m_computeSettings.latencyRange[1] * m_epochs.first()->channels().first()->samplingRate());

	for (auto e : epochs) {
		loadEpoch(e);

		for (auto c : e->channels()) {
			uword row = 0;
			// get data as double not float
			rowvec channelData = arma::conv_to<rowvec>::from(AwMath::channelToVec(c));
			if (m_computeSettings.useBaselineCorrection) {
				auto zeroSample = e->zeroSample();
				auto endSample = zeroSample - 1;
				auto start = zeroSample - baselineLowerSample;
				auto end = endSample - baselineHigherSample;
				if (start < 0)
					start = 0;
				double mean = 0., stddev = 0.;
				switch (m_computeSettings.baselineMethod)
				{
				case AwEpochComputeSettings::SubstractMean:
					mean = arma::mean(channelData(span(start, end)));
					channelData -= mean;
					break;
				case AwEpochComputeSettings::DivideByStd:
					stddev = arma::stddev(channelData(span(start, end)));
					channelData /= stddev;
					break;
				}
				// copy channel data back (for further std error computation)
				for (auto i = 0; i < c->dataSize(); i++)
					c->data()[i] = channelData(i);
			}
			// put row in the avgMatrix
			// add channel data to matrix.
			avgEpochMat.row(row) += channelData;
			row++;
		}
		emit epochLoaded(totalEpochs);
	}
	
	double sqr_n = sqrt(totalEpochs);
	// create a std dev matrix of each samples of a channel through all epochs.
	mat std_dev = arma::zeros(avgEpochMat.n_rows, avgEpochMat.n_cols);
	vec valuesThroughEpochs(totalEpochs);
	// compute error type based on std dev of each epochs
	for (arma::uword n = 0; n < std_dev.n_rows; n++) {
		for (arma::uword m = 0; m < std_dev.n_cols; m++) {
			for (auto nE = 0; nE < totalEpochs; nE++) {
				valuesThroughEpochs(nE) = epochs.at(nE)->channels().at(n)->data()[m];
			}
			std_dev(n, m) = stddev(valuesThroughEpochs);
		}
	}
	// do the averaging of all epochs
	avgEpochMat /= totalEpochs;
	mat errorTypeMat = std_dev / sqr_n;

	// build the resulting epoch
	AwEpochChannelList resultChannels;
	for (arma::uword n = 0; n < avgEpochMat.n_rows; n++) {
		auto channel = new AwEpochAverageChannel(m_channels.value(n));
		channel->setData(arma::conv_to<fvec>::from(avgEpochMat.row(n)));
		channel->setErrorType(arma::conv_to<fvec>::from(errorTypeMat.row(n)));
		resultChannels << channel;
	}
	if (!m_averaged)
		m_averaged = new AwAvgEpoch(this, m_epochs.first()->posAndDuration());
	m_averaged->setChannels(resultChannels);
}


int AwEpochTree::doAverage(bool verbose)
{
	if (!m_isConnected)
		return -1;
	if (m_epochs.isEmpty())
		return -1;
	m_matrix.clear();
	arma::uword nCols = (arma::uword)floor(m_epochDuration * m_channels.first()->samplingRate());
	arma::uword nRows = (arma::uword)m_channels.size();
	fmat epochs_matrix = arma::fmat(nRows, nCols);
	epochs_matrix.zeros();

	QVector<fmat> epochs;

	// skip rejected epochs
	int nEpochs = 0;
	for (AwEpoch *e : m_epochs) {
		if (e->isRejected())
			continue;
		loadEpoch(e); // (&m_channels, e->posAndDuration->start(), e->posAndDuration->duration());
		
		// apply baseline correction if set in settings
		if (m_computeSettings.useBaselineCorrection) {
			if (m_zeroPos > 0.) {
				for (auto c : m_channels) {
					fvec vec = AwMath::channelToVec(c);
					arma::uword n = (arma::uword)floor(m_zeroPos * c->samplingRate());
					int start = 0, end = n - 1;
					if (m_computeSettings.latencyRange[0] > 0 && m_computeSettings.latencyRange[0] <= m_zeroPos)
						start = (int)floor((m_zeroPos - m_computeSettings.latencyRange[0]) * c->samplingRate());
					if (m_computeSettings.latencyRange[1] > 0 && m_computeSettings.latencyRange[1] < m_computeSettings.latencyRange[0])
						end = (int)floor((m_zeroPos - m_computeSettings.latencyRange[1]) * c->samplingRate()) - 1;
					if (m_computeSettings.baselineMethod == AwEpochComputeSettings::SubstractMean) {
						float mean = arma::mean(vec(span(start, end)));
						vec -= mean;
					}
					else if (m_computeSettings.baselineMethod == AwEpochComputeSettings::DivideByStd) {
						float stddev = arma::stddev(vec(span(start, end))); 
						vec /= stddev;
					}
					// copy the data back to the original AwChannel object
					memcpy(c->data(), vec.memptr(), c->dataSize() * sizeof(float));
				}
			}
		}
		epochs.append(AwMath::channelsToFMat(m_channels));
		epochs_matrix += epochs.last();
		nEpochs++;
		if (verbose)
			emit epochLoaded(nEpochs);
	}
	if (nEpochs == 0) 
		return -1;

	// compute averaged signals in m_matrix
	m_matrix = epochs_matrix / nEpochs;
	m_std = arma::fmat(nRows, nCols);
	m_std.zeros();
	float sqr_n = (float)sqrt(nEpochs);
	//for (arma::uword n = 0; n < epochs_matrix.n_rows; n++) {
	//	for (arma::uword m = 0; m < epochs_matrix.n_cols; m++) {
	//		float x2 = 0.;
	//		for (int E = 0; E < epochs.size(); E++) {
	//			// sum of xi²
	//			x2 += epochs.at(E).at(n, m) * epochs.at(E).at(n, m);
	//		}
	//		// std deviation
	//		m_std(n, m) = sqrt(x2 / (nEpochs - 1) - m_matrix(n, m) * m_matrix(n, m));
	//	}
	//}
	//// now compute error types
	//m_errorType = m_std;
	//m_errorType /= sqr_n;
	fvec vec(nEpochs);
	for (arma::uword n = 0; n < epochs_matrix.n_rows; n++) {
		for (arma::uword m = 0; m < epochs_matrix.n_cols; m++) {
			for (int E = 0; E < epochs.size(); E++) {
				vec(E) = epochs.at(E).at(n, m);
			}
			m_std(n, m) = arma::stddev(vec);
		}
	}
	m_errorType = m_std / sqr_n;
	m_avgIsDone = true;
	return 0;
}

///
/// Create a buffer for the average channels.
/// Return NULL if the averaging is not done yet.
//AwEpochDataBuffer *AwEpochTree::createAVGBuffer()
//{
//	if (!m_avgIsDone)
//		return NULL;
//
//	//qDeleteAll(m_avgChannels);
//	while (!m_avgChannels.isEmpty())
//		delete m_avgChannels.takeLast();
//	m_avgChannels.clear();
//	for (arma::uword i = 0; i < m_matrix.n_rows; i++) {
//		AwEpochAverageChannel *newc = new AwEpochAverageChannel(m_channels.at(i));
//		newc->setData(conv_to<fvec>::from(m_matrix.row(i)));
//		newc->setErrorType(conv_to<fvec>::from(m_errorType.row(i)));
//		newc->setZeroPosition(m_zeroPos);
//		m_avgChannels << newc;
//	}
//	AwEpochDataBuffer *buffer = new AwEpochDataBuffer(&m_avgChannels);
//	return buffer;
//}
//
//AwEpochAverageChannel *AwEpochTree::createAVGChannel(const QString& label)
//{
//	QStringList labels = AwChannel::getLabels(m_channels);
//	int index = labels.indexOf(label);
//	if (index == -1)
//		return NULL;
//
//	AwEpochAverageChannel *newc = new AwEpochAverageChannel(m_channels.at(index));
//
//	newc->setData(conv_to<fvec>::from(m_matrix.row(index)));
//	newc->setErrorType(conv_to<fvec>::from(m_errorType.row(index)));
//	newc->setZeroPosition(m_zeroPos);
//	newc->setCondition(this);
//	return newc;
//}

///
/// Load the epoch at index index.
/// Load data into channels associated to the condition.
/// return -1 if failed.
int AwEpochTree::loadEpoch(int index)
{
	QMutexLocker lock(&m_mutex);
	if (index < 0 || index >= m_epochs.size())
		return -1;

	auto epoch = m_epochs.at(index);
	if (epoch->isLoaded())
		return 0;
	auto channels = epoch->channels();
//	m_filterSettings.apply(channels);
	requestData(&channels, epoch->posAndDuration());
	epoch->setLoaded();
	return 0;
}

int AwEpochTree::loadEpoch(AwEpoch *e)
{
	return loadEpoch(m_epochs.indexOf(e));
}
