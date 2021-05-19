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
	m_ui->spValue->hide();
	m_channelType = -1;
	connect(m_ui->spValue, SIGNAL(valueChanged(double)), this, SLOT(changeAmplitude(double)));
	connect(m_ui->buttonUp, SIGNAL(clicked()),this, SLOT(up()));
	connect(m_ui->buttonDown, SIGNAL(clicked()),this, SLOT(down()));
	m_am = AwAmplitudeManager::instance();
	// init combo levels => first Item MUST be All Gain Levels at index 0
	// Channel types will be added after and shifted by one in the called slot.
	m_ui->comboLevels->addItem(tr("All Gain Levels"));
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		m_indexes[i] = m_am->getScale(i).indexOf(m_am->amplitude(i));
		m_ui->comboLevels->addItem(AwChannel::types.value(i), i);
	}
	connect(m_ui->comboLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLevelForChannel(int)));
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