/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <widget/AwInputMarkerTableView.h>
#include "InputMarkerModel.h"

AwInputMarkerTableView::AwInputMarkerTableView(QWidget *parent) : QTableView(parent)
{
	// Default flags => show all marker properties and allow multi selection
	InputMarkerModel *model = new InputMarkerModel(this);
	setModel(model);
	this->setSelectionBehavior(QAbstractItemView::SelectRows);
	setFlags(ShowAll | MultiSelection);
}


void AwInputMarkerTableView::setMarkers(const AwMarkerList& markers)
{
	InputMarkerModel *m = qobject_cast<InputMarkerModel *>(this->model());
	m->setMarkers(markers);
}

void AwInputMarkerTableView::setFlags(int flags)
{
	m_flags = flags;
	if (m_flags & AwInputMarkerTableView::MultiSelection)
		this->setSelectionMode(QAbstractItemView::MultiSelection);
	else
		this->setSelectionMode(QAbstractItemView::SingleSelection);

	this->setColumnHidden(MARKER_MODEL_COLUMN_LABEL, !(m_flags & AwInputMarkerTableView::ShowLabel));
	this->setColumnHidden(MARKER_MODEL_COLUMN_DUR, !(m_flags & AwInputMarkerTableView::ShowDuration));
	this->setColumnHidden(MARKER_MODEL_COLUMN_VALUE, !(m_flags & AwInputMarkerTableView::ShowValue));
	this->setColumnHidden(MARKER_MODEL_COLUMN_COLOR, !(m_flags & AwInputMarkerTableView::ShowColor));
	this->setColumnHidden(MARKER_MODEL_COLUMN_START, !(m_flags & AwInputMarkerTableView::ShowPosition));
	this->setColumnHidden(MARKER_MODEL_COLUMN_TARGETS, !(m_flags & AwInputMarkerTableView::ShowTargets));

}