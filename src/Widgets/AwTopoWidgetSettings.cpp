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



