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
#include "GraphManagerWidget.h"
#include <widget/AwMessageBox.h>
#include "GraphSetModel.h"
#include <qfiledialog.h>
#include "H2Graphs.h"
#include <AwKeys.h>


GraphManagerWidget::GraphManagerWidget(AwGUIProcess *process, QWidget *parent)
	: AwProcessGUIWidget(process, parent)
{
	m_ui.setupUi(this);

	m_ui.tableView->setModel(new GraphSetModel(this));
	m_ui.tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(m_ui.buttonImport, &QPushButton::clicked, this, &GraphManagerWidget::loadFile);
	connect(m_ui.buttonView, &QPushButton::clicked, this, &GraphManagerWidget::visualize);
}

GraphManagerWidget::~GraphManagerWidget()
{

}


void GraphManagerWidget::updateTableView()
{
	GraphSetModel *m = (GraphSetModel *)m_ui.tableView->model();
	m->update(m_graphSets);
}


void GraphManagerWidget::visualize()
{
	QModelIndexList selectedItems = m_ui.tableView->selectionModel()->selectedIndexes();
	if (selectedItems.isEmpty())
		return;

	for (auto  && item : selectedItems) {
		if (item.column() == 0)
			m_graphSets.at(item.row())->visualize();
	}
}


int GraphManagerWidget::addGraphSet(const QString& file)
{
	GraphSet *ds = new GraphSet;
	if (ds->load(file) == 0) {
		m_graphSets << ds;
		updateTableView();
		return 0;
	}
	else {
		//AwMessageBox::information(this, "Error", ds->errorString());
		//delete ds;
		return -1;
	}
}


void GraphManagerWidget::loadFile()
{
	QString file = QFileDialog::getOpenFileName(this, "Load file", H2Graphs::instance()->pdi.input.settings[keys::data_dir].toString(), "Matlab Files (*.mat)");
	if (file.isEmpty())
		return;
	if (addGraphSet(file) != 0) 
		AwMessageBox::information(this, "Error", "Failed to load .mat file");
}
