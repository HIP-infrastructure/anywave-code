#include <widget/AwDownSamplingWidget.h>
#include "ui_AwDownSamplingWidget.h"

AwDownSamplingWidget::AwDownSamplingWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwDownSamplingWidget();
	m_ui->setupUi(this);
	connect(m_ui->comboNewSR, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSR(int)));
	m_ui->spAA->hide();
	m_ui->labelAA->hide();
}

AwDownSamplingWidget::~AwDownSamplingWidget()
{
	delete m_ui;
}


void AwDownSamplingWidget::setMaxSR(float maxSR)
{
	m_maxSR = maxSR;
	m_ui->spAA->show();
	m_ui->labelAA->show();
	m_ui->spMaxSR->setValue(m_maxSR);

	m_possibleSRs.clear();
	QStringList rates;
	// compute new possible SR on int decimate factors;
	for (int i = 2; i < 11; i++) {
		m_possibleSRs << m_maxSR / i;
		rates << QString("%1Hz").arg(m_maxSR / i);
	}
	m_ui->comboNewSR->addItems(rates);
	changeSR(0);
}


void AwDownSamplingWidget::changeSR(int index)
{
	m_newSR = m_possibleSRs.at(index);
	m_ui->spAA->setValue(m_newSR / 3.);
}