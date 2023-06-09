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
#include "automarking.h"
#include <AwKeys.h>
#include <widget/AwMessageBox.h>

AutoMarkingPlugin::AutoMarkingPlugin()
{
	type = AwProcessPlugin::GUI;
	name = "Epoch Maker";
	version = "1.0.0";
	description = tr("Create epoch as markers.");
	category = "Process:Markers:Create Epochs";
}


AutoMarking::AutoMarking()
{
	setInputFlags(Aw::ProcessIO::GetAllMarkers);
}

AutoMarking::~AutoMarking()
{
	if (m_widget)
		delete m_widget;
}

void AutoMarking::run()
{
	if (pdi.input.markers().isEmpty()) {
		QMessageBox::critical(0, tr("No Markers"), tr("At least one marker should be present."));
		emit closed();
		return;
	}

	m_widget = new AwMarkAroundWidget;
	m_widget->setTotalDuration(pdi.input.settings.value(keys::file_duration).toFloat());
	m_widget->setMarkers(pdi.input.markers());
	connect(m_widget, SIGNAL(closed()), this, SIGNAL(closed()));
	connect(m_widget, SIGNAL(newMarkersCreated(const AwMarkerList&)), this, SLOT(newMarkers(const AwMarkerList&)));
	m_widget->show();
}

void AutoMarking::newMarkers(const AwMarkerList& markers)
{
	AwMarkerList temp = markers;
	emit sendMarkers(&temp);
	AwMessageBox::information(0, tr("Markers inserted"), QString(tr("%1 markers added.")).arg(temp.size())); 
	while (!temp.isEmpty())
		delete temp.takeFirst();

}