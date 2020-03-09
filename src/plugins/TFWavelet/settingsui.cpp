/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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