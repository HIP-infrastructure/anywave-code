#include "TFColorMapWidget.h"
#include "display_settings.h"
#include <qwt_scale_widget.h>
#include <cmath>
#include <graphics/AwQwtColorMap.h>
#include <qwt_color_map.h>

TFColorMapWidget::TFColorMapWidget(DisplaySettings *settings, QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_lockedMinMax = false;
	m_min = m_max = 0.;
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
}

TFColorMapWidget::~TFColorMapWidget()
{
}


void TFColorMapWidget::setMinMax(const QwtInterval& ZInterval)
{
	if (!m_lockedMinMax) {
		m_min = ZInterval.minValue();
		m_max = ZInterval.maxValue();
		m_ui.spinMaxZ->setValue(m_max);
		m_ui.spinMinZ->setValue(m_min);

		updateColorMap();
		m_ui.slider->setMinimum((int)std::floor(m_min));
		m_ui.slider->setMaximum((int)std::floor(m_max));
		m_ui.slider->setSliderPosition((int)std::floor(m_max));
	}
}

void TFColorMapWidget::updateColorMap()
{
	QList<double> rTicks[QwtScaleDiv::NTickTypes];
	auto s = std::abs((m_max - m_min)) / 4;

	rTicks[QwtScaleDiv::MajorTick] << m_min << std::floor(m_min + 1. * s) << std::floor(m_min + 2. * s) << std::floor(m_min + 3. * s) << m_max;
	QwtScaleDiv divR(rTicks[QwtScaleDiv::MajorTick].first(), rTicks[QwtScaleDiv::MajorTick].last(), rTicks);
	m_scaleWidget->scaleDraw()->setScaleDiv(divR);
	m_scaleWidget->setColorMap(QwtInterval(m_min, m_max), AwQwtColorMap::newMap(m_settings->colorMap));
	m_scaleWidget->repaint();
}