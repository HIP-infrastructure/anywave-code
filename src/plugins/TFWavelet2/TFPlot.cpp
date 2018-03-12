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
#include "TFPlot.h"
#include <graphics/AwQwtColorMap.h>
#include <qwt_color_map.h>
#include <graphics/AwColorMap.h>
#include <qwt_scale_engine.h>
#include <qwt_plot_layout.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_widget.h>
#include <math.h>
#include <QtMath>

TFPlot::TFPlot(TFSettings *settings, DisplaySettings *ds, AwChannel *channel, QWidget *parent) : QwtPlot(parent)
{	
	// Set minimum heigth to 200
	setMinimumSize(QSize(0, 200));

	m_channel = channel;
	m_positionInData = 0.;
	enableAxis(QwtPlot::yRight, false);
	enableAxis(QwtPlot::yLeft, false);
	enableAxis(QwtPlot::xBottom, false);
	m_spectro = new QwtPlotSpectrogram();
    m_spectro->setRenderThreadCount(0); // use system specific thread count
	ds->colorMap =  AwColorMap::Parula;
	m_spectro->attach(this);
	plotLayout()->setAlignCanvasToScales(true);
	setAutoReplot(false);
	m_matrix = new QwtMatrixRasterData;
	m_matrix->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
	m_settings = settings;
	m_displaySettings = ds;
	m_displayCopy.gain = ds->gain;
	m_displayCopy.colorMap = ds->colorMap;
	m_displayCopy.logScale = ds->logScale;
	m_displayCopy.zInterval = ds->zInterval;
	m_displayCopy.normalization = ds->normalization;

	// building the left scale widget
	m_freqScaleWidget = new QwtScaleWidget(QwtScaleDraw::LeftScale);
	m_freqScaleWidget->setContentsMargins(0, 0, 0, 0);
	m_freqScaleWidget->setBorderDist(1, 1);
	m_freqScaleWidget->setSpacing(5);
	m_freqScaleWidget->setMargin(5);
	m_freqScaleWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	QwtText freqText(QObject::tr("Freq. (Hz)"));
	freqText.setColor(Qt::black);
	freqText.setFont(QFont("Arial", 10));
	m_freqScaleWidget->setTitle(freqText);
	// default to Log scale transformation
	m_freqScaleWidget->setTransformation(new QwtLogTransform());
	m_freqScaleWidget->hide();
	
	// building the colormap widget
	m_colorMapWidget = new QwtScaleWidget(QwtScaleDraw::RightScale);
	m_colorMapWidget->setContentsMargins(0, 0, 0, 0);
	m_colorMapWidget->setBorderDist(1, 1);
	m_colorMapWidget->setSpacing(5);
	m_colorMapWidget->setMargin(5);
	m_colorMapWidget->setColorBarEnabled(true);
	m_colorMapWidget->setColorBarWidth(25);
	m_colorMapWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_colorMapWidget->hide();
	
	// building picker
	m_picker = new TFPicker((QwtPlotCanvas *)canvas(), m_freqScaleWidget);
	m_picker->setStateMachine(new QwtPickerDragRectMachine);
	m_picker->setRubberBand(QwtPicker::RectRubberBand);
	m_picker->setTrackerMode(QwtPicker::AlwaysOn);
	connect(m_picker, SIGNAL(samplesSelected(int, int)), this, SLOT(select(int, int)));
	connect(m_picker, SIGNAL(selectionDone(int, int, float, int, int)), this, SLOT(select(int, int, float, int, int)));
	applyColorMap();

	// default to log scale for freq
	setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine());
	updateFreqScale(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	m_raw_data = NULL;
}

TFPlot::~TFPlot()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void TFPlot::select(int start, int dur, float fmin, int lrow, int hrow)
{
	emit selectionMade(m_channel, m_positionInData, start, dur, fmin, lrow, hrow);
}

void TFPlot::select(int start, int duration)
{
	float real_start = m_positionInData + (start / m_channel->samplingRate());
	float real_duration = (float)(duration  / m_channel->samplingRate());
	emit selectionDone(real_start, real_duration);
}

/// END SLOTS
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TFPlot::setNewData(float position, const QVector<double>& data, int row, int col)
{
	m_positionInData = position;
	m_raw_rows = row;
	m_raw_cols = col;
	if (m_raw_data)
		delete[] m_raw_data;
	m_raw_data = new double[row * col];
	memcpy(m_raw_data, data.data(), row * col * sizeof(double));
	applyNormalization();
}

void TFPlot::showFreqScale(bool flag)
{
	m_freqScaleWidget->setVisible(flag);
}

void TFPlot::showColorMapScale(bool flag)
{
	m_colorMapWidget->setVisible(flag);
}

void TFPlot::updateFreqScale(float min, float max, float step)
{
	m_settings->freq_min = min;
	m_settings->freq_max = max;
	m_settings->step = step;

	QVector<float> freqs = m_settings->freqs;
	double fmin = freqs.at(0);
	double fmax = freqs.at(freqs.size() - 1);
	QList<double> leftTicks[QwtScaleDiv::NTickTypes];
	leftTicks[QwtScaleDiv::MajorTick] << fmin << freqs.at((freqs.size() / 2) - 1) << fmax;
	QwtScaleDiv divL(leftTicks[QwtScaleDiv::MajorTick].first(), leftTicks[QwtScaleDiv::MajorTick].last(), leftTicks);
	m_freqScaleWidget->scaleDraw()->setScaleDiv(divL);
	m_picker->setFreqScaleInterval(fmin, fmax);
	setAxisScale(QwtPlot::yLeft, fmin, fmax);
	setAxisAutoScale(QwtPlot::yLeft);
	axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating, true);
	plotLayout()->setAlignCanvasToScales(true);
	m_freqScaleWidget->repaint();
}


void TFPlot::updateDisplaySettings()
{
	if (m_displaySettings->normalization != m_displayCopy.normalization) {
		m_displayCopy.normalization = m_displaySettings->normalization;
		applyNormalization();
	}
	if (m_displaySettings->zInterval != m_displayCopy.zInterval) {
		m_displayCopy.zInterval = m_displaySettings->zInterval;
		updateZScale();
	}
	if (m_displaySettings->colorMap != m_displayCopy.colorMap) {
		m_displayCopy.colorMap = m_displaySettings->colorMap;
		applyColorMap();
	}
	if (m_displaySettings->gain != m_displayCopy.gain) {
		m_displayCopy.gain = m_displaySettings->gain;
		updateZScale();
	}
	if (m_displaySettings->logScale != m_displayCopy.logScale) {
		m_displayCopy.logScale = m_displaySettings->logScale;
		if (m_displayCopy.logScale) {
			setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine());
			m_freqScaleWidget->setTransformation(new QwtLogTransform());
		}
		else {
			setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
			m_freqScaleWidget->setTransformation(new QwtNullTransform());
			
		}
		updateZScale();
	}
}

void TFPlot::applyNormalization()
{
	int r = m_raw_rows;
	int c = m_raw_cols;
	QVector<double> matrix(r * c);
	double *data = matrix.data();

	switch (m_displaySettings->normalization) {
	case DisplaySettings::N10log10Divisive:
		for (int i = 0; i < r; i++) {
			double mean = 0.;
			for (int j = 0; j < c; j++)
				mean += m_raw_data[i * c + j];
			mean /= c;
			for (int j = 0; j < c; j++)
				data[i * c + j] = 10 * log10(m_raw_data[i * c + j] / mean);
		}
		break;
	case DisplaySettings::NoNorm:
		// restore raw data
		memcpy(data, m_raw_data, r * c * sizeof(double));
		break;
	case DisplaySettings::ZScore:
		for (int i = 0; i < r; i++) {
			double mean = 0.;
			double Exx2 = 0.;
			for (int j = 0; j < c; j++)
				mean += m_raw_data[i * c + j];
			mean /= c;
			for (int j = 0; j < c; j++)
				Exx2 += (m_raw_data[i * c + j] - mean) *  (m_raw_data[i * c + j] - mean);
			double sd = sqrt(Exx2 / c);
			for (int j = 0; j < c; j++)
				data[i * c + j] = (m_raw_data[i * c + j] - mean) / sd;
		}
		break;
	}
	// compute min max
	m_min = m_max = data[0];

	for (int i = 1; i < r * c; i++) {
		if (data[i] < m_min)
			m_min = data[i];
		if (data[i] > m_max)
			m_max = data[i];
	}
	m_matrix->setValueMatrix(matrix, c);
	m_matrix->setInterval(Qt::XAxis, QwtInterval(1, m_matrix->numColumns()));
	m_matrix->setInterval(Qt::YAxis, QwtInterval(1, m_matrix->numRows()));
	setAxisScale(QwtPlot::xBottom, 0, m_matrix->numColumns());
	m_spectro->setData(m_matrix);

	// update z interval based on the Scale chosen
	updateZScale();
//	refresh();
}

void TFPlot::updateZScale()
{
	// apply gain
	double max = std::max(std::abs(m_min), std::abs(m_max));
	switch (m_displaySettings->zInterval) {
	case DisplaySettings::MinToMax:
		ZInterval = QwtInterval(m_min, m_max * m_displaySettings->gain);
		break;
	case DisplaySettings::ZeroToMax:
		ZInterval = QwtInterval(0, m_max * m_displaySettings->gain);
		break;
	case DisplaySettings::MaxToMax:
		ZInterval = QwtInterval(-max, m_max * m_displaySettings->gain);
		break;
	}
	setAxisScale(QwtPlot::yRight, ZInterval.minValue(), ZInterval.maxValue());
	m_matrix->setInterval(Qt::ZAxis, ZInterval);
	QList<double> rTicks[QwtScaleDiv::NTickTypes];
	rTicks[QwtScaleDiv::MajorTick] << ZInterval.minValue() << (ZInterval.maxValue() - ZInterval.minValue()) / 2 << ZInterval.maxValue();
	QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	m_colorMapWidget->setColorMap(ZInterval, AwQwtColorMap::newMap(m_displaySettings->colorMap));
	m_colorMapWidget->scaleDraw()->setScaleDiv(divR);

	updateFreqScale(m_settings->freq_min, m_settings->freq_max, m_settings->step);

	m_colorMapWidget->repaint();
	m_spectro->invalidateCache();
	replot();
}


void TFPlot::applyColorMap()
{
	m_spectro->setColorMap(AwQwtColorMap::newMap(m_displaySettings->colorMap));
	m_colorMapWidget->setColorMap(ZInterval, AwQwtColorMap::newMap(m_displaySettings->colorMap));
	m_colorMapWidget->repaint();
	m_spectro->invalidateCache();
	replot();

	switch (m_displaySettings->colorMap)
	{
	case AwColorMap::BlueBlackRed:
		m_picker->setTextColor(QColor(Qt::white));
		break;
	case AwColorMap::Jet:
		m_picker->setTextColor(QColor(Qt::red));
		break;
	case AwColorMap::FireAndIce:
		m_picker->setTextColor(QColor(Qt::white));
		break;
	case AwColorMap::Gray:
		m_picker->setTextColor(QColor(Qt::red));
		break;
	}
}