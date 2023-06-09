#pragma once

#include <QDialog>
#include "ui_SIWidget.h"
//#include <process/AwProcessGUIWidget.h>
//#include <aw_armadillo.h>

//class FFTIterations;
//class AwChannel;
//class PlotWidget;
//class Spectral;

class SIWidget : public QDialog
{
	Q_OBJECT

public:
	SIWidget(QWidget *parent = nullptr);
	~SIWidget();


public slots:
	void accept() override;
public:
	double timeWindow, overlap;
	QString windowing;
private:
	void clear();

	Ui::SIWidgetUi  m_ui;
};
