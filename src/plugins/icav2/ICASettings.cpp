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
#include "ICASettings.h"
#include <QMessageBox>
#include <AwCore.h>
#include <math.h>
#include <AwKeys.h>
#include "ica.h"
#include "ICAAlgorithm.h"

ICASettings::ICASettings(ICA *process, QWidget *parent) : QDialog(parent)
{
	m_ui.setupUi(this);
	// the plugin is configured to get as recorded channels as input when launched.
	m_channels = process->pdi.input.channels();
	// but we need also the current montage
	m_montage = process->montage();
	m_modalitiesAsRecorded = AwChannel::getTypesAsString(m_channels);
	// remove unwanted modalities
	m_modalitiesAsRecorded.removeAll("TRIGGER");
	m_modalitiesAsRecorded.removeAll("ECG");
	m_modalitiesAsRecorded.removeAll("OTHER");
	m_modalitiesAsRecorded.removeAll("ICA");
	m_modalitiesAsRecorded.removeAll("REFERENCE");
	m_modalitiesAsRecorded.removeAll("GRAD");
	m_modalitiesMontage = AwChannel::getTypesAsString(m_montage);
	// remove unwanted modalities
	m_modalitiesMontage.removeAll("TRIGGER");
	m_modalitiesMontage.removeAll("ECG");
	m_modalitiesMontage.removeAll("OTHER");
	m_modalitiesMontage.removeAll("ICA");
	m_modalitiesMontage.removeAll("REFERENCE");
	m_modalitiesMontage.removeAll("GRAD");

	m_channelSource = ICASettings::AsRecorded;

	m_ui.comboModality->addItems(m_modalitiesAsRecorded);

	if (!process->pdi.input.markers().isEmpty())
		m_ui.inputDataWidget->setMarkers(process->pdi.input.markers());
	else
		m_ui.inputDataWidget->hide();
	connect(m_ui.cbAll, SIGNAL(toggled(bool)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.ignoreBads, SIGNAL(toggled(bool)),  this, SLOT(updateMaxNumOfIC()));
	connect(m_ui.radioAsRecorded, SIGNAL(toggled(bool)), this, SLOT(changeChannelSource(bool)));
	m_ui.labelTotalIC->hide();
	m_process = process;
	m_algos = process->algorithms();
	for (auto algo : m_algos) 
		m_ui.comboAlgo->addItem(algo->name());
	
	m_extraGUIWidget = nullptr;
	m_ui.groupBoxExtras->hide();
	connect(m_ui.comboAlgo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAlgo(int)));
//	m_ui.comboAlgo->setCurrentIndex(0);
	changeAlgo(0);

	// hide SEEG Restrict mode
	m_ui.checkSEEGElectrode->hide();
	m_ui.comboSEEGElectrode->hide();
	updateMaxNumOfIC();
}

ICASettings::~ICASettings()
{

}

void ICASettings::changeChannelSource(bool flag)
{
	disconnect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
	if (flag) { // AsRecorded 
		m_channelSource = ICASettings::AsRecorded;
		m_ui.comboModality->clear();
		m_ui.comboModality->addItems(m_modalitiesAsRecorded);
	}
	else {
		m_channelSource = ICASettings::Montage;
		m_ui.comboModality->clear();
		m_ui.comboModality->addItems(m_modalitiesMontage);
	}
	connect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
}

void ICASettings::changeAlgo(int index)
{
	Q_ASSERT(index < m_algos.size());
	auto algo = m_algos.at(index);
	auto gui = algo->getGUI();
	if (gui) {
		if (m_extraGUIWidget)
			m_ui.extrasLayout->removeWidget(m_extraGUIWidget);
		m_ui.extrasLayout->addWidget(gui);
		m_ui.groupBoxExtras->show();
		m_extraGUIWidget = gui;
	}
	else 
		m_ui.groupBoxExtras->hide();
}

void ICASettings::changeInputChannels()
{

}


void ICASettings::accept()
{
	args["algorithm"] = m_ui.comboAlgo->currentText();
	args[keys::skip_bad_channels] = m_ui.ignoreBads->isChecked();
	if (m_channelSource == ICASettings::AsRecorded)
		args["modality"] = m_modalitiesAsRecorded.at(m_ui.comboModality->currentIndex());
	else
		args["modality"] = m_modalitiesMontage.at(m_ui.comboModality->currentIndex());
	int modality = AwChannel::stringToType(args.value("modality").toString());

	args[keys::lp] = m_ui.spinLPF->value();
	args[keys::hp] = m_ui.spinHPF->value();
	args["comp"] = m_ui.spinNC->value();

	QStringList usedMarkers = m_ui.inputDataWidget->usedMarkers();
	QStringList skippedMarkers = m_ui.inputDataWidget->skippedMarkers();
	if (!usedMarkers.isEmpty())
		args[keys::use_markers] = usedMarkers;
	if (!skippedMarkers.isEmpty())
		args[keys::skip_markers] = skippedMarkers;

	// get channels matching modality
	AwChannelList channels;
	if (m_channelSource == ICASettings::AsRecorded) 
		channels = AwChannel::getChannelsOfType(m_channels, modality);
	else
		channels = AwChannel::getChannelsOfType(m_montage, modality);

	channels = AwChannel::removeDoublons(channels);
	if (m_ui.ignoreBads->isChecked()) { // ignoring bad channels
		auto badLabels = m_process->pdi.input.settings[keys::bad_labels].toStringList();
		foreach (AwChannel *c, channels) {
			if (badLabels.contains(c->name()))
				channels.removeAll(c);
		}
	}

	// check if num of components is <= number of channels
	if (m_ui.spinNC->value() > channels.size()) {
		QMessageBox::critical(this, "Components", "The number of components is greater than the number of available channels.");
		return;
	}

	if (m_ui.spinNC->value() < 2) {
		QMessageBox::critical(this, "Components", "Not enough components to compute.");
		return;
	}

	if (m_ui.spinLPF->value() == 0. && m_ui.spinHPF->value() == 0.)	{
		int res = QMessageBox::question(this, tr("Filters"), tr("Are you sure you want to compute ICA components on raw data (no filters)?"), 
		QMessageBox::Yes|QMessageBox::No);
		if (res == QMessageBox::No)
			return;
	}

	if (m_ui.checkBoxDS->isChecked())
		args["downsampling"] = true;

	AwUniteMaps(m_process->pdi.input.settings, args);

	// IMPORTANT:
	// if current montage was chosen the replace input channels accordingly
	if (m_channelSource == ICASettings::Montage)
		m_process->pdi.input.setNewChannels(m_montage, true);

	return QDialog::accept();
}

void ICASettings::updateMaxNumOfIC()
{
	int modality;
	AwChannelList list;
	if (m_channelSource == ICASettings::AsRecorded) {
		modality = AwChannel::stringToType(m_modalitiesAsRecorded.at(m_ui.comboModality->currentIndex()));
		list = AwChannel::extractChannelsOfType(m_channels, modality);
	}
	else {
		modality = AwChannel::stringToType(m_modalitiesMontage.at(m_ui.comboModality->currentIndex()));
		list = AwChannel::extractChannelsOfType(m_montage, modality);
	}
	
	int count = list.size();
	auto ignoreBadChannels = m_ui.ignoreBads->isChecked();
	if (ignoreBadChannels) {
		for (AwChannel *c : list) {
			if (c->isBad())
				count--;
		}
	}

	m_ui.labelTotalIC->setText(QString::number(count));
	m_ui.spinNC->setValue(count);

	if (modality == AwChannel::SEEG) {
		m_ui.checkSEEGElectrode->show();
		m_ui.comboSEEGElectrode->show();
	}
	else {
		m_ui.checkSEEGElectrode->hide();
		m_ui.comboSEEGElectrode->hide();
	}
}