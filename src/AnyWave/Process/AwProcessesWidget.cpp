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
#include "AwProcessesWidget.h"
#include "Process/AwProcessManager.h"


AwProcessesWidget::AwProcessesWidget(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	connect(buttonClear, SIGNAL(clicked()), this, SLOT(clearList()));
}

AwProcessesWidget::~AwProcessesWidget()
{

}

void AwProcessesWidget::clear()
{
	for (int i = 0; i < listWidget->count(); i++) {
		QListWidgetItem *item = listWidget->item(i);
		AwProcessWidget *pw = (AwProcessWidget *)listWidget->itemWidget(item);
		pw->clear();
		//delete item;
	}
	listWidget->clear();
}

void AwProcessesWidget::addWidget(AwProcessWidget *pw)
{
	// item are inserted to the top of the list.
	QListWidgetItem *item = new QListWidgetItem();
	listWidget->insertItem(0, item);
	listWidget->setItemWidget(item, pw);
	item->setSizeHint(pw->size());

	connect(pw, SIGNAL(removeClicked(AwProcessWidget *)), this, SLOT(removeWidget(AwProcessWidget *)));
	buttonClear->show();
	this->show();
	this->raise();
}

void AwProcessesWidget::changeEvent(QEvent *e)
{
	if (e)
		if (e->type() == QEvent::LanguageChange)
			retranslateUi(this);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwProcessesWidget::removeWidget(AwProcessWidget *pw)
{
	QListWidgetItem *item;
	for (int i = 0; i < listWidget->count(); i++)	{
		AwProcessWidget *w = qobject_cast<AwProcessWidget *>(listWidget->itemWidget(listWidget->item(i)));
		if (w == pw) {
			item = listWidget->takeItem(i);
			break;
		}
	}

	delete item;

	if (listWidget->count() == 0)
		buttonClear->hide();
}

void AwProcessesWidget::clearList()
{
	QList<QListWidgetItem *> items;
	QList<AwProcessWidget *> widgets;
	for (int i = 0; i < listWidget->count(); i++) {
		AwProcessWidget *w = qobject_cast<AwProcessWidget *>(listWidget->itemWidget(listWidget->item(i)));
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