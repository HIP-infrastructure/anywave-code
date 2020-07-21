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
#include "TFPicker.h"
#include <qwt_interval.h>
#include <qwt_plot.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <QtMath>


TFPicker::TFPicker(QwtPlotCanvas *canvas, QwtScaleWidget *scaleWidget) : QwtPlotPicker(canvas)
{
	m_yScaleWidget = scaleWidget;
	connect(this, SIGNAL(selected(const QRectF&)), this, SLOT(prepareSelection(const QRectF&)));

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
	QwtScaleMap map = m_yScaleWidget->scaleDraw()->scaleMap();
	double value = map.invTransform(pos.y());
	output.sprintf("%.2f Hz", value);
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
	// Get plot scales intervals
	QwtPlotCanvas *c = (QwtPlotCanvas *)canvas();
	QwtInterval sampleInterv = c->plot()->axisInterval(QwtPlot::xBottom);
	QwtInterval freqInterv = c->plot()->axisInterval(QwtPlot::yLeft);

	// prepare sample selection
	int start = qCeil(select.x());
	int dur = qCeil(select.width());
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
