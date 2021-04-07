#pragma once

#include <QWidget>
#include "ui_TFColorMapWidget.h"
class QwtScaleWidget;
#include <qwt_interval.h>
class DisplaySettings;

class ColorMapSettings
{
public:
	ColorMapSettings(double min, double max, bool locked = false);
	ColorMapSettings();

	void setDataRange(double mmin, double mmax);
	void setDataRange(const QwtInterval& interval);

	inline double min() { return m_min; }
	inline double max() { return m_max; }
	inline double realMin() { return m_realMin; }
	inline double realMax() { return m_realMax; }
	inline bool isLocked() { return m_locked; }
	void setLock(bool f) { m_locked = f; }
	void setMax(double);
	void reset();
protected:
	double m_min, m_max;
	double m_realMin, m_realMax;
	bool m_locked;
};

class TFColorMapWidget : public QWidget
{
	Q_OBJECT

public:
	TFColorMapWidget(DisplaySettings *settings, QWidget*parent = Q_NULLPTR);
	~TFColorMapWidget();

	QwtInterval setDataZInterval(const QwtInterval& ZInterval);
	void forceMinMax(double min, double max);
	void updateColorMap();
signals:
	void newZInterval(const QwtInterval& interval);
	void applyMinMaxToAll(double, double);
public slots:
	void lock();
	void reset();
	void changeMaxGain(int);
	void applyToAllMaps();

private:
	Ui::TFColorMapWidget m_ui;
	ColorMapSettings m_colorMapSettings;
	QwtScaleWidget* m_scaleWidget;
	DisplaySettings *m_settings;
};
