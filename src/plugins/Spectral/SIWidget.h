#pragma once

#include <QWidget>
#include "ui_SIWidget.h"
#include <process/AwProcessGUIWidget.h>
#include <aw_armadillo.h>

class FFTIterations;
class AwChannel;
class PlotWidget;

class SIWidget : public QWidget
{
	Q_OBJECT

public:
	SIWidget(AwProcess *process, QWidget *parent = nullptr);
	~SIWidget();

	enum Windows { None,  Hanning, Hamming };
public slots:
	void compute();
	void showPlots();
private:
	void clear();

	Ui::SIWidgetUi  m_ui;
	int m_window; // type of windowing to use, 0 = none
	QMap<AwChannel*, FFTIterations*> m_results;
	QList<PlotWidget*> m_plotWidgets;
	AwProcess* m_process;
};
