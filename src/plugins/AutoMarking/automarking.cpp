/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "automarking.h"
#include <widget/AwMessageBox.h>

AutoMarkingPlugin::AutoMarkingPlugin()
{
	type = AwProcessPlugin::GUI;
	name = "Epoch Maker";
	description = tr("Automatically add markers around an existing marker.");
	category = "Process:Markers:Epoch Maker";
}


AutoMarking::AutoMarking()
{
	pdi.setInputFlags(Aw::ProcessInput::GetAllMarkers);
}

AutoMarking::~AutoMarking()
{
	if (m_widget)
		delete m_widget;
}

void AutoMarking::run(const QStringList& args)
{
	if (m_markers.isEmpty()) {
		QMessageBox::critical(0, tr("No Markers"), tr("At least one marker should be present."));
		emit closed();
		return;
	}

	//m_widget = new AwAutoMarkWidget(m_markers, pdi.input.fileDuration);
	m_widget = new AwMarkAroundWidget;
	m_widget->setTotalDuration(pdi.input.settings[processio::file_duration].toDouble());
	m_widget->setMarkers(m_markers);
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