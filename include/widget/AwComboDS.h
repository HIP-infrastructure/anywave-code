#pragma once
#include <QComboBox>
#include <AwGlobal.h>

// this combo box class is designed to select a marker from a list. Markers can be sorted to only display unique names or not.
class AW_WIDGETS_EXPORT AwComboDS : public QComboBox
{
	Q_OBJECT
public:
	AwComboDS(QWidget *parent) : QComboBox(parent) {}
	/** init the list by specifying a sampling rate in Hz**/
	void init(float sr);
	float getSamplingRate();
	int getDecimateFactor();
protected:
	float m_samplingRate;
};
