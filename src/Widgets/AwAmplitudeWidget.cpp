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
#include "widget/AwAmplitudeWidget.h"
//#include "widget/AwAmplitudeButton.h"
//#include <AwAmplitudeManager.h>
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
	if (m_widget) { // destroy and rebuild widget each time as gain levels types may change in the curren view.
		delete m_widget;
		m_widget = nullptr;
	}
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
		m_indexes[level->type] = level->getIndexOfValue(level->value());
}

void AwAmplitudeWidget::up()
{
	// lower gain value for all channels present
	auto levels = m_gainLevels->gainLevels();
	for (auto level : levels) {
		if (m_indexes[level->type] > 0) {
			m_indexes[level->type]--;
			level->setValue(level->values()(m_indexes[level->type]));
		}
	}
	emit amplitudesChanged();
}

void AwAmplitudeWidget::down()
{
	// raise gain value for all channels present
	auto levels = m_gainLevels->gainLevels();
	for (auto level : levels) {
		if (m_indexes[level->type] < level->values().size() - 1) {
			m_indexes[level->type]++;
			level->setValue(level->values()(m_indexes[level->type]));
		}
	}
	emit amplitudesChanged();
}
