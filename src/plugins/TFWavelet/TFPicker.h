#ifndef TFPICKER_H
#define TFPICKER_H

#include <qwt_plot_picker.h>
#include <qwt_interval.h>
#include <qwt_plot_canvas.h>
#include <qwt_transform.h>
#include <qwt_scale_map.h>
#include <qwt_scale_widget.h>
#include "tf_settings.h"

//
// custom QwtPlotPicker to display Frequency
//
class TFPicker : public QwtPlotPicker
{
	Q_OBJECT
public:
	TFPicker(QwtPlotCanvas *canvas /*, QwtScaleWidget *widget*/ , TFSettings *settings);
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
//	QwtScaleWidget *m_yScaleWidget;
	QwtPlot* m_plot;
	TFSettings* m_settings;
protected slots:
	void prepareSelection(const QRectF& rect);

	
};

#endif
