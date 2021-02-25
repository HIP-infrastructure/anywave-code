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
#include <widget/AwSelectInputDataWidget.h>
#include <AwProcessInterface.h>


AwExporterSettings::AwExporterSettings(AwProcess *p, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	decimateFactor = 1;
	connect(buttonFile, SIGNAL(clicked()), this, SLOT(pickupFile()));
	connect(buttonSelectChannels, SIGNAL(clicked()), this, SLOT(selectChannels()));
	connect(buttonSelectICA, SIGNAL(clicked()), this, SLOT(selectICAChannels()));
	connect(comboWriters, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOutputFileExtension(int)));
	//connect(buttonSkip, &QPushButton::clicked, this, &AwExporterSettings::addSkipLabel);
	//connect(buttonUse, &QPushButton::clicked, this, &AwExporterSettings::addUseLabel);
	//connect(buttonClearUse, &QPushButton::clicked, this, &AwExporterSettings::clearUse);
	//connect(buttonClearSkip, &QPushButton::clicked, this, &AwExporterSettings::clearSkip);
	filterTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	filterTableView->resizeColumnsToContents();
	m_process = p;
	bool markersManuallySelected = p->modifiersFlags() & Aw::ProcessIO::modifiers::UserSelectedMarkers;
	if (markersManuallySelected)
		inputDataWidget->hide();
	else 
		inputDataWidget->setMarkers(p->pdi.input.markers());

}

AwExporterSettings::~AwExporterSettings()
{

}

void AwExporterSettings::updateOutputFileExtension(int index)
{
	lineEditFile->setText(QString("%1%2").arg(initialPath).arg(extensions.at(index)));
}

//void AwExporterSettings::addSkipLabel()
//{
//	//auto item = comboSkip->currentText();
//	//if (!skippedMarkers.contains(item)) {
//	//	skippedMarkers << item;
//	//	listWidgetSkip->addItem(item);
//	//	listWidgetSkip->update();
//	//}
//}

//void AwExporterSettings::addUseLabel()
//{
//	//auto item = comboUse->currentText();
//	//if (!usedMarkers.contains(item)) {
//	//	usedMarkers << item;
//	//	listWidgetUse->addItem(item);
//	//	listWidgetUse->update();
//	//}
//}

//void AwExporterSettings::clearUse()
//{
//	//listWidgetUse->clear();
//	//usedMarkers.clear();
//}

//void AwExporterSettings::clearSkip()
//{
//	//listWidgetSkip->clear();
//	//skippedMarkers.clear();
//}

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
	filePath = QFileDialog::getSaveFileName(this, tr("Save File"), fi.absolutePath(), QString("*%1").arg(ext));
	if (!filePath.isEmpty())
		lineEditFile->setText(filePath);
}

int AwExporterSettings::exec()
{
	updateOutputFileExtension(0);
	comboWriters->addItems(writers);
	comboDS->init(channels.first()->samplingRate());
	filterTableView->setSettings(filterSettings);
	// init combo markers only with markers which have a duration.
	AwMarkerList tmp;
	for (auto m : markers) {
		if (m->duration() > 0.)
			tmp << m;
	}
	//if (!tmp.isEmpty()) {
	//	comboSkip->setMarkers(tmp);
	//	comboUse->setMarkers(tmp);
	//}
	//else {
	//	groupBoxMarkers->setDisabled(true);
	//}
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
	decimateFactor = comboDS->getDecimateFactor();
	filterSettings = filterTableView->settings();
	usedMarkers = inputDataWidget->usedMarkers();
	skippedMarkers = inputDataWidget->skippedMarkers();
//	renameLabels = checkRelabel->isChecked();
	QDialog::accept();
}