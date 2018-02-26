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

#include "AwMontageTableView.h"

#include <qmimedata.h>

AwMontageTableView::AwMontageTableView(QWidget *parent)
	: QTableView(parent)
{

}

AwMontageTableView::~AwMontageTableView()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS

// mouseReleaseEvent():
// used to check if the left mouse button was released over a valid index of the model.
// if so, emit a message with the corresponding index.
// Avoid to confuse with right click button event that shall only be used for context menu.
void AwMontageTableView::mouseReleaseEvent(QMouseEvent *event)
{
	QTableView::mouseReleaseEvent(event);
	if (event->button() != Qt::LeftButton) // only filter for left mouse button
		return;

	QModelIndex index = indexAt(event->pos());
	if (!index.isValid())
		return;

	emit clickedOnIndex(index);
}

void AwMontageTableView::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("text/plain") || event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
		event->acceptProposedAction();
	else
		event->setDropAction(Qt::IgnoreAction);
}


void AwMontageTableView::dragMoveEvent(QDragMoveEvent *event)
{
	if (!event->mimeData()->hasFormat("text/plain") && 
		!event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
	{
		event->ignore();
		return;
	}

	event->accept();
}