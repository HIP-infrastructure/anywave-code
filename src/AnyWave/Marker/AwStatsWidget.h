#pragma once
#include <QWidget>
#include "ui_AwStatsWidget.h"
#include <AwMarker.h>


class AwStatsWidget : public QWidget
{
	Q_OBJECT

public:
	AwStatsWidget(QWidget *parent = Q_NULLPTR);
	~AwStatsWidget();
signals:
	void finished();  // emit when histogram is updated
public slots:
	void compute();
	void enableUpdate() { m_ui.buttonUpdate->setEnabled(true); }
	void handleMouseMove(QMouseEvent*);
private:
	Ui::AwStatsWidgetUi m_ui;
	AwMarkerList m_markers;
	QHash<QString, int> m_histogram;
	QCPBars* m_barPlot;

	void computeHistogram();
	void refresh();
};
