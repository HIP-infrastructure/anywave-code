#include "AwTimeScaleCombo.h"

AwTimeScaleCombo::AwTimeScaleCombo(QWidget *parent)
	: QComboBox(parent)
{

	m_items << "3" << "2" << "1" << "0.5" << "0.4" << "0.3" << "0.2" << "0.1" << "0.05" << "0.04"
		<< "0.03" << "0.02" <<  "0.01";
	m_values << 3.0 << 2.0 << 1.0 << 0.5 << 0.4 << 0.3 << 0.2 << 0.1 << 0.05 << 0.04
		<< 0.03 << 0.02 << 0.01;
	this->addItems(m_items);
	this->setCurrentIndex(3);
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(extractValue(int)));
}

AwTimeScaleCombo::~AwTimeScaleCombo()
{

}


///////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwTimeScaleCombo::extractValue(int index)
{
	if (index != -1)
		emit valueChanged(m_values.at(index));
}