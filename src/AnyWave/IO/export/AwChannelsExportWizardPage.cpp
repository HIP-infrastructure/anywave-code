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
#include "AwChannelsExportWizardPage.h"
#include "Data/AwDataManager.h"
#include <QTableWidget>
#include <QGridLayout>
#include <qpushbutton.h>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QGroupBox>
#include <QLabel>

AwChannelsExportWizardPage::AwChannelsExportWizardPage(QWidget* parent) : QWizardPage(parent)
{
	setTitle("Select channels to export in file");
	auto group = new QGroupBox;
	group->setTitle("Channels to export:");
	m_table = new QTableWidget;
	m_table->setSelectionMode(QTableWidget::NoSelection);
	m_table->setEditTriggers(QTableWidget::NoEditTriggers);
	m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_table->setAlternatingRowColors(true);
	auto montage = AwDataManager::instance()->montage();
	auto selection = AwDataManager::instance()->selectedChannels();
	m_selected = m_asRecorded = m_byTypes = m_montage = m_clear = nullptr;
	int row = 0;
	m_table->setColumnCount(2);
	auto headerChannel = new QTableWidgetItem("Channel");
	headerChannel->setTextAlignment(Qt::AlignHCenter);
	auto headerType = new QTableWidgetItem("Type");
	headerType->setTextAlignment(Qt::AlignHCenter);
	m_table->setHorizontalHeaderItem(0, headerChannel);
	m_table->setHorizontalHeaderItem(1, headerType);
	if (selection.size()) {
		m_channels = selection;
		m_selected = new QPushButton("Export Selected Channels");
		connect(m_selected, &QPushButton::clicked, this, &AwChannelsExportWizardPage::setSelected);
	}
	else
		m_channels = montage;
	fillTable(m_channels);

	m_asRecorded = new QPushButton("Export Raw Channels");
	m_asRecorded->setToolTip("Click here to export only raw channels (no montage is used)");
	connect(m_asRecorded, &QPushButton::clicked, this, &AwChannelsExportWizardPage::setAsRecorded);

	m_clear = new QPushButton("Clear");
	m_clear->setToolTip("Clear the current list and allow you to build your own list using Add by types");
	connect(m_clear, &QPushButton::clicked, this, &AwChannelsExportWizardPage::clear);
	m_montage = new QPushButton("Export Current Montage");
	connect(m_montage, &QPushButton::clicked, this, &AwChannelsExportWizardPage::setCurrentMontage);
	m_byTypes = new QPushButton("Add by Types");
	// get types from montage
	auto types = AwChannel::getTypesAsString(AwDataManager::instance()->rawChannels() + AwDataManager::instance()->montage());
	QMenu* menu = new QMenu;
	for (const auto& type : types) {
		auto action = new QAction(type);
		action->setData(AwChannel::stringToType(type));
		connect(action, &QAction::triggered, this, &AwChannelsExportWizardPage::addByTypes);
		menu->addAction(action);
	}
	m_byTypes->setMenu(menu);

	// prepare horizontal layout
	QHBoxLayout* hLayout = new QHBoxLayout;
	auto label = new QLabel("Change channels:");
	hLayout->addWidget(label);
	int strechIndex = 3;
	if (selection.size()) {
		hLayout->addWidget(m_selected);
		strechIndex++;
	}
	hLayout->addWidget(m_montage);
	hLayout->addWidget(m_asRecorded);
	hLayout->addWidget(m_byTypes);
	hLayout->addWidget(m_clear);
	hLayout->insertStretch(strechIndex, 1);
	QVBoxLayout* vLayout = new QVBoxLayout;
	vLayout->addWidget(m_table);
	group->setLayout(vLayout);

	QGridLayout* layout = new QGridLayout;
	layout->addWidget(group, 0, 0);
	layout->addLayout(hLayout, 1, 0);
	setLayout(layout);
}

void AwChannelsExportWizardPage::fillTable(const AwChannelList& channels)
{
	while (m_table->rowCount())
		m_table->removeRow(0);
	m_table->setRowCount(channels.size());
	int row = 0;
	for (auto channel : channels) {
		auto item = new QTableWidgetItem(channel->fullName());
		item->setTextAlignment(Qt::AlignHCenter);
		m_table->setItem(row, 0, item);
		item = new QTableWidgetItem(AwChannel::typeToString(channel->type()));
		item->setTextAlignment(Qt::AlignHCenter);
		m_table->setItem(row++, 1, item);
	}
	emit completeChanged();
}

void AwChannelsExportWizardPage::clear()
{
	m_channels.clear();
	fillTable(m_channels);
	emit completeChanged();
 }

void AwChannelsExportWizardPage::setCurrentMontage()
{
	m_channels = AwDataManager::instance()->montage();
	fillTable(m_channels);
}

void AwChannelsExportWizardPage::setAsRecorded()
{
	m_channels = AwDataManager::instance()->rawChannels();
	fillTable(m_channels);
}

void AwChannelsExportWizardPage::setSelected()
{
	m_channels = AwDataManager::instance()->selectedChannels();
	fillTable(m_channels);
}


void AwChannelsExportWizardPage::addByTypes()
{
	QAction* action = static_cast<QAction*>(sender());
	if (action == nullptr)
		return;
	int type = action->data().toInt();
	m_channels += AwChannel::getChannelsOfType(AwDataManager::instance()->asRecordedChannels() + 
		AwDataManager::instance()->montage(), type);
    fillTable(m_channels);
}

bool AwChannelsExportWizardPage::isComplete() const
{
	return m_table->rowCount() > 0;
}
