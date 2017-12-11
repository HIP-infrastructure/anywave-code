/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "widget/AwAmplitudeWidget.h"
#include "widget/AwAmplitudeButton.h"
#include <AwAmplitudeManager.h>
#include <QGridLayout>
#include <QApplication>
#include "ui_AwAmplitudeWidget.h"

AwAmplitudeWidget::AwAmplitudeWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwAmplitudeWidgetUi();
	m_ui->setupUi(this);
	connect(m_ui->comboLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLevelForChannel(int)));
	m_ui->spValue->hide();
	m_channelType = -1;
	connect(m_ui->spValue, SIGNAL(valueChanged(double)), this, SLOT(changeAmplitude(double)));
	connect(m_ui->buttonUp, SIGNAL(clicked()),this, SLOT(up()));
	connect(m_ui->buttonDown, SIGNAL(clicked()),this, SLOT(down()));
	m_am = AwAmplitudeManager::instance();
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
		m_indexes[i]  = m_am->getScale(i).indexOf(m_am->amplitude(i));
}

AwAmplitudeWidget::~AwAmplitudeWidget()
{

}

void AwAmplitudeWidget::up()
{
	if (m_channelType == -1) {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)  {
			if (m_indexes[i] > 0) {
				m_indexes[i]--;
				m_am->setAmplitude(i, m_am->getScale(i).at(m_indexes[i]));
			}
		}
		emit amplitudesChanged();
	}
	else {
		if (m_indexes[m_channelType] > 0) {
			m_indexes[m_channelType]--;
			m_am->setAmplitude(m_channelType, m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			m_ui->spValue->setValue(m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			emit amplitudeChanged(m_channelType,  m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
		}
	}
}

void AwAmplitudeWidget::down()
{
	if (m_channelType == -1) {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)  {
			if (m_indexes[i] <  m_am->getScale(i).size() - 1) {
				m_indexes[i]++;
				m_am->setAmplitude(i, m_am->getScale(i).at(m_indexes[i]));
			}
		}
		emit amplitudesChanged();
	}
	else {
		if (m_indexes[m_channelType] <  m_am->getScale(m_channelType).size() - 1) {
			m_indexes[m_channelType]++;
			m_am->setAmplitude(m_channelType, m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			m_ui->spValue->setValue(m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			emit amplitudeChanged(m_channelType,  m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
		}
	}
}

void AwAmplitudeWidget::changeCurrentChannelTypeAndValue(int type, float amplitude)
{
	m_am->setAmplitude(type, amplitude);
	m_ui->comboLevels->setCurrentIndex(type + 1);
	m_channelType = type;
	emit amplitudeChanged(m_channelType, amplitude);
	m_am->insertValueInScale(m_channelType, amplitude);
	m_indexes[m_channelType] = m_am->getScale(m_channelType).indexOf(amplitude);
} 

void AwAmplitudeWidget::changeLevelForChannel(int type)
{
	m_channelType = type - 1;
	m_ui->spValue->setVisible(type != 0);
	if (type == 0)
		return;

	m_ui->spValue->setValue(m_am->amplitude(m_channelType));
	m_ui->spValue->setSuffix(m_am->unitOfChannel(m_channelType));
}


void AwAmplitudeWidget::changeAmplitude(double value)
{
	if (m_channelType == -1)
		return;
	m_am->setAmplitude(m_channelType, (float)value);
	emit amplitudeChanged(m_channelType, (float)value);
	m_am->insertValueInScale(m_channelType, (float)value);
	m_indexes[m_channelType] = m_am->getScale(m_channelType).indexOf(value);
}