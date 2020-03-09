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
#include <widget/AwComboMarker.h>


AwComboMarker::AwComboMarker(QWidget *parent) : QComboBox(parent)
{
	m_filter = 0;
}

void AwComboMarker::setMarkers(const AwMarkerList& markers)
{
	m_saved = markers;
	m_markers.clear();
	filter();
}

void AwComboMarker::filter()
{
	clear();
	m_markers = m_saved;
	if (m_filter & AwComboMarker::ExcludeNoDuration) {
		foreach(AwMarker *m, m_markers) {
			if (m->duration() <= 0.)
				m_markers.removeAll(m);
		}
	}
	addItems(AwMarker::getUniqueLabels(m_markers));
	update();
}

void AwComboMarker::setFilter(int type) 
{
	m_filter = type;
	filter();
}
