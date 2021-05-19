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
#include "settings.h"

settings::settings(const AwChannelList& channels, const AwMarkerList& markers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_channels = channels;
	m_markers = markers;

	// extract unique marker labels
	foreach (AwMarker *m, m_markers) {
		if (m->duration() > 0) { // only markers with duration
			if (!m_markerLabels.contains(m->label()))
				m_markerLabels << m->label();
		}
	}
	m_ui.comboMarker->insertItems(1, m_markerLabels);
	bool eeg = !AwChannel::getChannelsOfType(channels, AwChannel::EEG).isEmpty();
	bool seeg = !AwChannel::getChannelsOfType(channels, AwChannel::SEEG).isEmpty();
	bool meg = !AwChannel::getChannelsOfType(channels, AwChannel::MEG).isEmpty();
	bool ica = !AwChannel::getChannelsOfType(channels, AwChannel::ICA).isEmpty();

	m_ui.comboMatrix->addItem(QString(tr("All vs All")), settings::AllvsAll);
	if (eeg)
		m_ui.comboMatrix->addItem(QString(tr("EEG vs All")), settings::EEGvsAll);
	if (seeg)
		m_ui.comboMatrix->addItem(QString(tr("SEEG vs All")), settings::SEEGvsAll);
	if (meg)
		m_ui.comboMatrix->addItem(QString(tr("MEG vs All")), settings::MEGvsAll);
	if (ica)
		m_ui.comboMatrix->addItem(QString(tr("ICA vs All")), settings::ICAvsAll);
	computeAllvsAll = true;
	m_ui.comboMatrix->setCurrentIndex(0);
}

settings::~settings()
{

}


void settings::accept()
{
	// check if a marker is selected
	int index = m_ui.comboMarker->currentIndex();
	if (index) 
		marker = m_markerLabels.at(index - 1);
	index = m_ui.comboMatrix->itemData(m_ui.comboMatrix->currentIndex()).toInt();

	if (index == settings::AllvsAll) { // All vs All
		channels1 = channels2 = m_channels;
		return QDialog::accept();
	}
	AwChannelList channels;
	switch (index) 
	{
	case settings::SEEGvsAll:
		channels = AwChannel::getChannelsOfType(m_channels, AwChannel::SEEG);
		if (!channels.isEmpty() && m_channels.size() - channels.size() > 0) {
			channels1 = channels;
			foreach(AwChannel *c, m_channels) {
				if (!c->isSEEG())
					channels2 << c;
			}
			computeAllvsAll = false;
			return QDialog::accept();
		}
		break;
	case settings::EEGvsAll:
		channels = AwChannel::getChannelsOfType(m_channels, AwChannel::EEG);
		if (!channels.isEmpty() && m_channels.size() - channels.size() > 0) {
			channels1 = channels;
			foreach(AwChannel *c, m_channels) {
				if (!c->isEEG())
					channels2 << c;
			}
			computeAllvsAll = false;
			return QDialog::accept();
		}
		break;
	case settings::MEGvsAll:
		channels = AwChannel::getChannelsOfType(m_channels, AwChannel::MEG);
		if (!channels.isEmpty() && m_channels.size() - channels.size() > 0) {
			channels1 = channels;
			foreach(AwChannel *c, m_channels) {
				if (!c->isMEG())
					channels2 << c;
			}
			computeAllvsAll = false;
			return QDialog::accept();
		}
		break;
	case settings::ICAvsAll:
		channels = AwChannel::getChannelsOfType(m_channels, AwChannel::ICA);
		if (!channels.isEmpty() && m_channels.size() - channels.size() > 0) {
			channels1 = channels;
			foreach(AwChannel *c, m_channels) {
				if (!c->isICA())
					channels2 << c;
			}
			computeAllvsAll = false;
			return QDialog::accept();
		}
		break;
	}	
}
