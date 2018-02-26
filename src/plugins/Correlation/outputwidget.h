#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>
#include "ui_outputwidget.h"
#include "correlation.h"
#include <qwt_scale_draw.h>
#include <qwt_plot_picker.h>

class LabelDraw : public QwtScaleDraw
{
public:
	LabelDraw(const QStringList& labels) : QwtScaleDraw() { m_labels = labels; }
	virtual QwtText label(double value) const;
private:
	QStringList m_labels;
};

class CorrPicker : public QwtPlotPicker
{
public:
	CorrPicker(corr_result *res, QWidget *canvas) : QwtPlotPicker(canvas) { m_result = res; }
	QwtText trackerText(const QPoint& pos) const;
protected:
	corr_result *m_result;
};

class OutputWidget : public QWidget
{
	Q_OBJECT

public:
	OutputWidget(corr_result *res, QWidget *parent = 0);
	~OutputWidget();
public slots:
	void saveToMat();
private:
	Ui::OutputWidgetUi m_ui;
	CorrPicker *m_picker;
	corr_result *m_result;
};



#endif // OUTPUTWIDGET_H
