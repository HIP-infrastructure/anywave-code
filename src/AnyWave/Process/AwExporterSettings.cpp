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
	setupUi(this);
	downSample = 0.;
	connect(buttonFile, SIGNAL(clicked()), this, SLOT(pickupFile()));
	connect(buttonSelectChannels, SIGNAL(clicked()), this, SLOT(selectChannels()));
	connect(buttonSelectICA, SIGNAL(clicked()), this, SLOT(selectICAChannels()));
	connect(buttonMarkers, SIGNAL(clicked()), this, SLOT(selectMarkers()));
	connect(comboWriters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOutputFileExtension(int)));
}

AwExporterSettings::~AwExporterSettings()
{

}

void AwExporterSettings::updateOutputFileExtension(int index)
{
	lineEditFile->setText(QString("%1%2").arg(initialPath).arg(extensions.at(index)));
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
			checkBoxICA->setChecked(false);
	}
}

void AwExporterSettings::selectChannels()
{
	AwExportSelChannels ui;
	ui.channels = channels;
	if (ui.exec() == QDialog::Accepted) {
		selectedChannels = ui.selectedChannels;
		if (!selectedChannels.isEmpty())
			checkBoxCurrentMontage->setChecked(false);
	}
}

void AwExporterSettings::pickupFile()
{
	QString ext = extensions.at(comboWriters->currentIndex());
	QFileInfo fi(initialPath);
	filePath = QFileDialog::getSaveFileName(this, tr("Output file"), fi.absolutePath(), ext);
	lineEditFile->setText(filePath);
}

int AwExporterSettings::exec()
{
	updateOutputFileExtension(0);
	comboWriters->addItems(writers);
	spinMEGLP->setValue(foptions.megLP);
	spinMEGHP->setValue(foptions.megHP);
	spinEEGLP->setValue(foptions.eegLP);
	spinEEGHP->setValue(foptions.eegHP);
	spinEMGLP->setValue(foptions.emgLP);
	spinEMGHP->setValue(foptions.emgHP);
	spinEEGNotch->setValue(foptions.eegNotch);
	spinMEGNotch->setValue(foptions.megNotch);
	spinEMGNotch->setValue(foptions.emgNotch);

	comboDS->setSamplingRate(channels.first()->samplingRate());


	return QDialog::exec();
}

void AwExporterSettings::accept()
{
	filePath = lineEditFile->text();
	if (filePath.isEmpty()) {
		AwMessageBox::information(this, tr("Output File"), tr("Select a file"));
		return;
	}
	// remove extensions to filePath
	QFileInfo fi(filePath);
	filePath = fi.absolutePath() + "/" + fi.baseName();

	writer = comboWriters->currentText();
	useCurrentMontage = checkBoxCurrentMontage->isChecked();
	exportICA = checkBoxICA->isChecked();
	foptions.eegHP = (float)spinEEGHP->value();
	foptions.eegLP = (float)spinEEGLP->value();
	foptions.megHP = (float)spinMEGHP->value();
	foptions.megLP = (float)spinMEGLP->value();
	foptions.emgHP = (float)spinEMGHP->value();
	foptions.emgLP = (float)spinEMGLP->value();
	foptions.eegNotch = (float)spinEEGNotch->value();
	foptions.megNotch = (float)spinMEGNotch->value();
	foptions.emgNotch = (float)spinEMGNotch->value();
	downSample = comboDS->getSamplingRate();


	QDialog::accept();
}