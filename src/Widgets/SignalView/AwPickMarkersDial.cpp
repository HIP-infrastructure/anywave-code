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
#include "AwPickMarkersDial.h"
#include <AwMarkingSettings.h>

AwPickMarkersDial::AwPickMarkersDial(AwMarkingSettings*settings, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_settings = settings;
	int index = 0;
	for (auto m : settings->predefinedMarkers) {
		Qt::CheckState state = Qt::Unchecked;
		auto item = new QListWidgetItem(m->label());
		ui.listWidget->addItem(item);
		if (settings->selectedPredefinedMarkers.contains(index++))
			state = Qt::Checked;
		item->setCheckState(state);
		item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
	}
}

AwPickMarkersDial::~AwPickMarkersDial()
{
}


void AwPickMarkersDial::accept()
{
	m_settings->selectedPredefinedMarkers.clear();
	for (int i = 0; i < ui.listWidget->count(); i++) {
		auto item = ui.listWidget->item(i);
		if (item->checkState() == Qt::Checked)
			m_settings->selectedPredefinedMarkers.append(i);
	}

	QDialog::accept();
}