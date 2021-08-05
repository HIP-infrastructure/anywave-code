#include "AwUpdaterGui.h"
#include "AwUpdateManager.h"
#include <qabstractitemview.h>
#include <QTableWidgetItem>
#include <widget/AwMessageBox.h>

AwUpdaterGui::AwUpdaterGui(AwUpdateManager *um, QWidget *parent)
	: QDialog(parent)
{
	m_ui.setupUi(this);
	m_updateManager = um;

	// init table widget
	m_ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); 
	m_ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
	m_ui.tableWidget->setColumnCount(4);
	QStringList headers = { "Component", "Type", "Current", "Available" };
	m_ui.tableWidget->setHorizontalHeaderLabels(headers);
	m_ui.tableWidget->verticalHeader()->setVisible(false);
	m_ui.tableWidget->setStyleSheet("QTableView {selection-background-color: green;}");
	m_ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	m_ui.tableWidget->verticalHeader()->hide();
	int row = 0;
	for (auto c : m_updateManager->components()) {
		int col = 0;
		if (c->updateAvailable == false)
			continue;
		m_ui.tableWidget->insertRow(row);
		auto item = new QTableWidgetItem(c->name);
		item->setTextAlignment(Qt::AlignVCenter|Qt::AlignHCenter);
		m_ui.tableWidget->setItem(row, col++, item);
		QString type = "core";
		if (c->type == AwUpdateManager::Plugin)
			type = "plugin";
		item = new QTableWidgetItem(type);
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		m_ui.tableWidget->setItem(row, col++, item);
		item = new QTableWidgetItem(c->installedVersion);
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		m_ui.tableWidget->setItem(row, col++, item);
		item = new QTableWidgetItem(c->version);
		item->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
		m_ui.tableWidget->setItem(row, col++, item);
		row++;
	}
	connect(m_ui.buttonSelectAll, &QPushButton::clicked, this, &AwUpdaterGui::selectAll);
	connect(m_ui.buttonUnselect, &QPushButton::clicked, this, &AwUpdaterGui::unselect);
}

AwUpdaterGui::~AwUpdaterGui()
{
}


void AwUpdaterGui::selectAll()
{
	m_ui.tableWidget->selectAll();
}

void AwUpdaterGui::unselect()
{
	m_ui.tableWidget->clearSelection();
}

void AwUpdaterGui::accept()
{
	auto selectionModel = m_ui.tableWidget->selectionModel();
	QStringList names;
	for (int i = 0; i < m_ui.tableWidget->rowCount(); i++) {
		if (selectionModel->isRowSelected(i)) {
			auto item = m_ui.tableWidget->item(i, 0);
			names << item->text();
		}
	}
	for (auto c : m_updateManager->components()) {
		if (names.contains(c->name))
			m_selectedComponents << c;
	}

	if (m_selectedComponents.isEmpty()) {
		AwMessageBox::information(nullptr, "Updates", "Select an item to update.");
		return;
	}
	QDialog::accept();
}