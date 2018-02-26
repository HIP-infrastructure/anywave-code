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

#include "AwPluginDial.h"
#include <QHeaderView>
#include <AwFileIO.h>
#include <AwProcessInterface.h>
#include <graphics/AwGraphicInterface.h>

AwPluginDial::AwPluginDial(const QList<AwFileIOPlugin *>& readers,
						   const QList<AwFileIOPlugin *>& writers,
						   const QList<AwProcessPlugin *>& processes,
						   const QList<AwDisplayPlugin *>& displayers, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

	// Init TreeWidget
	// Un item parent pour chaque type de plugins
	QStringList labels;

	labels << tr("Name") << tr("Description");
	treeWidget->setHeaderLabels(labels);
#if QT_VERSION < 5
	treeWidget->header()->setResizeMode(0, QHeaderView::Stretch);
    treeWidget->header()->setResizeMode(1, QHeaderView::Stretch);
#else
	treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);
#endif

	m_writerItem = new QTreeWidgetItem(treeWidget);
	m_writerItem->setText(0, tr("Writer"));
	treeWidget->setItemExpanded(m_writerItem, true);
	m_writerItem->setIcon(0, QIcon(":/images/ox_plugin_32.png"));

	m_processItem = new QTreeWidgetItem(treeWidget);
	m_processItem->setText(0, tr("Process"));
	treeWidget->setItemExpanded(m_processItem, true);
	m_processItem->setIcon(0, QIcon(":/images/system_run_32.png"));
	
	m_readerItem = new QTreeWidgetItem(treeWidget);
	m_readerItem->setText(0, tr("Reader"));
	treeWidget->setItemExpanded(m_readerItem, true);
	m_readerItem->setIcon(0, QIcon(":/images/ox_plugin_32.png"));

	m_displayItem = new QTreeWidgetItem(treeWidget);
	m_displayItem->setText(0, tr("Display"));
	treeWidget->setItemExpanded(m_displayItem, true);
	m_displayItem->setIcon(0, QIcon(":/images/display-settings.png"));



	QFont boldFont = m_readerItem->font(0);
    boldFont.setBold(true);
    m_readerItem->setFont(0, boldFont);
 	m_writerItem->setFont(0, boldFont);
	m_processItem->setFont(0, boldFont);
	m_displayItem->setFont(0, boldFont);
	
	setWindowTitle(tr("Loaded Plug-ins"));

	
	// fill in the table widget
	QTreeWidgetItem *pluginItem = NULL; 
	if (!readers.isEmpty())	{
		foreach (AwFileIOPlugin *p, readers)	{
			 pluginItem = new QTreeWidgetItem(m_readerItem);
			 pluginItem->setText(0, p->name);
			 pluginItem->setText(1, p->description);
			 pluginItem->setIcon(0, m_featureIcon);
		}
	}
	if (!writers.isEmpty()) {
		foreach (AwFileIOPlugin *p, writers) {
			 pluginItem = new QTreeWidgetItem(m_writerItem);
			 pluginItem->setText(0, p->name);
			 pluginItem->setText(1, p->description);
			 pluginItem->setIcon(0, m_featureIcon);
		}
	}
	if (!processes.isEmpty())	{
		foreach (AwProcessPlugin *p, processes) {
			 pluginItem = new QTreeWidgetItem(m_processItem);
			 pluginItem->setText(0, p->name);
			 pluginItem->setText(1, p->description);
			 pluginItem->setIcon(0, m_featureIcon);
		}
	}
	if (!displayers.isEmpty())	{
		foreach (AwDisplayPlugin *p, displayers)	{
			 pluginItem = new QTreeWidgetItem(m_displayItem);
			 pluginItem->setText(0, p->name);
			 pluginItem->setText(1, p->description);
			 pluginItem->setIcon(0, m_featureIcon);
		}
	}
}



