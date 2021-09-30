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
#include "AwTopoWidgetSettings.h"
#include "ui_AwTopoWidgetSettings.h"
#include <graphics/AwColorMap.h>

AwTopoWidgetSettings::AwTopoWidgetSettings(AwTopoWidget *widget, QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::AwTopoWidgetSettings();
	ui->setupUi(this);
	m_widget = widget;

	ui->checkBoxLabels->hide();
	ui->checkBoxContours->setChecked(m_widget->contoursEnabled());
	ui->checkBoxSensors->setChecked(m_widget->sensorsEnabled());
	ui->checkBoxLabels->setChecked(m_widget->labelsEnabled());
	ui->spinBoxContours->setValue(m_widget->contourLevels());
	ui->comboColormap->initToColorMap(m_widget->colorMap());
}

AwTopoWidgetSettings::~AwTopoWidgetSettings()
{
	delete ui;
}

void AwTopoWidgetSettings::accept()
{
	m_widget->setColorMap(ui->comboColormap->selectedMap());
	m_widget->setContourLevels(ui->spinBoxContours->value());
	m_widget->enableContours(ui->checkBoxContours->isChecked());
	m_widget->enableSensors(ui->checkBoxSensors->isChecked());
	m_widget->enableLabels(ui->checkBoxLabels->isChecked());
	m_widget->redraw();
	QDialog::accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwTopoWidgetSettings::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			ui->retranslateUi(this);
	}
}



