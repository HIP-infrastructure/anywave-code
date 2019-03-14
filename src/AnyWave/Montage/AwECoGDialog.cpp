#include "AwECoGDialog.h"
#include <QListWidgetItem>
#include <qpushbutton.h>
#include <QMessageBox>
#include <QMenu>
#include "AwECoGTableModel.h"
#include <algorithm>


AwECoGDialog::AwECoGDialog(const AwChannelList& channels, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_channels = channels;
	ui.listWidget->addItems(AwChannel::getLabels(channels));
	connect(ui.buttonGrid, &QPushButton::clicked, this, &AwECoGDialog::addGridStrip);
	ui.tableView->setModel(new AwECoGTableModel(this));
	ui.tableView->setItemDelegate(new AwECoGModelDelegate(this));
	ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableView->resizeColumnsToContents();
}

AwECoGDialog::~AwECoGDialog()
{
}

void AwECoGDialog::addGridStrip()
{
	auto selection = ui.listWidget->selectedItems();
	if (selection.size() < 2) {
		QMessageBox::information(this, tr("Electrodes"), tr("Please select at least 2 electrodes"));
		return;
	}

	// build a popup menu depending on the number of selected items
	auto menu = ui.buttonGrid->menu();
	if (menu == Q_NULLPTR)
		menu = new QMenu(ui.buttonGrid);
	menu->clear();
	auto count = selection.size();
	QString strip = QString("Strip - 1x%1").arg(count);
	auto action = menu->addAction(strip);
	action->setData((int)1);
	connect(action, &QAction::triggered, this, &AwECoGDialog::addGrid);

	// test number of electrodes for even or odd
	int divisor = 2;
	int quotient = count / divisor;
	int remainder = count % divisor;
	QMap<int, int> grids;
	// compute even combinations
	while (quotient > 0 && remainder == 0) {
		grids[divisor] = quotient;
		divisor += 2;
		quotient = count / divisor;
		remainder = count % divisor;
	}
	// compute odd combinations
	divisor = 3;
	quotient = count / divisor;
	remainder = count % divisor;
	while (quotient > 0 && remainder == 0) {
		grids[divisor] = quotient;
		divisor += 2;
		quotient = count / divisor;
		remainder = count % divisor;
	}
	// compute reversed combinations
	for (auto r : grids.keys()) {
		if (!grids.contains(grids[r]))
			grids[grids[r]] = r;
	}
	auto keys = grids.keys();
	std::sort(keys.begin(), keys.end());
	for (auto r : keys) {
		QString str = QString("Grid - %1x%2").arg(r).arg(grids[r]);
		auto action = menu->addAction(str);
		action->setData(r);
		connect(action, &QAction::triggered, this, &AwECoGDialog::addGrid);
	}
	ui.buttonGrid->setMenu(menu);
	ui.buttonGrid->showMenu();
}

void AwECoGDialog::addGrid()
{
	QAction *action = static_cast<QAction *>(sender());
	if (action == Q_NULLPTR)
		return;
	int rows = action->data().toInt();
	auto selection = ui.listWidget->selectedItems();
	int cols = selection.size() / rows;
	QStringList labels;
	for (auto i : selection) {
		labels << i->text();
		ui.listWidget->removeItemWidget(i);
	}
	auto grid = new AwECoGGrid(rows, cols, labels);
	static_cast<AwECoGTableModel *>(ui.tableView->model())->addGrid(grid);
}

void AwECoGDialog::accept()
{
	auto grids = static_cast<AwECoGTableModel *>(ui.tableView->model())->grids();
	for (auto g : grids) {
		QStringList labels;
		int limit1 = 0, limit2 = 0;
		if (g->orientation() == AwECoGGrid::Horizontal) {
			limit1 = g->rows();
			limit2 = g->cols();
		}
		else {
			limit1 = g->cols();
			limit2 = g->rows();
		}
		for (int i = 0; i < limit1; i++) {
			for (int j = 0; j < limit2; j++) {
				labels << g->labels().value(i * limit2 + j);
			}
			auto channels = AwChannel::getChannelsWithLabels(m_channels, labels);
			int count = 0;
			for (int k = 0; k < channels.size() - 1; k++) {
				channels.at(k)->setReferenceName(labels.value(k + 1));
			}
			labels.clear();
		}
	}
	QDialog::accept();
}

void AwECoGDialog::reset()
{
	static_cast<AwECoGTableModel *>(ui.tableView->model())->clearGrids();
	ui.listWidget->clear();
	ui.listWidget->addItems(AwChannel::getLabels(m_channels));
}