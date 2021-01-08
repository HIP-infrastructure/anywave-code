#pragma once

#include <QWidget>
#include "ui_PlotWidget.h"
#include "FFTIterations.h"
#include "qcustomplot.h"

class PlotWidget : public QWidget
{
	Q_OBJECT

public:
	PlotWidget(const FFTs& iterations, QWidget *parent = Q_NULLPTR);
	~PlotWidget();

private:
	Ui::PlotWidget ui;
	FFTs m_FFTs;
};
