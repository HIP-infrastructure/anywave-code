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
	m_rawChannels = process->rawChannels();

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
	changeAlgo(0);

	// hide SEEG Restrict mode
	m_ui.checkSEEGElectrode->hide();
	m_ui.comboSEEGElectrode->setEnabled(false);
	m_ui.comboSEEGElectrode->hide();
	updateMaxNumOfIC();
	connect(m_ui.comboSEEGElectrode, SIGNAL(currentIndexChanged(int)), this, SLOT(changeSEEGElectrode(int)));
	connect(m_ui.checkSEEGElectrode, &QCheckBox::toggled, this,  &ICASettings::restrictSEEGElectrode);
}

ICASettings::~ICASettings()
{

}

int ICASettings::exec()
{
	m_modalitiesMontage = AwChannel::getTypesAsString(m_channels);
	// remove unwanted modalities
	m_modalitiesMontage.removeAll("TRIGGER");
	m_modalitiesMontage.removeAll("ECG");
	m_modalitiesMontage.removeAll("OTHER");
	m_modalitiesMontage.removeAll("ICA");
	m_modalitiesMontage.removeAll("REFERENCE");
	m_modalitiesMontage.removeAll("GRAD");
	m_modalitiesAsRecorded = AwChannel::getTypesAsString(m_rawChannels);
	// remove unwanted modalities
	m_modalitiesAsRecorded.removeAll("TRIGGER");
	m_modalitiesAsRecorded.removeAll("ECG");
	m_modalitiesAsRecorded.removeAll("OTHER");
	m_modalitiesAsRecorded.removeAll("ICA");
	m_modalitiesAsRecorded.removeAll("REFERENCE");
	m_modalitiesAsRecorded.removeAll("GRAD");

	// default input channels are as recorded ones
	m_channelSource = ICASettings::AsRecorded;
	m_ui.comboModalityMontage->addItems(m_modalitiesMontage);
	m_ui.comboModality->addItems(m_modalitiesAsRecorded);

	// check if one of the modalities is empty or both?
	if (m_modalitiesAsRecorded.isEmpty() && m_modalitiesMontage.isEmpty()) {
		QMessageBox::information(this, "Modalities", "Cannot compute ICA on this data file.");
		return QDialog::Rejected;
	}
	else if (m_modalitiesAsRecorded.isEmpty()) {
		m_ui.radioAsRecorded->setChecked(false);
		m_ui.radioCurrentMontage->setChecked(true);
		m_ui.radioAsRecorded->setEnabled(false);
		m_channelSource = ICASettings::Montage;
	}
	else if (m_modalitiesMontage.isEmpty()) {
		m_ui.radioCurrentMontage->setChecked(false);
		m_ui.radioAsRecorded->setChecked(true);
		m_ui.radioCurrentMontage->setEnabled(false);
		m_channelSource = ICASettings::AsRecorded;
	}

	return QDialog::exec();
}

void ICASettings::changeChannelSource(bool flag)
{
	if (flag) // AsRecorded 
		m_channelSource = ICASettings::AsRecorded;
	else
		m_channelSource = ICASettings::Montage;
	updateMaxNumOfIC();
	//disconnect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
	//if (flag) { // AsRecorded 
	//	m_channelSource = ICASettings::AsRecorded;
	//	m_ui.comboModality->clear();
	//	m_ui.comboModality->addItems(m_modalitiesAsRecorded);
	//}
	//else {
	//	m_channelSource = ICASettings::Montage;
	//	m_ui.comboModality->clear();
	//	m_ui.comboModality->addItems(m_modalitiesMontage);
	//}
	//connect(m_ui.comboModality, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxNumOfIC()));
}

void ICASettings::restrictSEEGElectrode(bool flag)
{
	if (flag) 
		changeSEEGElectrode(m_ui.comboSEEGElectrode->currentIndex());
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

/// <summary>
/// changeSEEGElectrode()
/// Update the max number of ICA components
/// </summary>
/// <param name="index"></param>
void ICASettings::changeSEEGElectrode(int index)
{
	Q_ASSERT(index < m_seegElectrodes.size());
	auto label = m_seegElectrodes.at(index);
	AwChannelList list;
	if (m_channelSource == ICASettings::AsRecorded)
		list = m_rawChannels;
	else
		list = m_channels;
	int count = 0;
	for (auto c : list) {
		if (c->name().startsWith(label))
			count++;
	}
	m_ui.labelTotalIC->setText(QString::number(count));
	m_ui.spinNC->setValue(count);
}

void ICASettings::changeInputChannels()
{

}


void ICASettings::accept()
{
	args[keys::algorithm] = m_ui.comboAlgo->currentText();
	args[keys::skip_bad_channels] = m_ui.ignoreBads->isChecked();
	if (m_channelSource == ICASettings::AsRecorded)
		args[keys::modality] = m_modalitiesAsRecorded.at(m_ui.comboModality->currentIndex());
	else
		args[keys::modality] = m_modalitiesMontage.at(m_ui.comboModalityMontage->currentIndex());
	int modality = AwChannel::stringToType(args.value(keys::modality).toString());

	args[keys::lp] = m_ui.spinLPF->value();
	args[keys::hp] = m_ui.spinHPF->value();
	args[keys::notch] = m_ui.spinNotch->value();
	args[keys::comp] = m_ui.spinNC->value();

	QStringList usedMarkers = m_ui.inputDataWidget->usedMarkers();
	QStringList skippedMarkers = m_ui.inputDataWidget->skippedMarkers();
	if (!usedMarkers.isEmpty())
		args[keys::use_markers] = usedMarkers;
	if (!skippedMarkers.isEmpty())
		args[keys::skip_markers] = skippedMarkers;

	// get channels matching modality
	AwChannelList channels;
	if (m_channelSource == ICASettings::AsRecorded) 
		channels = AwChannel::getChannelsOfType(m_rawChannels, modality);
	else
		channels = AwChannel::getChannelsOfType(m_channels, modality);

	// add new option Restrict to SEEG
	if (m_ui.checkSEEGElectrode->isChecked() && modality == AwChannel::SEEG) {
		auto label = m_seegElectrodes.at(m_ui.comboSEEGElectrode->currentIndex());
		args[keys::use_seeg_electrode] = label;
		AwChannelList list;
		for (auto c : channels) {
			if (c->name().startsWith(label))
				list << c;
		}
		channels = list;
	}

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
		args[keys::downsampling] = true;

	AwUniteMaps(m_process->pdi.input.settings, args);

	// IMPORTANT:
	// if As Recorded channels are chosen, then replace default input channels by as recorded channels
	if (m_channelSource == ICASettings::AsRecorded)
		m_process->pdi.input.setNewChannels(m_rawChannels, true);

	return QDialog::accept();
}

void ICASettings::updateMaxNumOfIC()
{
	int modality;
	AwChannelList list;
	if (m_channelSource == ICASettings::AsRecorded) {
		if (m_modalitiesAsRecorded.isEmpty())
			return;
		modality = AwChannel::stringToType(m_modalitiesAsRecorded.at(m_ui.comboModality->currentIndex()));
		list = AwChannel::extractChannelsOfType(m_rawChannels, modality);
	}
	else {
		if (m_modalitiesMontage.isEmpty())
			return;
		modality = AwChannel::stringToType(m_modalitiesMontage.at(m_ui.comboModalityMontage->currentIndex()));
		list = AwChannel::extractChannelsOfType(m_channels, modality);
	}
	if (modality == AwChannel::SEEG) {
		m_seegElectrodes.clear();
		m_ui.comboSEEGElectrode->clear();
		QRegularExpression exp("(\\d+)$");
		for (auto c : list) {
			auto label = c->name().remove(exp);
			if (!m_seegElectrodes.contains(label))
				m_seegElectrodes.append(label);
		}
		m_ui.comboSEEGElectrode->addItems(m_seegElectrodes);
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