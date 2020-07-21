#include "AwVideoSettingsDial.h"

AwVideoSettingsDial::AwVideoSettingsDial(QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	shift = drift = 0;
}

AwVideoSettingsDial::~AwVideoSettingsDial()
{
}


void AwVideoSettingsDial::accept()
{
	shift = m_ui.spinShift->value();
	drift = m_ui.spinDrift->value();
	QDialog::accept();
}