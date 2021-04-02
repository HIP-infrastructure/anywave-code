#pragma once

#include <QWidget>
#include "ui_TFColorMapWidget.h"
class QwtScaleWidget;

class TFColorMapWidget : public QWidget
{
	Q_OBJECT

public:
	TFColorMapWidget(QWidget *parent = Q_NULLPTR);
	~TFColorMapWidget();

	void setMinMax(double min, double max);

private:
	Ui::TFColorMapWidget m_ui;

	void updateColorMap();

	bool m_lockedMinMax;
	double m_min, m_max;
	QwtScaleWidget* m_scaleWidget;
};
