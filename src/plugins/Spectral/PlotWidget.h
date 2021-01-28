#pragma once

#include <QWidget>
#include "ui_PlotWidget.h"
#include "FFTIterations.h"
#include "qcustomplot.h"
#include <process/AwProcessOutputWidget.h>

class PlotWidget : public AwProcessOutputWidget
{
	Q_OBJECT

public:
	PlotWidget(const FFTs& iterations, AwBaseProcess *process, QWidget *parent = Q_NULLPTR);
	~PlotWidget();
protected slots:
	void graphClicked(QCPAbstractPlottable*, int);
private:
	Ui::PlotWidget ui;
	FFTs m_FFTs;
	QCPTextElement* m_textBox;
};
