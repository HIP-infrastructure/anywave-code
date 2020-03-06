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
#ifndef TFPICKER_H
#define TFPICKER_H

#include <qwt_plot_picker.h>
#include <qwt_interval.h>
#include <qwt_plot_canvas.h>
#include <qwt_transform.h>
#include <qwt_scale_map.h>
#include <qwt_scale_widget.h>

//
// custom QwtPlotPicker to display Frequency
//
class TFPicker : public QwtPlotPicker
{
	Q_OBJECT
public:
	TFPicker(QwtPlotCanvas *canvas, QwtScaleWidget *widget);
	~TFPicker();
	QwtText trackerText(const QPoint &pos) const;
	void setFreqScaleInterval(float min, float max);
	void setTextColor(const QColor& color) { m_trackerColor = color; }
signals:
	void selectionDone(int start, int duration, float fmin, int lrow, int hrow); // selection is the area selected in data coordinates.
	void samplesSelected(int start, int duration); // emited after a selection
protected:
	QwtInterval m_freqInterval;
	QColor m_trackerColor;
	QwtScaleWidget *m_yScaleWidget;
protected slots:
	void prepareSelection(const QRectF& rect);

	
};

#endif
