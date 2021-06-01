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
#include "AwChannelSelectionDial.h"

#if QT_VERSION >  QT_VERSION_CHECK(4, 8, 0)
#include <QMenu>
#endif

AwChannelSelectionDial::AwChannelSelectionDial(const QList<AwGraphicsSignalItem *>& items, QWidget *parent)
	: QDialog(parent), m_items(items)
{
	setupUi(this);
	
	m_model = new AwChannelSelectionModel(m_items);
	m_proxyModel = new QSortFilterProxyModel();
	m_proxyModel->setSourceModel(m_model);

	tvChannels->setModel(m_proxyModel);
	tvChannels->setSortingEnabled(true);

	m_contextMenu = new QMenu(tvChannels);
	m_actSelectAll = new QAction(tr("Select All"), tvChannels);
	m_actUnselectAll = new QAction(tr("Unselect All"), tvChannels);
	m_actInvertSelection = new QAction(tr("Invert Selection"), tvChannels);

	m_actSelectMEG = new QAction(tr("Select MEG Channels"), tvChannels);
	m_actSelectEEG = new QAction(tr("Select EEG Channels"), tvChannels);
	m_actSelectSEEG = new QAction(tr("Select SEEG Channels"), tvChannels);
	m_actSelectTrigger = new QAction(tr("Select Trigger Channels"), tvChannels);

	m_contextMenu->addAction(m_actSelectAll);
	m_contextMenu->addAction(m_actUnselectAll);
	m_contextMenu->addAction(m_actInvertSelection);
	m_contextMenu->addSeparator();
	m_contextMenu->addAction(m_actSelectMEG);
	m_contextMenu->addAction(m_actSelectEEG);
	m_contextMenu->addAction(m_actSelectSEEG);
	m_contextMenu->addAction(m_actSelectTrigger);

	connect(m_actInvertSelection, SIGNAL(triggered()), this, SLOT(invertSelection()));
	connect(m_actUnselectAll, SIGNAL(triggered()), this, SLOT(unselectAll()));
	connect(m_actSelectAll, SIGNAL(triggered()), this, SLOT(selectAll()));
	connect(m_actSelectMEG, SIGNAL(triggered()), this, SLOT(selectMEG()));
	connect(m_actSelectEEG, SIGNAL(triggered()), this, SLOT(selectEEG()));
	connect(m_actSelectSEEG, SIGNAL(triggered()), this, SLOT(selectSEEG()));
	connect(m_actSelectTrigger, SIGNAL(triggered()), this, SLOT(selectTrigger()));
	connect(tvChannels, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contextMenu(const QPoint&)));
}

AwChannelSelectionDial::~AwChannelSelectionDial()
{
	delete m_model;
	delete m_proxyModel;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// EVENTS
void AwChannelSelectionDial::changeEvent(QEvent *e)
{
	if (e) {
		if (e->type() == QEvent::LanguageChange) 
			retranslateUi(this);
	}
}


void AwChannelSelectionDial::invertSelection()
{
	foreach (AwGraphicsSignalItem *item, m_items) {
		item->setSelected(!item->isSelected());
		item->channel()->setSelected(!item->isSelected());
	}
}

void AwChannelSelectionDial::unselectAll()
{
	foreach (AwGraphicsSignalItem *item, m_items) {
		item->setSelected(false);
		item->channel()->setSelected(false);
	}
}

void AwChannelSelectionDial::selectAll()
{
	foreach (AwGraphicsSignalItem *item, m_items) {
		item->setSelected(true);
		item->channel()->setSelected(true);
	}
}

void AwChannelSelectionDial::selectMEG()
{
	foreach (AwGraphicsSignalItem *item, m_items)
		if (item->channel()->isMEG()) {
			item->setSelected(true);
			item->channel()->setSelected(true);
		}
}

void AwChannelSelectionDial::selectEEG()
{
	foreach (AwGraphicsSignalItem *item, m_items)
		if (item->channel()->isEEG()) {
			item->setSelected(true);
			item->channel()->setSelected(true);
		}
}

void AwChannelSelectionDial::selectSEEG()
{
	foreach (AwGraphicsSignalItem *item, m_items)
		if (item->channel()->isSEEG()) {
			item->setSelected(true);
			item->channel()->setSelected(true);
		}
}

void AwChannelSelectionDial::selectTrigger()
{
	foreach (AwGraphicsSignalItem *item, m_items)
		if (item->channel()->isTrigger()) {
			item->setSelected(true);
			item->channel()->setSelected(true);
		}
}

void AwChannelSelectionDial::contextMenu(const QPoint& pos)
{
	m_contextMenu->exec(tvChannels->mapToGlobal(pos));
}