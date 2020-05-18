/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "ChooseMarker.h"
#include <QMessageBox>
#include <AwChannel.h>

ChooseMarkerDialog::ChooseMarkerDialog(AwMarkerList *markers, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	foreach (AwMarker *m, *markers)
	{
		if (!m_markersNames.contains(m->label()))
			m_markersNames.append(m->label());
	}
	m_ui.listWidgetMarkers->addItems(m_markersNames);

}

ChooseMarkerDialog::~ChooseMarkerDialog()
{

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void ChooseMarkerDialog::accept()
{
	m_beforems = m_ui.spBefore->value();
	m_afterms = m_ui.spAfter->value();
	m_value = m_ui.spValue->value() * 1E-12;

	m_selectedMarkers.clear();
	// get markers selection
	if (!m_ui.listWidgetMarkers->selectionModel()->hasSelection())
	{
		QMessageBox::information(this, tr("Markers"), tr("No markers selected!"));
		return;
	}
	else
	{
		QModelIndexList selected = m_ui.listWidgetMarkers->selectionModel()->selectedRows();
		foreach (QModelIndex index, selected)
		{
			m_selectedMarkers.append(m_ui.listWidgetMarkers->item(index.row())->text());
		}
	}
	m_types.clear();
	if (m_ui.checkMEG->isChecked())
		m_types << AwChannel::MEG;
	if (m_ui.checkEEG->isChecked())
		m_types << AwChannel::EEG;
	if (m_types.isEmpty())
	{
		QMessageBox::information(this, tr("Channels"), tr("Select at least one channel type."));
		return;
	}


	return QDialog::accept();
}