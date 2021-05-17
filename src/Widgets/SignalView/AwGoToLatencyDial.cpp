#include "AwGoToLatencyDial.h"
#include <QMessageBox>

AwGoToLatencyDial::AwGoToLatencyDial(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	m_position = 0.;
}

void AwGoToLatencyDial::accept()
{
	m_position = spinBoxLatency->value();
	if (m_position < 0 || m_position > m_duration)
		QMessageBox::critical(this, tr("Invalid value"), tr("The position selected is incorrect!"));
	else
		QDialog::accept();	
}