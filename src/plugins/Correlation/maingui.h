#pragma once

#include <QWidget>
#include "ui_maingui.h"
#include "correlation.h"
#include "outputwidget.h"

class maingui : public QWidget
{
	Q_OBJECT

public:
	maingui(const QList<corr_result *>& results, QWidget *parent = Q_NULLPTR);
	~maingui();
public slots:
	void saveToMatlab();
private:
	void computeMean();

	Ui::mainguiUi m_ui;
	QList<corr_result *> m_results;
	corr_result m_mean;
	OutputWidget *m_outputMeanWidget;
};
