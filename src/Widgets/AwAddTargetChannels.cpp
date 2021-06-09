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
#include "AwAddTargetChannels.h"

AwAddTargetChannels::AwAddTargetChannels(const QStringList& labels, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_ui.listWidgetAvailable->addItems(labels);
}

void AwAddTargetChannels::accept()
{
	QList<QListWidgetItem *> selection = m_ui.listWidgetAvailable->selectedItems();

	foreach (QListWidgetItem *item, selection)
		m_selectedChannels << item->text();
	QDialog::accept();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwAddTargetChannels::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			m_ui.retranslateUi(this);
	}
}
