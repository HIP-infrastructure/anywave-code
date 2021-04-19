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
#include "TFColorMapWidget.h"

TFPlot::TFPlot(TFSettings *settings, DisplaySettings *ds, AwChannel *channel, QWidget *parent) : QwtPlot(parent)
{	
	// Set minimum heigth to 200
	setMinimumSize(QSize(50, 200));
	setLineWidth(0);

	m_channel = channel;
	m_positionInData = 0;
	m_zgain = 0;
	enableAxis(QwtPlot::yRight, false);
	enableAxis(QwtPlot::yLeft, false);
	enableAxis(QwtPlot::xBottom, false);
	m_spectro = new QwtPlotSpectrogram();
    m_spectro->setRenderThreadCount(0); // use system specific thread count
	ds->colorMap =  AwColorMap::Parula;
	m_spectro->attach(this);
	plotLayout()->setAlignCanvasToScales(true);
	plotLayout()->setCanvasMargin(0);
	setAutoReplot(false);
	m_matrix = new QwtMatrixRasterData;
	m_matrix->setResampleMode(QwtMatrixRasterData::BilinearInterpolation);
	m_settings = settings;
	m_displaySettings = ds;

	m_displayCopy.colorMap = ds->colorMap;
	m_displayCopy.logScale = ds->logScale;
	m_displayCopy.zInterval = ds->zInterval;
	m_displayCopy.normalization = ds->normalization;

	// building the left scale widget
	m_freqScaleWidget = new QwtScaleWidget(QwtScaleDraw::LeftScale);
	m_freqScaleWidget->setContentsMargins(0, 0, 0, 0);
	m_freqScaleWidget->setBorderDist(1, 1);
	m_freqScaleWidget->setSpacing(5);
	m_freqScaleWidget->setMargin(0);
	m_freqScaleWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	QwtText freqText(QObject::tr("Freq. (Hz)"));
	freqText.setColor(Qt::black);
	freqText.setFont(QFont("Arial", 10));
	m_freqScaleWidget->setTitle(freqText);
	// default to Log scale transformation
	m_freqScaleWidget->setTransformation(new QwtLogTransform());

	// default to log scale for freq
	setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
	axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating, true);

	// building the colormap widget
	m_colorMapWidget = new TFColorMapWidget(m_displaySettings, this);
	connect(m_colorMapWidget, &TFColorMapWidget::ZGainChanged, this, &TFPlot::updateGainZInterval);

	//m_colorMapWidget = new QwtScaleWidget(QwtScaleDraw::RightScale);
	//m_colorMapWidget->setContentsMargins(0, 0, 0, 0);
	//m_colorMapWidget->setBorderDist(1, 1);
	//m_colorMapWidget->setSpacing(5);
	//m_colorMapWidget->setMargin(5);
	//m_colorMapWidget->setColorBarEnabled(true);
	//m_colorMapWidget->setColorBarWidth(25);
	//m_colorMapWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	// m_colorMapWidget->hide();
	
	// building picker
	m_picker = new TFPicker((QwtPlotCanvas*)canvas(), m_settings);
	m_picker->setStateMachine(new QwtPickerDragRectMachine);
	m_picker->setRubberBand(QwtPicker::RectRubberBand);
	m_picker->setTrackerMode(QwtPicker::AlwaysOn);
	connect(m_picker, SIGNAL(samplesSelected(int, int)), this, SLOT(select(int, int)));
	connect(m_picker, SIGNAL(selectionDone(int, int, float, int, int)), this, SLOT(select(int, int, float, int, int)));
	applyColorMap();

	updateFreqScale(m_settings->freq_min, m_settings->freq_max, m_settings->step);
}

TFPlot::~TFPlot()
{
}

QSize TFPlot::sizeHint() const
{
	return QSize(400, 800);
}

void TFPlot::updateGainZInterval(double gain)
{
	m_zgain = gain;
	updateZInterval(m_ZInterval);
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

	QList<double> rTicks[QwtScaleDiv::NTickTypes];
	auto s = std::abs((max - min))  / 4;

	rTicks[QwtScaleDiv::MajorTick] << min << std::floor(min + 1. * s) << std::floor(min + 2. * s) << std::floor(min + 3. * s) << max;
	QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	m_freqScaleWidget->scaleDraw()->setScaleDiv(divR);
	m_freqScaleWidget->repaint();

//	setAxisScale(QwtPlot::yLeft, min, max, step);
	m_picker->setFreqScaleInterval(min, max);
	replot();
}


void TFPlot::updateDisplaySettings()
{
	if (m_displaySettings->colorMap != m_displayCopy.colorMap) {
		m_displayCopy.colorMap = m_displaySettings->colorMap;
		applyColorMap();
	}
	if (m_displaySettings->logScale != m_displayCopy.logScale) {
		m_displayCopy.logScale = m_displaySettings->logScale;
		if (m_displayCopy.logScale) {
			setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
			setAxisScale(QwtPlot::yLeft, m_settings->freq_min, 	m_settings->freq_max, m_settings->step);
			m_freqScaleWidget->setTransformation(new QwtLogTransform());
		}
		else {
			setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
			m_freqScaleWidget->setTransformation(new QwtNullTransform());
			setAxisScale(QwtPlot::yLeft, m_settings->freq_min, m_settings->freq_max, m_settings->step);
		}
		updateFreqScale(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	}
}

void TFPlot::updateZScale()
{
//	double max = std::max(std::abs(m_min), std::abs(m_max));
//	switch (m_displaySettings->zInterval) {
//	case DisplaySettings::MinToMax:
//		m_ZInterval = QwtInterval(m_min, m_max);
//		break;
//	case DisplaySettings::ZeroToMax:
//		m_ZInterval = QwtInterval(0, m_max);
//		break;
//	case DisplaySettings::MaxToMax:
//		m_ZInterval = QwtInterval(-max, max);
//		break;
//	}
//	if (m_ZInterval.maxValue() <= m_ZInterval.minValue())
//		return;
//	setAxisScale(QwtPlot::yRight, m_ZInterval.minValue(), m_ZInterval.maxValue());
//	m_matrix->setInterval(Qt::ZAxis, m_ZInterval);
//	m_colorMapWidget->setDataZInterval(m_ZInterval);
////	m_realMin = ZInterval.minValue();
////	m_realMax = ZInterval.maxValue();
//	m_min = m_ZInterval.minValue();
//	m_max = m_ZInterval.maxValue();
//	m_spectro->invalidateCache();
//	replot();
}

void TFPlot::setDataMatrix(const mat& matrix, float position)
{
	QVector<double> vec(matrix.n_rows * matrix.n_cols);
	double* data = vec.data();
	m_positionInData = position;
	m_min = matrix.min();
	m_max = matrix.max();
	for (auto r = 0; r < matrix.n_rows; r++)
		for (auto c = 0; c < matrix.n_cols; c++)
			*data++ = matrix(r, c);

	m_matrix->setValueMatrix(vec, matrix.n_cols);
	m_matrix->setInterval(Qt::XAxis, QwtInterval(0, m_matrix->numColumns()));
	m_matrix->setInterval(Qt::YAxis, QwtInterval(0, m_matrix->numRows()));
	setAxisScale(QwtPlot::xBottom, 0, m_matrix->numColumns());

	setAxisScale(QwtPlot::yLeft, m_settings->freq_min, m_settings->freq_max + m_settings->freq_min - 1);
//	setAxisScale(QwtPlot::yLeft, 1, matrix.n_rows, 1.);
	m_spectro->setData(m_matrix);
	replot();
}


void TFPlot::applyColorMap()
{
	m_spectro->setColorMap(AwQwtColorMap::newMap(m_displaySettings->colorMap));
	m_spectro->invalidateCache();
	replot();
}

void TFPlot::resetZScale()
{
	m_min = m_realMin;
	m_max = m_realMax;
	m_colorMapWidget->reset();
}

void TFPlot::updateZInterval(const QwtInterval& interval)
{
	// check that interval is correct
	m_ZInterval = interval;

	// apply z gain
	double maxValue = m_ZInterval.maxValue();
	if (m_zgain < 100)
		maxValue = m_ZInterval.maxValue() * (100 - m_zgain) / 100.;

	QwtInterval newInterval = m_ZInterval;
	newInterval.setMaxValue(maxValue);
	m_matrix->setInterval(Qt::ZAxis, newInterval);
	m_spectro->invalidateCache();
	replot();
	m_colorMapWidget->setZInterval(m_ZInterval);
}