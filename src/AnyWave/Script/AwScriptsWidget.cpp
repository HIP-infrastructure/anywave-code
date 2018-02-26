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
#include "AwScriptsWidget.h"

AwScriptsWidget::AwScriptsWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	connect(buttonClear, SIGNAL(clicked()), this, SLOT(clearList()));
}

AwScriptsWidget::~AwScriptsWidget()
{

}


void AwScriptsWidget::clear()
{
	for (int i = 0; i < listWidget->count(); i++)
		delete listWidget->item(i);
}

void AwScriptsWidget::addWidget(AwScriptWidget *sw)
{
	// item are inserted to the top of the list.
	QListWidgetItem *item = new QListWidgetItem();
	listWidget->insertItem(0, item);
	listWidget->setItemWidget(item, sw);
	item->setSizeHint(sw->size());

	connect(sw, SIGNAL(removeClicked(AwScriptWidget *)), this, SLOT(removeWidget(AwScriptWidget *)));
	buttonClear->show();
	this->show();
	this->raise();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwScriptsWidget::removeWidget(AwScriptWidget *sw)
{
	QListWidgetItem *item;
	for (int i = 0; i < listWidget->count(); i++) {
		AwScriptWidget *w = qobject_cast<AwScriptWidget *>(listWidget->itemWidget(listWidget->item(i)));
		if (w == sw) {
			item = listWidget->takeItem(i);
			break;
		}
	}

	delete item;

	if (listWidget->count() == 0)
		buttonClear->hide();
}


void AwScriptsWidget::clearList()
{
	QList<QListWidgetItem *> items;
	QList<AwScriptWidget *> widgets;
	for (int i = 0; i < listWidget->count(); i++) {
		AwScriptWidget *w = qobject_cast<AwScriptWidget *>(listWidget->itemWidget(listWidget->item(i)));
		//		if (!w->isIdle() && !w->isRunning())
		if (!w->isActive())	{
			items << listWidget->item(i);
			widgets << w;
		}
	}

	int i = 0;
	foreach (QListWidgetItem *item, items)
		delete item; // deleting an item will remove it from the QListWidget.

	if (listWidget->count() == 0)
		buttonClear->hide();
}
