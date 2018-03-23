/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwExporterSettings.h"
#include "ui_AwExporterSettings.h"
#include <QFileDialog>
#include <widget/AwMessageBox.h>
#include "AwExportSelChannels.h"
#include "AwExporterSelMarkers.h"

AwExporterSettings::AwExporterSettings(QWidget *parent)
	: QDialog(parent)
{
	m_ui = new Ui::AwExporterSettingsUi();
	m_ui->setupUi(this);
	downSample = 0.;
	connect(m_ui->buttonFile, SIGNAL(clicked()), this, SLOT(pickupFile()));
	connect(m_ui->buttonSelectChannels, SIGNAL(clicked()), this, SLOT(selectChannels()));
	connect(m_ui->buttonSelectICA, SIGNAL(clicked()), this, SLOT(selectICAChannels()));
	connect(m_ui->buttonMarkers, SIGNAL(clicked()), this, SLOT(selectMarkers()));
	connect(m_ui->comboWriters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOutputFileExtension(int)));
}

AwExporterSettings::~AwExporterSettings()
{
	delete m_ui;
}

void AwExporterSettings::updateOutputFileExtension(int index)
{
	m_ui->lineEditFile->setText(QString("%1%2").arg(initialPath).arg(extensions.at(index)));
}

void AwExporterSettings::selectMarkers()
{
	AwExporterSelMarkers ui;
	ui.markers = markers;	// all markers
	if (ui.exec() ==  QDialog::Accepted) {
		selectedMarkers = ui.selectedMarkers;
	}
}

void AwExporterSettings::selectICAChannels()
{
	AwExportSelChannels ui;
	ui.channels = icaChannels;
	if (ui.exec() == QDialog::Accepted) {
		selectedICA = ui.selectedChannels;
		if (!selectedICA.isEmpty())
			m_ui->checkBoxICA->setChecked(false);
	}
}

void AwExporterSettings::selectChannels()
{
	AwExportSelChannels ui;
	ui.channels = channels;
	if (ui.exec() == QDialog::Accepted) {
		selectedChannels = ui.selectedChannels;
		if (!selectedChannels.isEmpty())
			m_ui->checkBoxCurrentMontage->setChecked(false);
	}
}

void AwExporterSettings::pickupFile()
{
	QString ext = extensions.at(m_ui->comboWriters->currentIndex());
	QFileInfo fi(initialPath);
	filePath = QFileDialog::getSaveFileName(this, tr("Output file"), fi.absolutePath(), ext);
	m_ui->lineEditFile->setText(filePath);
}

int AwExporterSettings::exec()
{
	updateOutputFileExtension(0);
	m_ui->comboWriters->addItems(writers);
	m_ui->spinMEGLP->setValue(foptions.megLP);
	m_ui->spinMEGHP->setValue(foptions.megHP);
	m_ui->spinEEGLP->setValue(foptions.eegLP);
	m_ui->spinEEGHP->setValue(foptions.eegHP);
	m_ui->spinEMGLP->setValue(foptions.emgLP);
	m_ui->spinEMGHP->setValue(foptions.emgHP);
	m_ui->spinEEGNotch->setValue(foptions.eegNotch);
	m_ui->spinMEGNotch->setValue(foptions.megNotch);
	m_ui->spinEMGNotch->setValue(foptions.emgNotch);
	return QDialog::exec();
}

void AwExporterSettings::accept()
{
	filePath = m_ui->lineEditFile->text();
	if (filePath.isEmpty()) {
		AwMessageBox::information(this, tr("Output File"), tr("Select a file"));
		return;
	}
	// remove extensions to filePath
	QFileInfo fi(filePath);
	filePath = fi.absolutePath() + "/" + fi.baseName();

	writer = m_ui->comboWriters->currentText();
	useCurrentMontage = m_ui->checkBoxCurrentMontage->isChecked();
	exportICA = m_ui->checkBoxICA->isChecked();
	foptions.eegHP = (float)m_ui->spinEEGHP->value();
	foptions.eegLP = (float)m_ui->spinEEGLP->value();
	foptions.megHP = (float)m_ui->spinMEGHP->value();
	foptions.megLP = (float)m_ui->spinMEGLP->value();
	foptions.emgHP = (float)m_ui->spinEMGHP->value();
	foptions.emgLP = (float)m_ui->spinEMGLP->value();
	foptions.eegNotch = (float)m_ui->spinEEGNotch->value();
	foptions.megNotch = (float)m_ui->spinMEGNotch->value();
	foptions.emgNotch = (float)m_ui->spinEMGNotch->value();

	downSample = (float)m_ui->sbDownSample->value();
	QDialog::accept();
}