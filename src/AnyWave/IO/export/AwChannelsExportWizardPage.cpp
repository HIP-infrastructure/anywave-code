#include "AwChannelsExportWizardPage.h"
#include "Data/AwDataManager.h"
#include <QTableWidget>
#include <QGridLayout>
#include <qpushbutton.h>
#include <QMenu>
#include <QAction>
#include <QHeaderView>

AwChannelsExportWizardPage::AwChannelsExportWizardPage(QWidget* parent) : QWizardPage(parent)
{
	setTitle("Select channels to export in file");
	m_table = new QTableWidget;
	m_table->setSelectionMode(QTableWidget::NoSelection);
	m_table->setEditTriggers(QTableWidget::NoEditTriggers);
	m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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
		m_selected = new QPushButton("&Selected Channels");
		connect(m_selected, &QPushButton::clicked, this, &AwChannelsExportWizardPage::setSelected);
	}
	else 
		m_channels = montage;
	fillTable(m_channels);

	m_asRecorded = new QPushButton("Raw Channels");
	connect(m_asRecorded, &QPushButton::clicked, this, &AwChannelsExportWizardPage::setAsRecorded);

	m_clear = new QPushButton("Clear");
	connect(m_clear, &QPushButton::clicked, this, &AwChannelsExportWizardPage::clear);
	m_montage = new QPushButton("Current Montage");
	connect(m_montage, &QPushButton::clicked, this, &AwChannelsExportWizardPage::setCurrentMontage);
	m_byTypes = new QPushButton("Add by Types");
	// get types from montage
	auto types = AwChannel::getTypesAsString(AwDataManager::instance()->rawChannels());
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
	if (selection.size()) {
		hLayout->addWidget(m_selected);
	}
	hLayout->addWidget(m_montage);
	hLayout->addWidget(m_asRecorded);
	hLayout->addWidget(m_byTypes);
	hLayout->addWidget(m_clear);
	
	QGridLayout* layout = new QGridLayout;
	layout->addWidget(m_table, 0, 0);
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
	m_channels += AwChannel::getChannelsOfType(AwDataManager::instance()->asRecordedChannels(), type);
    fillTable(m_channels);
}

bool AwChannelsExportWizardPage::isComplete() const
{
	return m_table->rowCount() > 0;
}
