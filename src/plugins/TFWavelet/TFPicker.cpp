#include "TFPicker.h"
#include <qwt_interval.h>
#include <qwt_plot.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
//#include <QtMath>
#include <cmath>


TFPicker::TFPicker(QwtPlotCanvas *canvas /*, QwtScaleWidget *scaleWidget*/, TFSettings *settings) : QwtPlotPicker(canvas)
{
	//m_yScaleWidget = scaleWidget;
	connect(this, SIGNAL(selected(const QRectF&)), this, SLOT(prepareSelection(const QRectF&)));
	m_plot = static_cast<QwtPlot*>(canvas->plot());
	m_settings = settings;
}

TFPicker::~TFPicker()
{
}

void TFPicker::setFreqScaleInterval(float min, float max)
{
	m_freqInterval = QwtInterval(min, max);
}

// Implementation for TFPicker
QwtText TFPicker::trackerText(const QPoint &pos) const
{
	QString output;
	//auto scaleDraw = m_plot->axisScaleDraw(QwtPlot::yLeft);
	//auto map = scaleDraw->scaleMap();
	//double value = map.invTransform(pos.y());
	double value = m_plot->invTransform(QwtPlot::yLeft, pos.y());
	//value += (m_settings->freq_min - 1);
	output.sprintf("%d Hz", static_cast<int>(std::floor(value)));
	QwtText text(output);
	QBrush bg(Qt::SolidPattern);
	bg.setColor(Qt::white);
	text.setBackgroundBrush(bg);
	text.setColor(Qt::black);
	return text;
}

void TFPicker::prepareSelection(const QRectF &rect)
{
	QRectF select = rect;
	// prepare sample selection
	int start = static_cast<int>(std::ceil(select.x()));
	int dur = static_cast<int>(std::ceil(select.width()));
	emit samplesSelected(start, dur);

	// ensure that Y is between 0 and max
	if (select.y() < 0)
		select.setY(0.);

	int lrow = ceil(select.y());
	int hrow = ceil(select.y() + select.height());
	int lcol = ceil(select.x());
	int hcol = ceil(select.x() + select.width());
	float fmin = select.y()+ m_freqInterval.minValue();
	emit selectionDone(start, dur, fmin, lrow, hrow);
}
