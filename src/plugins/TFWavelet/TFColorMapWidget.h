#pragma once

#include <QWidget>
#include "ui_TFColorMapWidget.h"
class QwtScaleWidget;
#include <qwt_interval.h>
class DisplaySettings;

class TFColorMapWidget : public QWidget
{
	Q_OBJECT

public:
	TFColorMapWidget(DisplaySettings *settings, QWidget*parent = Q_NULLPTR);
	~TFColorMapWidget();

	void setMinMax(const QwtInterval& ZInterval);
	void updateColorMap();
private:
	Ui::TFColorMapWidget m_ui;
	bool m_lockedMinMax;
	double m_min, m_max;
	QwtScaleWidget* m_scaleWidget;
	DisplaySettings *m_settings;
};
