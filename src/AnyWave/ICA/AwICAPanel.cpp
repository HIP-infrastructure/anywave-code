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
#include "AwICAPanel.h"
#include "ui_AwICAPanel.h"
#include "AwICAManager.h"
#include "AwICAComponents.h"
#include "AwPanelItem.h"
#include "Display/AwDisplay.h"

AwICAPanel::AwICAPanel(int type, QWidget *parent)
	: QWidget(parent)
{
	ui = new Ui::AwICAPanelUI();
	ui->setupUi(this);
	m_type = type;
	m_columns = 4;	//default to 5 widgets by row
	AwICAComponents *comps = AwICAManager::instance()->getComponents(type);
	connect(ui->radioAll, SIGNAL(toggled(bool)), this, SLOT(showAllSelected(bool)));
	connect(AwDisplay::instance(), SIGNAL(selectedChannelsChanged(AwChannelList&)), this, SLOT(updateSelected(AwChannelList&)));
	if (comps) {
		m_items = comps->createPanelItems();
		int col = 0, m_rows = 0;
		foreach(AwPanelItem *item, m_items) {
			ui->gridLayout->addWidget(item, m_rows, col++);
			if (col > m_columns) {
				col = 0;
				m_rows++;
			}
			connect(item, SIGNAL(itemStateChanged(int, bool)), this, SLOT(updateItemState(int, bool)));
			connect(item, SIGNAL(showSignalClicked(const QString&)), AwDisplay::instance(), SLOT(makeChannelVisible(const QString&)));
			connect(item, SIGNAL(selected(const QString&, bool)), AwDisplay::instance(), SLOT(changeChannelsSelectionState(const QString&, bool)));
			connect(item, SIGNAL(itemStateChanged(const QString&, bool)), AwDisplay::instance(), SLOT(updateICAChannelRejected(const QString&, bool)));
			if (item->isRejected())
				m_rejectedIndexes.append(item->index());
		}
	}
}

AwICAPanel::~AwICAPanel()
{
	delete ui;
}

void AwICAPanel::grabMappings(const QString& filePath) 
{
	// get all visible items
	QList<QPixmap *> pixmaps;
	foreach (AwPanelItem *item, m_items) {
		if (item->isVisible()) {
			// copy the pixmap and add it to the list
			pixmaps << new QPixmap(item->capture());
		}
	}

	// build the big pixmap
	// base size of one widget
	QSize wSize = pixmaps.first()->size();
	// build a pixmap with a maximum of 4 mappings by rows
	QSize fSize;
	fSize.setWidth(wSize.width() * qMin(4, pixmaps.size()));
	int rows = pixmaps.size();
	int wHeight = 0; 
	do {
		wHeight += wSize.height();
		rows -= 4;
	} while (rows >= 0);
	fSize.setHeight(wHeight); 
	QPixmap finalPix(fSize);
	finalPix.fill();
	QPainter painter(&finalPix);
	int col = 0, row = 0;
	for (int i = 0; i < pixmaps.size(); i++) {
		painter.drawPixmap(col++ * wSize.width(), row * wSize.height(), wSize.width(), wSize.height(), *pixmaps.at(i));
		if (col > 3) {
			row++;
			col = 0;
		}
	}
	finalPix.save(filePath);
	while (!pixmaps.isEmpty())
		delete pixmaps.takeFirst();
}

void AwICAPanel::updateRejectedComponents(const QList<int>& indexes)
{
	foreach(AwPanelItem *item, m_items) {
		if (indexes.contains(item->index()))
			item->setRejected(true);
		else
			item->setRejected(false);
	}
}

void AwICAPanel::updateItemState(int index, bool rejected)
{
	if (rejected && !m_rejectedIndexes.contains(index))
		m_rejectedIndexes.append(index);
	else if (!rejected) 
		m_rejectedIndexes.removeAll(index);

	emit componentModified(index, rejected);  // target AwICAComponent object
}

void AwICAPanel::updateSelected(AwChannelList& channels)
{
	QStringList labels = AwChannel::getLabels(channels);
	foreach (AwPanelItem *item, m_items) {
		if (labels.contains(item->channel()->name())) {
			item->channel()->setSelected(true);
			item->updateSelected(true);
		}
		else {
			item->channel()->setSelected(false);
			item->updateSelected(false);
		}
	}
}

void AwICAPanel::showAllSelected(bool flag)
{
	foreach (AwPanelItem *item, m_items) {
		ui->gridLayout->removeWidget(item);
		item->setVisible(false);
	}

	int col = 0, m_rows = 0;

	if (flag) { // Show All
		foreach (AwPanelItem *item, m_items) {
			item->setVisible(true);
			ui->gridLayout->addWidget(item, m_rows, col++);
			if (col > m_columns) {
				col = 0;
				m_rows++;
			}
		}
	}
	else {
		foreach (AwPanelItem *item, m_items) {
			if (item->channel()->isSelected()) {
				item->setVisible(true);
				ui->gridLayout->addWidget(item, m_rows, col++);
				if (col > m_columns) {
					col = 0;
					m_rows++;
				}
			}
		}
	}
	
}
	
