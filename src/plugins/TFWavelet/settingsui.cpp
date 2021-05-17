#include "settingsui.h"
#include "ui_settingsui.h"


SettingsUi::SettingsUi(TFSettings *settings, QWidget *parent)
	: QDialog(parent)
{
	ui = new Ui::SettingsUi();
	ui->setupUi(this);
	m_settings = settings;
	ui->sbXi->setValue(settings->xi);
	ui->sbMax->setValue(settings->freq_max);
	ui->sbMin->setValue(settings->freq_min);
	ui->checkBoxDIFF->setChecked(settings->useDIFF);
}

SettingsUi::~SettingsUi()
{
	delete ui;
}



void SettingsUi::accept()
{
	m_settings->xi = ui->sbXi->value();
	m_settings->freqs.clear();

	int step = ui->sbStep->value();
	m_settings->freq_min = ui->sbMin->value();
	m_settings->freq_max = ui->sbMax->value();

	m_settings->useDIFF = ui->checkBoxDIFF->isChecked();
	
	if (m_settings->freq_max <= 0)
		return;
	if (m_settings->freq_min <= 0)
		return;

	for (double f = m_settings->freq_min; f <= m_settings->freq_max; f += step)
		m_settings->freqs << f;

	QDialog::accept();
}