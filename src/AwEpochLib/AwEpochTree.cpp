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
	m_filters[0] = m_filters[1] = m_filters[2] = 0.;
	m_avgIsDone = false;
}

AwEpochTree::~AwEpochTree()
{
}

int AwEpochTree::numberOfGoodEpochs()
{
	int count = 0;
	for (auto e : m_epochs)
		if (!e->rejected)
			count++;

	return count;
}

void AwEpochTree::setFilters(float *filters)
{
	memcpy(m_filters, filters, 3 * sizeof(float));
	m_avgIsDone = false;	// flag to recompute avg when filters change
}

int AwEpochTree::buildEpochs(const AwMarkerList& markers, const QString& label, float pre, float post)
{
	if (m_totalDuration <= 0.)
		return -1;
	clearEpochs();
	for (AwMarker *m : markers) {
		if (m->label() != label)
			continue;
		float pos = m->start() - pre;
		if (pos < 0.) // skip this epoch 
			continue;
		if (m->start() + post > m_totalDuration)
			continue;

		AwEpoch *e = new AwEpoch;
		e->posAndDuration = new AwMarker(label, pos, pre + post);
		e->condition = this;
		m_epochs << e;
	}
	m_zeroPos = pre;
	m_zeroSample = (int)floor(pre * m_channels.first()->samplingRate());
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

float AwEpochTree::epochDuration()
{
	// if no epochs return 0 otherwise return the first epoch duration.
	return m_epochs.isEmpty() ? 0 : m_epochs.first()->posAndDuration->duration();
}

void AwEpochTree::setComputeSettings(AwEpochComputeSettings& settings)
{
	m_computeSettings = settings;
	m_avgIsDone = false;
}

int AwEpochTree::doAverage(bool verbose)
{
	if (!m_isConnected)
		return -1;
	if (m_epochs.isEmpty())
		return -1;
	m_matrix.clear();
	arma::uword nCols = (arma::uword)floor(m_epochs.first()->posAndDuration->duration() * m_channels.first()->samplingRate());
	arma::uword nRows = (arma::uword)m_channels.size();
	fmat epochs_matrix = arma::fmat(nRows, nCols);
	epochs_matrix.zeros();

	QVector<fmat> epochs;

	// skip rejected epochs
	int nEpochs = 0;
	for (AwEpoch *e : m_epochs) {
		if (e->rejected)
			continue;
		requestData(&m_channels, e->posAndDuration->start(), e->posAndDuration->duration());
		
		// apply baseline correction if set in settings
		if (m_computeSettings.useBaselineCorrection) {
			if (m_zeroPos > 0.) {
				for (auto c : m_channels) {
					fvec vec = AwMath::channelToVec(c);
//					if (c->isMEG() || c->isGRAD())
//						vec *= 1e12;
//					else
//						vec *= 1e6;
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
	//				if (c->isMEG() || c->isGRAD())
	//					vec *= 1e-12;
	//				else
	//					vec *= 1e-6;
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
AwEpochDataBuffer *AwEpochTree::createAVGBuffer()
{
	if (!m_avgIsDone)
		return NULL;

	//qDeleteAll(m_avgChannels);
	while (!m_avgChannels.isEmpty())
		delete m_avgChannels.takeLast();
	m_avgChannels.clear();
	for (arma::uword i = 0; i < m_matrix.n_rows; i++) {
		AwEpochAverageChannel *newc = new AwEpochAverageChannel(m_channels.at(i));
		newc->setData(conv_to<fvec>::from(m_matrix.row(i)));
		newc->setErrorType(conv_to<fvec>::from(m_errorType.row(i)));
		newc->setZeroPosition(m_zeroPos);
		m_avgChannels << newc;
	}
	AwEpochDataBuffer *buffer = new AwEpochDataBuffer(&m_avgChannels);
	return buffer;
}

AwEpochAverageChannel *AwEpochTree::createAVGChannel(const QString& label)
{
	QStringList labels = AwChannel::getLabels(m_channels);
	int index = labels.indexOf(label);
	if (index == -1)
		return NULL;

	AwEpochAverageChannel *newc = new AwEpochAverageChannel(m_channels.at(index));

	newc->setData(conv_to<fvec>::from(m_matrix.row(index)));
	newc->setErrorType(conv_to<fvec>::from(m_errorType.row(index)));
	newc->setZeroPosition(m_zeroPos);
	newc->setCondition(this);
	return newc;
}

///
/// Load the epoch at index index.
/// Load data into channels associated to the condition.
/// return -1 if failed.
int AwEpochTree::loadEpoch(int index)
{
	if (index < 0 || index >= m_epochs.size())
		return -1;
	// check if filters have changed
	AwChannel *first = m_channels.first();
	if (first->lowFilter() != m_filters[0] || first->highFilter() != m_filters[1] 
		|| first->notch() != m_filters[2]) {  // apply filters settings to all channels
		for (auto c : m_channels) {
			c->setLowFilter(m_filters[0]);
			c->setHighFilter(m_filters[1]);
			c->setNotch(m_filters[2]);
		}
	}
	requestData(&m_channels, m_epochs.at(index)->posAndDuration);
	return 0;
}

int AwEpochTree::loadEpoch(AwEpoch *e)
{
	int index = m_epochs.indexOf(e);
	if (index == -1)
		return index;
	return loadEpoch(index);
}