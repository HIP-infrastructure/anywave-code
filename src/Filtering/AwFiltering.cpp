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
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <AwFiltering.h>
#include <QtConcurrentMap>
#include "AwButterWorth.h"
#include <QtMath>
#include "DspFilters/Dsp.h"
#include "DspFilters/Butterworth.h"

#include <math/AwMath.h>

struct decimation {
	AwChannel *c;
	int f;
	decimation(AwChannel *channel, int factor) { c = channel; f = factor; }
};
struct down_sampling {
	AwChannel *c;
	float freq;
	down_sampling(AwChannel *channel, float f) { c = channel; freq = f; }
};

/* function used in QtConcurrent map for filtering data */
AwChannel *filterChannel(AwChannel *chan);
/* function used in QtConccurent map to filter data using Chebychev 1 order 8 */
AwChannel *filterCheby1_8AA(AwChannel *chan);
/* function used in QtConccurent map to filter data using ButterWorth order 4 */
AwChannel *filterButterWorthAA(AwChannel *chan);
///* function to filter using a specified filter and order */
//AwChannel *filterChannelUsingFilterAndOrder(filtering_struct *fs);
/* function used in QtConcurrent map to decimate data */
void decimateChannel(decimation *d);
/* function for notch filtering */
AwChannel *notchFilterChannel(AwChannel *chan);
/* function for down sampling a channel */
AwChannel *downSamplingChannel(down_sampling *ds);

void AwFiltering::downSample(AwChannel *chan, float freq)
{
	float sr = chan->samplingRate();
	if (freq >= sr)
		return;

	int decim_factor = (int)floor(sr / freq);
	// recompute target Sampling rate with new decim factor.
	float new_sr = sr / (float)decim_factor;
	// check if current low filter is lower than anti aliasing filter required
	float aa = freq / 3;
	if (aa > chan->lowFilter()) { // need to apply anti aliasing filter
		filterButterWorthAA(chan);
	}
	chan->decimate(decim_factor);
	filterChannel(chan); // apply filters.
	chan->setSamplingRate(new_sr);
}

void AwFiltering::downSample(const AwChannelList& channels, float freq)
{
	if (channels.isEmpty())
		return;
	
	QList<down_sampling *> toProcess;
	foreach(AwChannel *c, channels) {
		if (freq < c->samplingRate()) {
			toProcess << new down_sampling(c, freq);
		}
	}

	QFuture<void> res = QtConcurrent::mapped(toProcess, downSamplingChannel);
	res.waitForFinished();
	while (!toProcess.isEmpty())
		delete toProcess.takeLast();
}

AwChannel *downSamplingChannel(down_sampling *ds)
{
	float sr = ds->c->samplingRate();
	float freq = ds->freq;
	if (freq >= sr)
		return ds->c;

	int decim_factor = (int)floor(sr / freq);
	// recompute target Sampling rate with new decim factor.
	float new_sr = sr / (float)decim_factor;

	float aa = sr / 3;
	filterButterWorthAA(ds->c);
	ds->c->decimate(decim_factor);
	ds->c->setSamplingRate(new_sr);

	return ds->c;
}

void AwFiltering::filter(const AwChannelList& channels)
{
	if (channels.isEmpty())
		return;

	AwChannelList channelsToFilter;

	for (auto chan :  channels)	{
		if (chan->lowFilter() > 0 || chan->highFilter() > 0 || chan->notch() > 0) {
			channelsToFilter.append(chan);
		}
	}

	if (channelsToFilter.isEmpty())
		return;

	QFuture<void> res = QtConcurrent::mapped(channelsToFilter, filterChannel);
	res.waitForFinished();
}

void AwFiltering::filter(AwChannelList* channels)
{
	AwFiltering::filter(*channels);
}

void AwFiltering::filter(AwChannelList* channels, AwFilteringOptions *fo)
{
	if (fo == NULL)
		return;
	AwChannel::clearFilters(*channels);
	fo->setFilters(*channels);
	AwFiltering::filter(*channels);
}


//void AwFiltering::notch(const AwChannelList& channels)
//{
//	if (channels.isEmpty())
//		return;
//
//	AwChannelList channelsToFilter;
//
//	foreach (AwChannel *chan, channels)	{
//		if (chan->notch() > 0)
//			channelsToFilter.append(chan);
//	}
//
//	if (channelsToFilter.isEmpty())
//		return;
//
//	QFuture<void> res = QtConcurrent::mapped(channelsToFilter, notchFilterChannel);
//	res.waitForFinished();
//}

//void AwFiltering::notch(AwChannelList *channels)
//{
//	AwFiltering::notch(*channels);
//}

///
/// decimate channels
/// WARNING : we assume here that the signals are not filtered (raw data).
/// A chebychev 1 filter with order 8 will be applied before decimation. (like in MATLAB).
///
void AwFiltering::decimate(const AwChannelList& channels, int factor)
{
	// filter all channels
	AwChannelList toFilter = channels;
	QFuture<void> future = QtConcurrent::map(toFilter, filterButterWorthAA);
	future.waitForFinished();

	QList<decimation *> decims;
	foreach (AwChannel *c, toFilter)
		decims <<  new decimation(c, factor);

	future = QtConcurrent::map(decims, decimateChannel);
	future.waitForFinished();
	while (!decims.isEmpty())
		delete decims.takeLast();
}

//AwChannel *notchFilterChannel(AwChannel *c)
//{
//	// check for data pointer. If null pointer => don't filter
//	// null data pointer can mean a virtual channel that is not computed yet.
//	if(c->data() == NULL)
//		return c;
//	Dsp::SimpleFilter <Dsp::ChebyshevI::BandStop <3>, 1> f;
//	f.setup(3, c->samplingRate(), c->notch(), 4, 1);	// 4Hz band width
//	float *data[1];
//	data[0] = c->data();
//	f.process(c->dataSize(), data);
//	c->setDataReady();
//	return c;
//}

QVector<float> AwFiltering::pad_left(AwChannel *channel)
{
	qint64 sample_1s = qCeil(channel->samplingRate());
	qint64 padding = 3 * sample_1s;

	QVector<float> vector(padding); 
	vector.fill(channel->data()[0]);
	return vector;
}

QVector<float>  AwFiltering::pad_right(AwChannel *channel)
{
	qint64 sample_1s = qCeil(channel->samplingRate());
	qint64 padding = 3 * sample_1s;
	//while (padding > channel->dataSize())
	//	padding -= sample_1s;

	QVector<float> vector(padding); 
	qint64 last_sample = channel->dataSize() - 1;
	vector.fill(channel->data()[last_sample]);
	return vector;


	//// one sample won't be used, so check the special case when padding lenght = signal length.
	//if (padding == channel->dataSize())
	//	padding--;
	//if (padding <= 0) {
	//	vector.resize(3 * sample_1s);
	//	vector.fill(channel->data()[last_sample]);
	//	return vector;
	//}
	//// skip 1st sample of original data
	//float sample_x0 = channel->data()[last_sample];

	//// copy 3s of signal in reverse order
	//for (int i = last_sample - 1;  i > last_sample - padding - 1; i--)
	//	vector << channel->data()[i];

	//float *pad_data = vector.data();
	//for (int i = 0; i < vector.size(); i++)
	//	pad_data[i] = 2 * sample_x0 - pad_data[i];

	//return vector;
}


AwChannel *filterCheby1_8AA(AwChannel *chan)
{
	// check for data pointer. If null pointer => don't filter
	// null data pointer can mean a virtual channel that is not computed yet.
	if (chan->data() == NULL)
		return chan;
	QVector<float> pad_l = AwFiltering::pad_left(chan);
	QVector<float> pad_r = AwFiltering::pad_right(chan);
	QVector<float> signal = chan->toVector();
	QVector<float> total = pad_l + signal + pad_r;

	arma::fvec vdata(total.data(), total.size());
	float *data[1];
	data[0] = vdata.memptr();
	Dsp::SimpleFilter <Dsp::ChebyshevI::LowPass <4>, 1> f;
	f.setup(4, chan->samplingRate(), chan->samplingRate() / 3, 0.5);
	f.process(vdata.size(), data);
	vdata = arma::flipud(vdata);
	f.process(vdata.size(), data);
	vdata = arma::flipud(vdata);
	// copy filtered data to original signal
	memcpy(chan->data(), &vdata.memptr()[pad_l.size()], signal.size() * sizeof(float));
	chan->setDataReady();
	return chan;
}

AwChannel *filterButterWorthAA(AwChannel *chan)
{
	// check for data pointer. If null pointer => don't filter
	// null data pointer can mean a virtual channel that is not computed yet.
	if (chan->data() == NULL)
		return chan;
	QVector<float> pad_l = AwFiltering::pad_left(chan);
	QVector<float> pad_r = AwFiltering::pad_right(chan);
	QVector<float> signal = chan->toVector();
	QVector<float> total = pad_l + signal + pad_r;

	arma::fvec vdata(total.data(), total.size());
	float *data[1];
	data[0] = vdata.memptr();
	Dsp::SimpleFilter <Dsp::Butterworth::LowPass <2>, 1> f;
	f.setup(2, chan->samplingRate(), chan->samplingRate() / 3);
	f.process(vdata.size(), data);
	vdata = arma::flipud(vdata);
	f.process(vdata.size(), data);
	vdata = arma::flipud(vdata);
	// copy filtered data to original signal
	memcpy(chan->data(), &vdata.memptr()[pad_l.size()], signal.size() * sizeof(float));
	chan->setDataReady();
	return chan;
}

// fonction de filtrage pour QtConcurrent
AwChannel *filterChannel(AwChannel *chan)
{

	// check for data pointer. If null pointer => don't filter
	// null data pointer can mean a virtual channel that is not computed yet.
	if(chan->data() == NULL)
		return chan;
#ifdef OLD_FILTER
	LinearFilterPtr highPassFilter;
	LinearFilterPtr lowPassFilter;
#endif
	QVector<float> pad_l = AwFiltering::pad_left(chan);
	QVector<float> pad_r = AwFiltering::pad_right(chan);
	QVector<float> signal = chan->toVector();
	QVector<float> total = pad_l + signal + pad_r;
			
	arma::fvec vdata(total.data(), total.size());
#ifndef OLD_FILTER
	float *data[1];
	data[0] = vdata.memptr();
#endif

	// apply notch first if set
	if (chan->notch() > 0) {
		Dsp::SimpleFilter <Dsp::ChebyshevI::BandStop <4>, 1> f;
		f.setup(4, chan->samplingRate(), chan->notch(), 4, 1);	// 4Hz band width
		f.process(vdata.size(), data);
	}

	if (chan->lowFilter() > 0)	{
#ifndef OLD_FILTER
			Dsp::SimpleFilter <Dsp::Butterworth::LowPass <2>, 1> f;
			f.setup(2, chan->samplingRate(), chan->lowFilter());
			f.process(vdata.size(), data);
			vdata = arma::flipud(vdata);
			f.process(vdata.size(), data);
			vdata = arma::flipud(vdata);
#else
		// Peut etre pas optimal de calculer un filtre pour CHAQUE canal parcouru ...  A VOIR
		lowPassFilter = BW_Compute(BW_LowPass, BW_DefaultOrder, chan->lowFilter(), chan->samplingRate());
		//float *dataFiltered = BW_Apply(lowPassFilter, chan->data(), chan->dataSize());
		float *dataFiltered = BW_Apply(lowPassFilter, vdata.memptr(), vdata.size());

		memcpy(vdata.memptr(), dataFiltered,  vdata.size() * sizeof(float));
		delete[] dataFiltered;
#endif
	}
	if (chan->highFilter() > 0)	{
#ifdef OLD_FILTER
		highPassFilter = BW_Compute(BW_HighPass, BW_DefaultOrder, chan->highFilter(), chan->samplingRate());
		float *dataFiltered = BW_Apply(highPassFilter, vdata.memptr(), vdata.size());
		memcpy(vdata.memptr(), dataFiltered,  vdata.size() * sizeof(float));
		delete[] dataFiltered;
#else
		Dsp::SimpleFilter <Dsp::Butterworth::HighPass <2>, 1> f;
		f.setup(2, chan->samplingRate(), chan->highFilter());
		f.process(vdata.size(), data);
		vdata = arma::flipud(vdata);
		//	data[0] = vdata.memptr();
		f.process(vdata.size(), data);
		vdata = arma::flipud(vdata);
#endif
	}
	// copy filtered data to original signal
	memcpy(chan->data(), &vdata.memptr()[pad_l.size()], signal.size() * sizeof(float));
	chan->setDataReady();
	return chan;
}


void decimateChannel(decimation *d)
{
	d->c->decimate(d->f);
}

