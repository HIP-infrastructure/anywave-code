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
	m_positionInData = 0.;
	enableAxis(QwtPlot::yRight, false);
	enableAxis(QwtPlot::yLeft, false);
	enableAxis(QwtPlot::xBottom, false);
	m_spectro = new QwtPlotSpectrogram();
    m_spectro->setRenderThreadCount(0); // use system specific thread count
	ds->colorMap =  AwColorMap::Parula;
	m_spectro->attach(this);
	plotLayout()->setAlignCanvasToScales(true);
	//plotLayout()->setAlignCanvasToScales(false);
	plotLayout()->setCanvasMargin(0);
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

	// default to log scale for freq
	setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine(10));
	axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating, true);

	// building the colormap widget
	m_colorMapWidget = new TFColorMapWidget(m_displaySettings, this);
	connect(m_colorMapWidget, SIGNAL(applyMinMaxToAll(double, double)), this, SIGNAL(applyMinMaxToAll(double, double)));
	connect(m_colorMapWidget, &TFColorMapWidget::newZInterval, this, &TFPlot::updateZInterval);

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
	m_picker = new TFPicker((QwtPlotCanvas*)canvas());
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

void TFPlot::setMinMaxZScale(double min, double max)
{
	m_colorMapWidget->forceMinMax(min, max);

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


//void TFPlot::setNewData(float position, const QVector<double>& data, int row, int col)
void TFPlot::setNewData(float position, TFParam *param)
{
	m_positionInData = position;
	m_rawMat = param->data;
	m_baselineMat = param->baselineData;
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

	QList<double> rTicks[QwtScaleDiv::NTickTypes];
	auto s = std::abs((max - min))  / 4;

	rTicks[QwtScaleDiv::MajorTick] << min << std::floor(min + 1. * s) << std::floor(min + 2. * s) << std::floor(min + 3. * s) << max;
	QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	m_freqScaleWidget->scaleDraw()->setScaleDiv(divR);
	m_freqScaleWidget->repaint();

	setAxisScale(QwtPlot::yLeft, min, max, step);
	m_picker->setFreqScaleInterval(min, max);
	replot();

	//QVector<float> freqs = m_settings->freqs;
	//double fmin = freqs.at(0);
	//double fmax = freqs.at(freqs.size() - 1);
	//QList<double> leftTicks[QwtScaleDiv::NTickTypes];
	//auto start = fmin;
	//int interval = (int)std::floor((fmax + fmin) / 8);
	//while (start  < fmax) {
	//	leftTicks[QwtScaleDiv::MajorTick] << start;
	//	start += interval;
	//}
	//leftTicks[QwtScaleDiv::MajorTick] << fmax;
	//QwtScaleDiv divL(leftTicks[QwtScaleDiv::MajorTick].first(), leftTicks[QwtScaleDiv::MajorTick].last(), leftTicks);
	//m_freqScaleWidget->scaleDraw()->setScaleDiv(divL);
	//m_picker->setFreqScaleInterval(fmin, fmax);
	//setAxisScale(QwtPlot::yLeft, fmin, fmax);
	//setAxisAutoScale(QwtPlot::yLeft);
	//axisScaleEngine(QwtPlot::yLeft)->setAttribute(QwtScaleEngine::Floating, true);
	//plotLayout()->setAlignCanvasToScales(true);
	//m_freqScaleWidget->repaint();
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
	//if (m_displaySettings->gain != m_displayCopy.gain) {
	//	m_displayCopy.gain = m_displaySettings->gain;
	//	updateZScale();
	//}
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
		//updateZScale();
		updateFreqScale(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	}
}

void TFPlot::applyNormalization()
{
	m_mat = mat(m_rawMat.memptr(), m_rawMat.n_rows, m_rawMat.n_cols, true);
	QVector<double> matrix(m_mat.n_rows * m_mat.n_cols);
	double *data = matrix.data();

	switch (m_displaySettings->normalization) {
	case DisplaySettings::N10log10Divisive:
		// check for baseline correction
		if (!m_baselineMat.is_empty()) {
			for (auto i = 0; i < m_mat.n_rows; i++)
				m_mat.row(i) = 10 * log10(m_mat.row(i) / arma::mean(m_baselineMat.row(i)));
		}
		else {
			for (auto i = 0; i < m_mat.n_rows; i++)
				m_mat.row(i) = 10 * log10(m_mat.row(i) / arma::mean(m_mat.row(i)));
		}
		break;
	case DisplaySettings::NoNorm:
		break;
	case DisplaySettings::ZScore:
		if (!m_baselineMat.is_empty()) {
			for (auto i = 0; i < m_mat.n_rows; i++) {
				m_mat.row(i) -= arma::mean(m_baselineMat.row(i));
				m_mat.row(i) /= arma::stddev(m_baselineMat.row(i));
			}
		}
		else {
			for (auto i = 0; i < m_mat.n_rows; i++) {
				m_mat.row(i) -= arma::mean(m_mat.row(i));
				m_mat.row(i) /= arma::stddev(m_mat.row(i));
			}
		}
		break;
	}
	// store min and max
	m_min = m_mat.min();
	m_max = m_mat.max();
	for (auto r = 0; r < m_mat.n_rows; r++)
		for (auto c = 0; c < m_mat.n_cols; c++)
			*data++ = m_mat(r, c);

	m_matrix->setValueMatrix(matrix, m_mat.n_cols);
	m_matrix->setInterval(Qt::XAxis, QwtInterval(1, m_matrix->numColumns()));
	m_matrix->setInterval(Qt::YAxis, QwtInterval(1, m_matrix->numRows()));
	setAxisScale(QwtPlot::xBottom, 0, m_matrix->numColumns());
	m_spectro->setData(m_matrix);

	// update z interval based on the Scale chosen
	updateZScale();
}

void TFPlot::updateZScale()
{
	// apply gain
	double max = std::max(std::abs(m_min), std::abs(m_max));
	switch (m_displaySettings->zInterval) {
	case DisplaySettings::MinToMax:
		//ZInterval = QwtInterval(m_min, m_max * m_displaySettings->gain);
		ZInterval = QwtInterval(m_min, m_max);
		break;
	case DisplaySettings::ZeroToMax:
		//ZInterval = QwtInterval(0, m_max * m_displaySettings->gain);
		ZInterval = QwtInterval(0, m_max);
		break;
	case DisplaySettings::MaxToMax:
		//ZInterval = QwtInterval(-max, m_max * m_displaySettings->gain);
		ZInterval = QwtInterval(-max, max);
		break;
	}
	setAxisScale(QwtPlot::yRight, ZInterval.minValue(), ZInterval.maxValue());
	m_matrix->setInterval(Qt::ZAxis, ZInterval);
	auto Interval = m_colorMapWidget->setDataZInterval(ZInterval);
	m_matrix->setInterval(Qt::ZAxis, Interval);
	//m_colorMapWidget->setMinMax(Interval);

	//QList<double> rTicks[QwtScaleDiv::NTickTypes];
	//rTicks[QwtScaleDiv::MajorTick] << ZInterval.minValue() << (ZInterval.maxValue() - ZInterval.minValue()) / 2 << ZInterval.maxValue();
	//QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	//m_colorMapWidget->setColorMap(ZInterval, AwQwtColorMap::newMap(m_displaySettings->colorMap));
	//m_colorMapWidget->scaleDraw()->setScaleDiv(divR);
//	updateFreqScale(m_settings->freq_min, m_settings->freq_max, m_settings->step);
	//m_colorMapWidget->repaint();
	m_spectro->invalidateCache();
	replot();
}

//void TFPlot::setMinMax(double min, double max)
//{
//	m_min = min;
//	m_max = max;
//	updateZScale();
//}

void TFPlot::applyColorMap()
{
	m_spectro->setColorMap(AwQwtColorMap::newMap(m_displaySettings->colorMap));
	//m_colorMapWidget->setColorMap(ZInterval, AwQwtColorMap::newMap(m_displaySettings->colorMap));
	//m_colorMapWidget->repaint();
	m_spectro->invalidateCache();
	m_colorMapWidget->updateColorMap();
	replot();
}

void TFPlot::updateZInterval(const QwtInterval& interval)
{
	// check that interval is correct
	if (interval.maxValue() <= interval.minValue())
		return;
	m_matrix->setInterval(Qt::ZAxis, interval);
	m_spectro->invalidateCache();
	replot();
}