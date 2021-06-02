#include "widget/AwAmplitudeWidget.h"
#include "widget/AwAmplitudeButton.h"
#include <AwAmplitudeManager.h>
#include <QGridLayout>
#include <QApplication>
#include "ui_AwAmplitudeWidget.h"
#include <aw_armadillo.h>

AwAmplitudeWidget::AwAmplitudeWidget(QWidget *parent)
	: QWidget(parent)
//AmplitudeWidget::AwAmplitudeWidget(AwGainLevels *gl, QWidget* parent)
// : QWidget(parent)
{
	m_ui = new Ui::AwAmplitudeWidgetUi();
	m_ui->setupUi(this);
	m_ui->spValue->hide();
	m_channelType = -1;
	connect(m_ui->spValue, SIGNAL(valueChanged(double)), this, SLOT(changeAmplitude(double)));
	connect(m_ui->buttonUp, SIGNAL(clicked()),this, SLOT(up()));
	connect(m_ui->buttonDown, SIGNAL(clicked()),this, SLOT(down()));
//	m_am = AwAmplitudeManager::instance();
	//m_gainLevels = gl;
	//// init combo levels => first Item MUST be All Gain Levels at index 0
	//// Channel types will be added after and shifted by one in the called slot.
	//m_ui->comboLevels->addItem(tr("All Gain Levels"));
	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	////	m_indexes[i] = m_am->getScale(i).indexOf(m_am->amplitude(i));
	//	auto gl = m_gainLevels->getGainLevelFor(i);
	//	uvec index  = arma::find(gl->values == gl->value);
	//	m_indexes[i] = index(0);
	//	m_ui->comboLevels->addItem(AwChannel::types.value(i), i);
	//}
	//connect(m_ui->comboLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLevelForChannel(int)));
}

AwAmplitudeWidget::~AwAmplitudeWidget()
{


}

void AwAmplitudeWidget::setGainLevels(AwGainLevels* gl)
{
	m_gainLevels = gl;
	m_ui->comboLevels->addItem(tr("All Gain Levels"));
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		//	m_indexes[i] = m_am->getScale(i).indexOf(m_am->amplitude(i));
		auto gl = m_gainLevels->getGainLevelFor(i);
		uvec index = arma::find(gl->values == gl->value);
		if (index.is_empty()) { // if the value is not in the scale, add it
			auto size = gl->values.size();
			gl->values.resize(size + 1);
			gl->values(size) = gl->value;
			sort(gl->values);
			index = arma::find(gl->values == gl->value);
		}
		m_indexes[i] = index(0);
		m_ui->comboLevels->addItem(AwChannel::types.value(i), i);
	}
	connect(m_ui->comboLevels, SIGNAL(currentIndexChanged(int)), this, SLOT(changeLevelForChannel(int)));
}

void AwAmplitudeWidget::up()
{
	if (m_channelType == -1) {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)  {
			if (m_indexes[i] > 0) {
				m_indexes[i]--;
		//		m_am->setAmplitude(i, m_am->getScale(i).at(m_indexes[i]));
				auto gl = m_gainLevels->getGainLevelFor(i);
				gl->value = gl->values(m_indexes[i]);
			}
		}
		emit amplitudesChanged();
	}
	else {
		if (m_indexes[m_channelType] > 0) {
			m_indexes[m_channelType]--;
			//m_am->setAmplitude(m_channelType, m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			auto gl = m_gainLevels->getGainLevelFor(m_channelType);
			gl->value = gl->values(m_indexes[m_channelType]);
			m_ui->spValue->setValue(gl->value);
		//	m_ui->spValue->setValue(m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
		//	emit amplitudeChanged(m_channelType,  m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			emit amplitudeChanged(m_channelType, gl->value);
		}
	}
}

void AwAmplitudeWidget::down()
{
	if (m_channelType == -1) {
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)  {
			auto gl = m_gainLevels->getGainLevelFor(i);

		//	if (m_indexes[i] <  m_am->getScale(i).size() - 1) {
			if (m_indexes[i] < gl->values.size() - 1) {
				m_indexes[i]++;
			//	m_am->setAmplitude(i, m_am->getScale(i).at(m_indexes[i]));
				gl->value = gl->values(m_indexes[i]);
			}
		}
		emit amplitudesChanged();
	}
	else {
		auto gl = m_gainLevels->getGainLevelFor(m_channelType);
		//if (m_indexes[m_channelType] <  m_am->getScale(m_channelType).size() - 1) {
		if (m_indexes[m_channelType] < gl->values.size() - 1) {
			m_indexes[m_channelType]++;
		//	m_am->setAmplitude(m_channelType, m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			gl->value = gl->values(m_indexes[m_channelType]);
		//	m_ui->spValue->setValue(m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			m_ui->spValue->setValue(gl->value);
		//	emit amplitudeChanged(m_channelType,  m_am->getScale(m_channelType).at(m_indexes[m_channelType]));
			emit amplitudeChanged(m_channelType, gl->value);
		}
	}
}

void AwAmplitudeWidget::changeCurrentChannelTypeAndValue(int type, float amplitude)
{
//	m_am->setAmplitude(type, amplitude);
	auto gl = m_gainLevels->getGainLevelFor(type);
	gl->value = amplitude;

	m_ui->comboLevels->setCurrentIndex(type + 1);
	m_channelType = type;
	emit amplitudeChanged(m_channelType, amplitude);
//	m_am->insertValueInScale(m_channelType, amplitude);
	auto size = gl->values.size();
	// check if value already exists in scale
	uvec indexes = arma::find(gl->values == amplitude);
	if (indexes.is_empty()) {
		gl->values.resize(size + 1);
		gl->values(size) = amplitude;
		arma::sort(gl->values);

	}
	indexes = arma::find(gl->values == amplitude);
	m_indexes[m_channelType] = indexes(0);

	//m_indexes[m_channelType] = m_am->getScale(m_channelType).indexOf(amplitude);
	//m_indexes[m_channelType] = indexes(0);
} 

void AwAmplitudeWidget::changeLevelForChannel(int type)
{
	m_channelType = type - 1;
	m_ui->spValue->setVisible(type != 0);
	if (type == 0)
		return;

	auto gl = m_gainLevels->getGainLevelFor(type);
	m_ui->spValue->setValue(gl->value);
	m_ui->spValue->setSuffix(gl->unit);


	//m_ui->spValue->setValue(m_am->amplitude(m_channelType));
	//m_ui->spValue->setSuffix(m_am->unitOfChannel(m_channelType));
}


void AwAmplitudeWidget::changeAmplitude(double value)
{
	if (m_channelType == -1)
		return;
//	m_am->setAmplitude(m_channelType, (float)value);
	auto gl = m_gainLevels->getGainLevelFor(m_channelType);
	gl->value = value;

	emit amplitudeChanged(m_channelType, (float)value);
//	m_am->insertValueInScale(m_channelType, (float)value);
		// check if value already exists in scale
	uvec indexes = arma::find(gl->values == value);
	auto size = gl->values.size();
	if (indexes.is_empty()) {
		gl->values.resize(size + 1);
		gl->values(size) = value;
		arma::sort(gl->values);
	}
//_indexes[m_channelType] = m_am->getScale(m_channelType).indexOf(value);
	m_indexes[m_channelType] = indexes(0);
}