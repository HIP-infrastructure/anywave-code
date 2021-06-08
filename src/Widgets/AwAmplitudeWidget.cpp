#include "widget/AwAmplitudeWidget.h"
#include "widget/AwAmplitudeButton.h"
#include <AwAmplitudeManager.h>
#include <QGridLayout>
#include <QApplication>
#include "ui_AwAmplitudeWidget.h"
#include <aw_armadillo.h>
#include <widget/AwGainLevelsWidget.h>


AwAmplitudeWidget::AwAmplitudeWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui = new Ui::AwAmplitudeWidgetUi();
	m_ui->setupUi(this);
	connect(m_ui->buttonUp, SIGNAL(clicked()),this, SLOT(up()));
	connect(m_ui->buttonDown, SIGNAL(clicked()),this, SLOT(down()));
	connect(m_ui->buttonEdit, &QPushButton::clicked, this, &AwAmplitudeWidget::editGains);
	m_widget = nullptr;
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
		m_indexes[i] = 0;
}

AwAmplitudeWidget::~AwAmplitudeWidget()
{


}

void AwAmplitudeWidget::editGains()
{
	if (m_gainLevels == nullptr)
		return;
	if (m_widget == nullptr) { 
		m_widget = new AwGainLevelsWidget(m_gainLevels, this); // we can make the widget a child of this because it has a Qt::Window flag set so it will show up standalone
		connect(m_widget, &AwGainLevelsWidget::amplitudeChanged, this, &AwAmplitudeWidget::amplitudeChanged);
	}
	m_widget->show();
}

void AwAmplitudeWidget::setGainLevels(AwGainLevels* gl)
{
	m_gainLevels = gl;
	auto levels = gl->gainLevels();
	for (auto level : levels) 
		m_indexes[level->type] = level->getIndexOfValue(level->value);
}

void AwAmplitudeWidget::up()
{
	// lower gain value for all channels present
	auto levels = m_gainLevels->gainLevels();
	for (auto level : levels) {
		if (m_indexes[level->type] > 0) {
			m_indexes[level->type]--;
			level->value = level->values(m_indexes[level->type]);
		}
	}
	emit amplitudesChanged();
}

void AwAmplitudeWidget::down()
{
	// raise gain value for all channels present
	auto levels = m_gainLevels->gainLevels();
	for (auto level : levels) {
		if (m_indexes[level->type] < level->values.size() - 1) {
			m_indexes[level->type]++;
			level->value = level->values(m_indexes[level->type]);
		}
	}
	emit amplitudesChanged();
}

//void AwAmplitudeWidget::changeCurrentChannelTypeAndValue(int type, float amplitude)
//{
////	m_am->setAmplitude(type, amplitude);
//	auto gl = m_gainLevels->getGainLevelFor(type);
//	gl->value = amplitude;
//
//	m_ui->comboLevels->setCurrentIndex(type + 1);
//	m_channelType = type;
//	emit amplitudeChanged(m_channelType, amplitude);
////	m_am->insertValueInScale(m_channelType, amplitude);
//	auto size = gl->values.size();
//	// check if value already exists in scale
//	uvec indexes = arma::find(gl->values == amplitude);
//	if (indexes.is_empty()) {
//		gl->values.resize(size + 1);
//		gl->values(size) = amplitude;
//		arma::sort(gl->values);
//
//	}
//	indexes = arma::find(gl->values == amplitude);
//	m_indexes[m_channelType] = indexes(0);
//
//	//m_indexes[m_channelType] = m_am->getScale(m_channelType).indexOf(amplitude);
//	//m_indexes[m_channelType] = indexes(0);
//} 

//void AwAmplitudeWidget::changeLevelForChannel(int type)
//{
//	m_channelType = type - 1;
//	m_ui->spValue->setVisible(type != 0);
//	if (type == 0)
//		return;
//
//	auto gl = m_gainLevels->getGainLevelFor(type);
//	m_ui->spValue->setValue(gl->value);
//	m_ui->spValue->setSuffix(gl->unit);
//
//
//	//m_ui->spValue->setValue(m_am->amplitude(m_channelType));
//	//m_ui->spValue->setSuffix(m_am->unitOfChannel(m_channelType));
//}


//void AwAmplitudeWidget::changeAmplitude(double value)
//{
//	if (m_channelType == -1)
//		return;
////	m_am->setAmplitude(m_channelType, (float)value);
//	auto gl = m_gainLevels->getGainLevelFor(m_channelType);
//	gl->value = value;
//
//	emit amplitudeChanged(m_channelType, (float)value);
////	m_am->insertValueInScale(m_channelType, (float)value);
//		// check if value already exists in scale
//	uvec indexes = arma::find(gl->values == value);
//	auto size = gl->values.size();
//	if (indexes.is_empty()) {
//		gl->values.resize(size + 1);
//		gl->values(size) = value;
//		arma::sort(gl->values);
//	}
////_indexes[m_channelType] = m_am->getScale(m_channelType).indexOf(value);
//	m_indexes[m_channelType] = indexes(0);
//}