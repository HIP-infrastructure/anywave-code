#include "TFColorMapWidget.h"

TFColorMapWidget::TFColorMapWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_lockedMinMax = false;
	m_min = m_max = 0.;

	m_ui.mainLayout->replaceWidget(m_ui.widget,)
}

TFColorMapWidget::~TFColorMapWidget()
{
}


void TFColorMapWidget::setMinMax(double min, double max)
{
	if (!m_lockedMinMax) {
		m_min = min;
		m_max = max;
		updateColorMap();
	}

}