// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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