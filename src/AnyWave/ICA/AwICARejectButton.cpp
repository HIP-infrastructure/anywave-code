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
#include "AwICARejectButton.h"

AwICARejectButton::AwICARejectButton(bool rejected, AwICASignalItem *item, QWidget *parent)
	: QPushButton(parent)
{
	m_item = item;
	m_rejected = rejected;
	setStyleSheet("QPushButton { color: blue; border-width: 1px; border-color: #339; padding:3px; \
					 border-style:solid; border-radius:7; min-width: 50px; font-family: Arial; font-size: 8pt;}");
	setText(tr("Reject Component"));
	connect(this, SIGNAL(clicked()), this, SLOT(reject()));
}

AwICARejectButton::~AwICARejectButton()
{

}

void AwICARejectButton::updateRejectState(bool rejected)
{
	m_rejected = rejected;
	if (!m_rejected) {
		setText(tr("Reject Component"));
		repaint();
	}
	else	{	
		setText(tr("Add Component"));
		repaint();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS

void AwICARejectButton::reject()
{
	m_rejected = !m_rejected;

	if (!m_rejected) {
		setText(tr("Reject Component"));
		repaint();
	}
	else	{	
		setText(tr("Add Component"));
		repaint();
	}
	m_item->setRejected(m_rejected);
}