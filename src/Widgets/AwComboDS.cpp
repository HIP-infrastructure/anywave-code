#include <widget/AwComboDS.h>


void AwComboDS::setSamplingRate(float sr)
{
	clear();
	addItem(QString("x1 (%1Hz)").arg(sr));
	addItem(QString("x2 (%1Hz->%2Hz)").arg(sr).arg(sr / 2));
	addItem(QString("x4 (%1Hz->%2Hz)").arg(sr).arg(sr / 4));
	addItem(QString("x8 (%1Hz->%2Hz)").arg(sr).arg(sr / 8));
	m_samplingRate = sr;
}

float AwComboDS::getSamplingRate()
{
	switch (currentIndex()) {
	case 0:
		return 0.;
		break;
	case 1:
		return m_samplingRate / 2;
		break;
	case 2:
		return m_samplingRate / 4;
		break;
	case 3:
		return m_samplingRate / 8;
		break;
	default:
		return 0.;
		break;
	}
}