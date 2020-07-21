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

	for (auto item : selectedItems) {
		if (item.column() == 0)
			m_graphSets.at(item.row())->visualize();
	}
}


void GraphManagerWidget::addGraphSet(const QString& file)
{
	GraphSet *ds = new GraphSet;
	if (ds->load(file) == 0) {
		m_graphSets << ds;
		updateTableView();
	}
	else {
		AwMessageBox::information(this, "Error", ds->errorString());
		delete ds;
	}
}


void GraphManagerWidget::loadFile()
{
	QString file = QFileDialog::getOpenFileName(this, "Load file", H2Graphs::instance()->pdi.input.settings[processio::data_dir].toString(), "Matlab Files (*.mat)");
	if (file.isEmpty())
		return;
	addGraphSet(file);
}
