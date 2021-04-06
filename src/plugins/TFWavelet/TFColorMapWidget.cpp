#include "TFColorMapWidget.h"
#include "display_settings.h"
#include <qwt_scale_widget.h>
#include <cmath>
#include <graphics/AwQwtColorMap.h>
#include <qwt_color_map.h>


ColorMapSettings::ColorMapSettings()
{
	m_min = m_max = 0.;
	m_locked = false;
}

ColorMapSettings::ColorMapSettings(double mmin, double mmax, bool l)
{
	m_min = mmin;
	m_max = mmax;
	m_locked = l;
}

void ColorMapSettings::setDataRange(double min, double max)
{
	m_realMin = min;
	m_realMax = max;
	if (!m_locked) {
		m_min = min;
		m_max = max;
	}
}

void ColorMapSettings::setDataRange(const QwtInterval& interval)
{
	setDataRange(interval.minValue(), interval.maxValue());
}

void ColorMapSettings::reset()
{
	m_locked = false;
	m_min = m_realMin;
	m_max = m_realMax;
}

void ColorMapSettings::setMax(double v)
{
	if (m_locked)
		return;
	m_max = v;
}


TFColorMapWidget::TFColorMapWidget(DisplaySettings *settings, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_settings = settings;
	m_scaleWidget = new QwtScaleWidget(QwtScaleDraw::RightScale);
	m_scaleWidget->setContentsMargins(0, 0, 0, 0);
	m_scaleWidget->setBorderDist(1, 1);
	m_scaleWidget->setSpacing(5);
	m_scaleWidget->setMargin(5);
	m_scaleWidget->setColorBarEnabled(true);
	m_scaleWidget->setColorBarWidth(25);
	m_scaleWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	m_ui.mainLayout->replaceWidget(m_ui.widget, m_scaleWidget);
	m_ui.slider->setMinimum(0);
	m_ui.slider->setMaximum(100);
	m_ui.slider->setValue(0);
	connect(m_ui.slider, SIGNAL(valueChanged(int)), this, SLOT(changeMaxGain(int)));
	connect(m_ui.buttonLock, &QPushButton::clicked, this, &TFColorMapWidget::lock);
}

TFColorMapWidget::~TFColorMapWidget()
{
}


QwtInterval TFColorMapWidget::setMinMax(const QwtInterval& ZInterval)
{
	m_colorMapSettings.setDataRange(ZInterval);
	updateColorMap();
	return QwtInterval(m_colorMapSettings.min(), m_colorMapSettings.max());
}

void TFColorMapWidget::updateColorMap()
{
	//QList<double> rTicks[QwtScaleDiv::NTickTypes];
	auto min = m_colorMapSettings.min();
	auto max = m_colorMapSettings.max();
	//auto s = std::abs(max - min) / 4;

	//rTicks[QwtScaleDiv::MajorTick] << min << std::floor(min + 1. * s) << std::floor(min + 2. * s) << std::floor(min + 3. * s) << max;
	//QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	//m_scaleWidget->scaleDraw()->setScaleDiv(divR);
	m_scaleWidget->setColorMap(QwtInterval(min, max), AwQwtColorMap::newMap(m_settings->colorMap));
	m_scaleWidget->repaint();
}


void TFColorMapWidget::lock()
{
	if (m_colorMapSettings.isLocked()) {
		// unlock
		m_ui.buttonLock->setText("Lock");
		m_colorMapSettings.reset();
		m_colorMapSettings.setLock(false);
	}
	else {
		m_ui.buttonLock->setText("Unlock");
		m_colorMapSettings.setLock(true);
	}
	m_ui.slider->setEnabled(!m_colorMapSettings.isLocked());
}

void TFColorMapWidget::reset()
{
	m_colorMapSettings.reset();
	m_ui.slider->setEnabled(true);
	m_ui.slider->setValue(0);
}

void TFColorMapWidget::changeMaxGain(int percent)
{
	double maxGain = m_colorMapSettings.realMax() * ( 100 - static_cast<double>(percent)) / 100.;
	// ensure max is alway > min
	if (maxGain <= m_colorMapSettings.min())
		return;
 	m_colorMapSettings.setMax(maxGain);
//	updateColorMap();
	emit newZInterval(QwtInterval(m_colorMapSettings.min(), m_colorMapSettings.max()));
}