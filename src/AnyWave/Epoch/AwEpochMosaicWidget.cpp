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
#include "AwEpochMosaicWidget.h"
#include "AwEpochManager.h"
#include <widget/AwWaitWidget.h>


AwEpochMosaicWidget::AwEpochMosaicWidget(AwEpochTree *condition, AwEpochVisuWidget *parent)
	: QWidget(Q_NULLPTR)
{
	ui.setupUi(this);
	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setSpacing(5);
	ui.listWidget->setLayoutMode(QListWidget::Batched);
	ui.listWidget->setResizeMode(QListWidget::Adjust);

	m_parent = parent;
	if (condition == Q_NULLPTR)
		condition = AwEpochManager::instance()->conditions().first();
	else 
		m_condition = condition;
	setWindowTitle(QString("Icon view for condition %1").arg(condition->name()));
	ui.listWidget->setIconSize(QSize(200, 150));
	connect(ui.listWidget, &QListWidget::itemClicked, this, &AwEpochMosaicWidget::changeEpoch);
}

AwEpochMosaicWidget::~AwEpochMosaicWidget()
{
}

void AwEpochMosaicWidget::setThumbs(thumbList *thumbs)
{
	int i = 0;
	for (auto t : *thumbs) {
		auto item = new QListWidgetItem(ui.listWidget);
		item->setIcon(QIcon(t->pixmap));
		item->setData(Qt::UserRole, i);
		item->setToolTip(QString("Epoch %1").arg(i));
		i++;
	};
	ui.listWidget->update();
}


void AwEpochMosaicWidget::changeEpoch(QListWidgetItem *item)
{
	emit itemClicked(m_condition, item->data(Qt::UserRole).toInt());
}