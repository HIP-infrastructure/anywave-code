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
#include "AwPickChannelsDial.h"
#include "Montage/AwMontageManager.h"
#include <QListWidgetItem>
///
/// Constructor
///
/// marker could be null if we want to set Channels to be marker by the user when inserting new markers.
/// marker must be point to a marker if we want the user to edit the targeted channels for a particular marker.
AwPickChannelsDial::AwPickChannelsDial(const QStringList& existing, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	AwChannelList montage = AwMontageManager::instance()->channels();
		
	// building a list a unique channel names from montage
	foreach (AwChannel *c, montage) {
		QString name = c->name();
		if (c->hasReferences())
			name += "-" + c->referenceName(); 
		if (!m_montageLabels.contains(name))
			m_montageLabels << name;
	}
	QStringList available = m_montageLabels;
	foreach(QString s, existing)
		if (available.contains(s))
			available.removeAll(s);
	
	if (!existing.isEmpty())
		m_ui.listWidgetTargets->insertItems(0, existing);
	m_ui.listWidgetAvailable->addItems(available);
}

AwPickChannelsDial::~AwPickChannelsDial()
{

}


/////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwPickChannelsDial::addChannels()
{
	if (m_ui.listWidgetAvailable->selectedItems().isEmpty())
		return;

	foreach (QListWidgetItem *item, m_ui.listWidgetAvailable->selectedItems()) {
		QString text = item->text();
		for (int i = 0; i < m_ui.listWidgetAvailable->count(); i++)
			if (m_ui.listWidgetAvailable->item(i)->text() == text)
				delete m_ui.listWidgetAvailable->takeItem(i);
		m_ui.listWidgetTargets->addItem(text);
	}
}

void AwPickChannelsDial::removeChannels()
{
	if (m_ui.listWidgetTargets->selectedItems().isEmpty())
		return;

	foreach (QListWidgetItem *item, m_ui.listWidgetTargets->selectedItems()) {
		QString text = item->text();
		for (int i = 0; i < m_ui.listWidgetTargets->count(); i++)
			if (m_ui.listWidgetTargets->item(i)->text() == text)
				delete m_ui.listWidgetTargets->takeItem(i);
		m_ui.listWidgetAvailable->addItem(text);
	}
}

void AwPickChannelsDial::clearTargets()
{
	m_ui.listWidgetTargets->clear();
	m_ui.listWidgetAvailable->clear();
	m_ui.listWidgetAvailable->addItems(m_montageLabels);
}

void AwPickChannelsDial::accept()
{
	for (int i = 0; i < m_ui.listWidgetTargets->count(); i++)
		targets.append(m_ui.listWidgetTargets->item(i)->text());
	QDialog::accept();
}