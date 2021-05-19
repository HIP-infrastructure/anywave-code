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
#include "AwPluginDial.h"
#include <QHeaderView>
#include <AwFileIO.h>
#include <AwProcessInterface.h>
#include <filter/AwFilterPlugin.h>
#include <graphics/AwGraphicInterface.h>
#include "Plugin/AwPluginManager.h"

AwPluginDial::AwPluginDial(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

	// Init TreeWidget
	// Un item parent pour chaque type de plugins
	treeWidget->setHeaderLabels({ "Name", "Description" });
	treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	auto writerRoot =  new QTreeWidgetItem(treeWidget);
	writerRoot->setText(0, tr("Writer"));
	treeWidget->setItemExpanded(writerRoot, true);
	writerRoot->setIcon(0, QIcon(":/images/ox_plugin_32.png"));

	auto processRoot = new QTreeWidgetItem(treeWidget);
	processRoot->setText(0, tr("Process"));
	treeWidget->setItemExpanded(processRoot, true);
	processRoot->setIcon(0, QIcon(":/images/system_run_32.png"));
	
	auto filterRoot = new QTreeWidgetItem(treeWidget);
	filterRoot->setText(0, tr("Filter"));
	treeWidget->setItemExpanded(filterRoot, true);
	processRoot->setIcon(0, QIcon(":/images/system_run_32.png"));

	auto readerRoot = new QTreeWidgetItem(treeWidget);
	readerRoot->setText(0, tr("Reader"));
	treeWidget->setItemExpanded(readerRoot, true);
	readerRoot->setIcon(0, QIcon(":/images/ox_plugin_32.png"));

	auto displayRoot = new QTreeWidgetItem(treeWidget);
	displayRoot->setText(0, tr("Display"));
	treeWidget->setItemExpanded(displayRoot, true);
	displayRoot->setIcon(0, QIcon(":/images/display-settings.png"));
	
	QFont boldFont = writerRoot->font(0);
    boldFont.setBold(true);
	writerRoot->setFont(0, boldFont);
	processRoot->setFont(0, boldFont);
	filterRoot->setFont(0, boldFont);
	readerRoot->setFont(0, boldFont);
	displayRoot->setFont(0, boldFont);
	
	setWindowTitle(tr("Loaded Plug-ins"));

	
	// fill in the table widget
	auto pm = AwPluginManager::getInstance();
	for (auto p : pm->readers()) {
		auto item = new QTreeWidgetItem(readerRoot);
		item->setText(0, p->name);
		item->setText(1, p->description);
		item->setIcon(0, m_featureIcon);
	}
	for (auto p : pm->writers()) {
		auto item = new QTreeWidgetItem(writerRoot);
		item->setText(0, p->name);
		item->setText(1, p->description);
		item->setIcon(0, m_featureIcon);
	}
	for (auto p : pm->processes()) {
		auto item = new QTreeWidgetItem(processRoot);
		item->setText(0, p->name);
		item->setText(1, p->description);
		item->setIcon(0, m_featureIcon);
	}
	for (auto p : pm->displays()) {
		auto item = new QTreeWidgetItem(displayRoot);
		item->setText(0, p->name);
		item->setText(1, p->description);
		item->setIcon(0, m_featureIcon);
	}
	for (auto p : pm->filters()) {
		auto item = new QTreeWidgetItem(filterRoot);
		item->setText(0, p->name);
		item->setText(1, p->description);
		item->setIcon(0, m_featureIcon);
	}
}



