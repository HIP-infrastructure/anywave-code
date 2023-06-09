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
#include "AwEpochVisuWidget.h"
#include "AwEpochManager.h"
#include <QtConcurrent>
#include <qwt_plot.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_renderer.h>
#include <AwAmplitudeManager.h>
#include "AwEpochMosaicWidget.h"


//////////////////////////////////////////////////////////////////////////////////////////
/// EpochLoader implementation
AwEpochLoader::AwEpochLoader(const QString& title, AwEpochTree *condition, QWidget *parent) : AwWaitWidget(title, parent)
{
	m_condition = condition;
	m_signalView = new AwEpochSignalView;
	m_signalView->resize(QSize(800, 600));
	m_signalView->setAttribute(Qt::WA_DontShowOnScreen);
	m_signalView->setFlags(AwBaseSignalView::NoNavBar | AwBaseSignalView::NoMarkerBar | AwBaseSignalView::ViewAllChannels);
	m_signalView->show();
	m_thumbs = new thumbList;
}

AwEpochLoader::~AwEpochLoader()
{
	delete m_signalView;
}

void AwEpochLoader::loadEpochs()
{
	int i = 0;
	for (auto e : m_condition->epochs()) {
		if (!e->isLoaded())
			m_condition->loadEpoch(e);
		auto t = new thumb;
		t->epoch = e;
		m_signalView->setEpoch(e);
		m_signalView->repaint();
	    t->pixmap = QPixmap(QSize(800, 600));
		t->pixmap = m_signalView->grab();
		t->pixmap = t->pixmap.scaled(QSize(200, 150), Qt::KeepAspectRatio);
	   *m_thumbs << t;
	   setCurrentProgress(i++);
	}
	accept();
}


int AwEpochLoader::exec()
{
	QTimer::singleShot(0, this, &AwEpochLoader::loadEpochs);
	return AwWaitWidget::exec();
}
//////////////////////////////////////////////////////////////////////////////////////////

AwEpochVisuWidget::AwEpochVisuWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ui.setupUi(this);
	m_ui.treeView->setModel(&m_treeViewModel);
	m_ui.treeView->header()->setSectionResizeMode(QHeaderView::Stretch);
	m_ui.treeView->header()->setDefaultAlignment(Qt::AlignCenter);
	connect(m_ui.treeView, &QTreeView::clicked, this, &AwEpochVisuWidget::showEpoch);
	connect(&m_treeViewModel, &QStandardItemModel::itemChanged, this, &AwEpochVisuWidget::updateItem);
	m_currentCondition = NULL;
	m_currentEpochIndex = -1;
	updateConditions();
	m_signalView = new AwEpochSignalView();
	m_ui.groupBoxEpoch->layout()->replaceWidget(m_ui.widget, m_signalView);
	// update navigation widgets
	for (auto c : AwEpochManager::instance()->conditions()) 
		m_ui.comboCondition->addItem(c->name());
	m_ui.comboCondition->setCurrentIndex(0);
	connect(m_ui.comboCondition, &QComboBox::currentTextChanged, this, &AwEpochVisuWidget::changeCondition);
	connect(m_ui.buttonNext, &QPushButton::clicked, this, &AwEpochVisuWidget::nextEpoch);
	connect(m_ui.buttonPrev, &QPushButton::clicked, this, &AwEpochVisuWidget::prevEpoch);
	connect(m_ui.buttonReject, &QPushButton::clicked, this, &AwEpochVisuWidget::rejectEpoch);
	m_mosaicWidget = Q_NULLPTR;
	repaint();
}

AwEpochVisuWidget::~AwEpochVisuWidget()
{
	delete m_signalView;
	for (auto thumbs : m_hashThumbs.values()) {
		while (!thumbs->isEmpty())
			delete thumbs->takeFirst();
		delete thumbs;
	}
}

void AwEpochVisuWidget::prevEpoch()
{
	if (m_currentEpochIndex) {
		m_currentEpochIndex--;
		selectCurrentEpoch();
	}
}

void AwEpochVisuWidget::nextEpoch()
{
	if (m_currentEpochIndex + 1 < m_currentCondition->epochs().size()) {
		m_currentEpochIndex++;
		selectCurrentEpoch();
	}
}

void AwEpochVisuWidget::rejectEpoch()
{
	if (m_currentCondition == NULL || m_currentEpochIndex < 0)
		return;
	// select the corresponding epoch in the treeview
	QList<QStandardItem *> items = m_treeViewModel.findItems(m_currentCondition->name());
	if (items.isEmpty()) // should never happen
		return;
	QStandardItem *cond_item = items.first();
	if (!cond_item->hasChildren())
		return;

	QStandardItem *epoch_item = cond_item->child(m_currentEpochIndex, 1);
	AwEpoch *epoch = m_currentCondition->epochs().at(m_currentEpochIndex);
	epoch->setRejected(!epoch->isRejected());
	epoch->isRejected() ? epoch_item->setCheckState(Qt::Checked) : epoch_item->setCheckState(Qt::Unchecked);
	updateNavBar();
}

void AwEpochVisuWidget::changeCondition(const QString& condition)
{
	m_currentCondition = AwEpochManager::instance()->getCondition(condition);
	m_currentEpochIndex = 0;
	// select the corresponding epoch in the treeview
	QList<QStandardItem *> items = m_treeViewModel.findItems(condition);
	if (items.isEmpty()) // should never happen
		return;
	QStandardItem *item = items.first();
	if (item->hasChildren()) {
		m_ui.treeView->collapseAll();
		// expand the root item
		m_ui.treeView->expand(item->index());
		m_ui.treeView->selectionModel()->select(item->child(0)->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
		viewCurrentEpoch();
	}
}

///
/// Change the current selected epoch in the three view
/// Trigger the loading and preview of the epoch.
void AwEpochVisuWidget::selectCurrentEpoch()
{
	// select the corresponding epoch in the treeview
	QList<QStandardItem *> items = m_treeViewModel.findItems(m_currentCondition->name());
	if (items.isEmpty()) // should never happen
		return;
	QStandardItem *cond_item = items.first();
	if (!cond_item->hasChildren())
		return;
	m_ui.treeView->selectionModel()->select(cond_item->child(m_currentEpochIndex)->index(), QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
	viewCurrentEpoch();
	updateNavBar();
}



QList<QStandardItem *> AwEpochVisuWidget::createChildEpochs(AwEpochTree *condition)
{
	QList<QStandardItem *> list;
	int index = 1;
	for (int i = 0; i < condition->epochs().size(); i++) {
		QStandardItem *label = new QStandardItem(QString("epoch %1").arg(index++));
		label->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		label->setData(i);
		list << label;
	}
	return list;
}

QList<QStandardItem *> AwEpochVisuWidget::createChildStatus(AwEpochTree *condition)
{
	QList<QStandardItem *> list;
	for (int i = 0; i < condition->epochs().size(); i++) {
		QStandardItem *status = new QStandardItem(QString("rejected"));
		AwEpoch *epoch = condition->epochs().at(i);
		status->setCheckable(true);
		status->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
		status->setData(i);	// store index of epoch in the list
		epoch->isRejected() ? status->setCheckState(Qt::Checked) : status->setCheckState(Qt::Unchecked);
		list  << status;
	}
	return list;
}

void AwEpochVisuWidget::updateConditions()
{
	m_treeViewModel.clear();
	m_treeViewModel.setColumnCount(2);
	m_treeViewModel.setHeaderData(0, Qt::Horizontal, "Conditions");
	m_treeViewModel.setHeaderData(1, Qt::Horizontal, "Status");

	QList<AwEpochTree *> conditions = AwEpochManager::instance()->conditions();
	for (auto cond : conditions) {
		QStandardItem *condition = new QStandardItem(cond->name());
		condition->insertColumn(0, createChildEpochs(cond));
		condition->insertColumn(1, createChildStatus(cond));
		m_treeViewModel.appendRow(condition);
	}
}


void AwEpochVisuWidget::updateItem(QStandardItem *item)
{
	QStandardItem *parent = item->parent();
	if (parent == 0)   // should never happen as only child epoch items are editable (checkable)
		return;

	QString name = parent->data(Qt::DisplayRole).toString();
	m_currentCondition = AwEpochManager::instance()->getCondition(name);
	if (m_currentCondition == NULL)
		return;
	// update current epoch
	AwEpoch *epoch = m_currentCondition->epochs().at(item->data().toInt());
	epoch->setRejected(item->checkState() == Qt::Checked);
	viewCurrentEpoch();
}

void AwEpochVisuWidget::viewCurrentEpoch()
{
	auto epoch = m_currentCondition->epochs().at(m_currentEpochIndex);
	if (epoch == NULL)
		return;
	if (!epoch->isLoaded())
		m_currentCondition->loadEpoch(epoch);
	m_signalView->setEpoch(epoch);
	m_signalView->show();
	repaint();
}

void AwEpochVisuWidget::showEpoch(const QModelIndex& index)
{
	// choose the epoch column anyway.
	QStandardItem *item = m_treeViewModel.itemFromIndex(index);
	if (item == 0)
		return;

	QStandardItem *parent = item->parent();
	if (parent == 0)   // no parent means the user clicked on condition item
		return;
	QString name = parent->data(Qt::DisplayRole).toString();
	m_currentCondition = AwEpochManager::instance()->getCondition(name);

	if (m_currentCondition == NULL)
		return;
	int i = item->data().toInt();
	if (i != m_currentEpochIndex) {
		m_currentEpochIndex = i;
		viewCurrentEpoch();
	}
	emit epochClicked(m_currentCondition, m_currentEpochIndex);
	// update navigation bar
	updateNavBarCondition();
}

void AwEpochVisuWidget::updateNavBarCondition()
{
	disconnect(m_ui.comboCondition);
	m_ui.comboCondition->setCurrentText(m_currentCondition->name());
	connect(m_ui.comboCondition, &QComboBox::currentTextChanged, this, &AwEpochVisuWidget::changeCondition);
	updateNavBar();	// update widgets
}

void AwEpochVisuWidget::updateNavBar()
{
	auto epoch = m_currentCondition->epochs().at(m_currentEpochIndex);
	QString status = epoch->isRejected() ? "REJECTED" : "OK";
	QString message = QString("Epoch %1 is %2").arg(m_currentEpochIndex + 1).arg(status);
	if (epoch->isRejected()) {
		m_ui.labelEpoch->setStyleSheet("QLabel { color: red; text-align:center; font-family: Arial; font-size: 16pt; }");
		m_ui.buttonReject->setText("Undo Reject");
	}
	else {
		m_ui.labelEpoch->setStyleSheet("QLabel { color: green; text-align:center; font-family: Arial; font-size: 16pt; }");
		m_ui.buttonReject->setText("Reject");
	}
	m_ui.labelEpoch->setText(message);
}


void AwEpochVisuWidget::doAveraging()
{
	this->close();
	AwEpochManager::instance()->average();
}

void AwEpochVisuWidget::openMosaicView()
{
	if (m_currentCondition == Q_NULLPTR) {
		m_currentCondition = AwEpochManager::instance()->conditions().first();
		m_currentEpochIndex = 0;
	}
	if (m_mosaicWidget == Q_NULLPTR) {
		m_mosaicWidget = new AwEpochMosaicWidget(m_currentCondition, this);
		connect(m_mosaicWidget, &AwEpochMosaicWidget::itemClicked, this, &AwEpochVisuWidget::selectEpoch);
	}
	else
		m_mosaicWidget->setCondition(m_currentCondition);
	thumbList *thumbs;
	if (!m_hashThumbs.contains(m_currentCondition->name())) {
		AwEpochLoader loader("Building thumbnails...", m_currentCondition);
		loader.initProgress(0, m_currentCondition->epochs().size());
		loader.exec();
		m_hashThumbs.insert(m_currentCondition->name(), loader.thumbs());
		thumbs = loader.thumbs();
	}
	else
		thumbs = m_hashThumbs.value(m_currentCondition->name());

	m_mosaicWidget->setThumbs(thumbs);
	m_mosaicWidget->show();
}



void AwEpochVisuWidget::selectEpoch(AwEpochTree *condition, int index)
{
	m_currentCondition = condition;
	m_currentEpochIndex = index;
	selectCurrentEpoch();
}
