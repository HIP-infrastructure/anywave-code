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
	auto pm = AwPluginManager::getInstance();
	setWindowTitle("Loaded plugins");
	setWindowIcon(QIcon(":/images/AnyWave_icon.png"));

	// Init TreeWidget
	// Un item parent pour chaque type de plugins
	treeWidget->setHeaderLabels({ "Name", "Version", "Description" });
	treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    treeWidget->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	auto ioPlugins = pm->readers();
	for (auto p : pm->writers()) {
		if (!ioPlugins.contains(p))
			ioPlugins << p;
	}

	if (!ioPlugins.isEmpty()) {
		auto ioRoot = new QTreeWidgetItem(treeWidget);
		QFont boldFont = ioRoot->font(0);
		ioRoot->setText(0, tr("I/O"));
		ioRoot->setIcon(0, QIcon(":/images/ox_plugin_32.png"));
		ioRoot->setFont(0, boldFont);
		for (auto p : ioPlugins) {
			auto item = new QTreeWidgetItem(ioRoot);
			item->setText(0, p->name);
			item->setText(1, p->version);
			item->setTextAlignment(1, Qt::AlignHCenter);
			item->setText(2, p->description);
			item->setIcon(0, m_featureIcon);
		}
	}
	if (!pm->processes().isEmpty()) {
		auto processRoot = new QTreeWidgetItem(treeWidget);
		QFont boldFont = processRoot->font(0);
		processRoot->setText(0, tr("Process"));
		processRoot->setIcon(0, QIcon(":/images/system_run_32.png"));
		processRoot->setFont(0, boldFont);
		for (auto p : pm->processes()) {
			auto item = new QTreeWidgetItem(processRoot);
			item->setText(0, p->name);
			item->setText(1, p->version);
			item->setTextAlignment(1, Qt::AlignHCenter);
			item->setText(2, p->description);
			item->setIcon(0, m_featureIcon);
		}
	}

	if (!pm->displays().isEmpty()) {
		auto displayRoot = new QTreeWidgetItem(treeWidget);
		QFont boldFont = displayRoot->font(0);
		displayRoot->setText(0, tr("Display"));
		displayRoot->setIcon(0, QIcon(":/images/display-settings.png"));
		displayRoot->setFont(0, boldFont);
		for (auto p : pm->displays()) {
			auto item = new QTreeWidgetItem(displayRoot);
			item->setText(0, p->name);
			item->setText(1, p->version);
			item->setTextAlignment(1, Qt::AlignHCenter);
			item->setText(2, p->description);
			item->setIcon(0, m_featureIcon);
		}
	}

	if (!pm->filters().isEmpty()) {
		auto filterRoot = new QTreeWidgetItem(treeWidget);
		QFont boldFont = filterRoot->font(0);
		filterRoot->setText(0, tr("Filters"));
		filterRoot->setFont(0, boldFont);
		filterRoot->setIcon(0, QIcon(":/images/ox_view_filter_32.png"));
		for (auto p : pm->filters()) {
			auto item = new QTreeWidgetItem(filterRoot);
			item->setText(0, p->name);
			item->setText(1, p->version);
			item->setTextAlignment(1, Qt::AlignHCenter);
			item->setText(2, p->description);
			item->setIcon(0, m_featureIcon);
		}
	}
}



