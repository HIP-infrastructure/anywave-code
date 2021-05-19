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
#include "AwEpochCreateUi.h"
#include "AwEpochModel.h"
#include "AwEpochManager.h"
#include <widget/AwMessageBox.h>
#include <Prefs/AwSettings.h>
#include "Montage/AwMontageManager.h"

AwEpochCreateUi::AwEpochCreateUi(const AwMarkerList& markers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_markers = markers;
	m_ui.comboBox->setMarkers(markers);
	m_ui.comboArtefacts->setMarkers(markers);
    QList<AwEpochTree *> conds = AwEpochManager::instance()->conditions();
    m_ui.tableView->setModel(new AwEpochModel(conds, m_ui.tableView));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	// build up the modality combo using the current montage
	AwChannelList channels = AwMontageManager::instance()->channels();
	QStringList types = AwChannel::getTypesAsString(channels);
	// only accept EEG/SEEG/MEG/GRAD/ICA/Source
	for (QString s : types) {
		if (s == "EEG")
			m_ui.comboBoxChannels->addItem(s, AwChannel::EEG);
		else if (s == "SEEG")
			m_ui.comboBoxChannels->addItem(s, AwChannel::SEEG);
		else if (s == "MEG")
			m_ui.comboBoxChannels->addItem(s, AwChannel::MEG);
		else if (s == "GRAD")
			m_ui.comboBoxChannels->addItem(s, AwChannel::GRAD);
		else if (s == "ICA")
			m_ui.comboBoxChannels->addItem(s, AwChannel::ICA);
		else if (s == "Source")
			m_ui.comboBoxChannels->addItem(s, AwChannel::Source);
	}
	updateTableView();

}

AwEpochCreateUi::~AwEpochCreateUi()
{

}


void AwEpochCreateUi::updateTableView()
{
	AwEpochModel *model = (AwEpochModel *)m_ui.tableView->model();
	model->setList(AwEpochManager::instance()->conditions());
}



void AwEpochCreateUi::addCondition()
{
	// get the name
	QString name = m_ui.editName->text().trimmed();
	// check if the condition already exists
	if (AwEpochManager::instance()->getCondition(name)) {
		AwMessageBox::critical(this, "Name", "A condition with this name already exists.");
		return;
	}
	float pre = m_ui.spinPre->value();
	float post = m_ui.spinPost->value();
	if (pre + post <= 0.) {
		AwMessageBox::critical(this, "Timing", "Please set the timing parameters.");
		return;
	}

	int modality = m_ui.comboBoxChannels->itemData(m_ui.comboBoxChannels->currentIndex()).toInt();
	QString artefact = m_ui.comboArtefacts->currentText();

	AwChannelList channels = AwChannel::duplicateChannels(AwChannel::getChannelsOfType(AwMontageManager::instance()->channels(), modality));
	if (AwEpochManager::instance()->createCondition(name, channels, m_ui.comboBox->currentText(), pre, post) != NULL) {
		updateTableView();
		AwEpochManager::instance()->save();
	}
	else {
		AwMessageBox::critical(this, "Epoching", "Failed to create epochs for the condition.");
		return;
	}
}
