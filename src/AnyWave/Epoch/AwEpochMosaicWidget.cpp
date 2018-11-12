#include "AwEpochMosaicWidget.h"
#include "AwEpochManager.h"
#include <widget/AwWaitWidget.h>


AwEpochMosaicWidget::AwEpochMosaicWidget(AwEpochTree *condition, AwEpochVisuWidget *parent)
	: QWidget(Q_NULLPTR)
{
	ui.setupUi(this);
//	m_builder = Q_NULLPTR;
	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setSpacing(5);
	m_parent = parent;
	if (condition == Q_NULLPTR)
		condition = AwEpochManager::instance()->conditions().first();
	else 
		m_condition = condition;
	m_signalView = new AwEpochSignalView();
	// use a size of 800x600 pixels
	m_signalView->resize(QSize(800, 600));
	int i = 0;
	for (auto c : AwEpochManager::instance()->conditions()) {
		ui.comboCondition->addItem(c->name());
		if (c == condition)
			ui.comboCondition->setCurrentIndex(i);
		i++;
	}
	connect(ui.comboCondition, &QComboBox::currentTextChanged, this, &AwEpochMosaicWidget::changeCondition);
	m_signalView->setAttribute(Qt::WA_DontShowOnScreen);
	m_signalView->setFlags(AwBaseSignalView::NoNavBar | AwBaseSignalView::NoMarkerBar | AwBaseSignalView::ViewAllChannels);
	m_signalView->show();
	m_iconSize = QSize(200, 150); // default size
	ui.listWidget->setIconSize(m_iconSize);
	ui.progressBar->hide();
	ui.labelProgress->hide();

//	changeCondition(condition->name());
}

AwEpochMosaicWidget::~AwEpochMosaicWidget()
{
	delete m_signalView;
	for (auto thumbs : m_thumbs.values()) {
		while (!thumbs->isEmpty())
			delete thumbs->takeFirst();
		delete thumbs;
	}
}

void AwEpochMosaicWidget::changeCondition(const QString& condition)
{
	// Use EpochManager
	auto cond = AwEpochManager::instance()->getCondition(condition);
	// clear listWidget
	ui.listWidget->clear();
	// generate thumb if necessary
	thumbList *list;
	if (!m_thumbs.contains(condition)) {
		list = new thumbList;
		m_thumbs.insert(condition, list);
	//	AwWaitWidget wait("Building thumbnails...", "");
	//	wait.initProgress(0, cond->epochs().size());
		for (auto e : cond->epochs()) {
			auto t = newThumb(e);
			auto item = new QListWidgetItem(ui.listWidget);
			item->setIcon(QIcon(t->pixmap));
			item->setData(Qt::UserRole, m_condition->epochs().indexOf(t->epoch));
			*list << t;
			ui.listWidget->update();
		//	wait.repaint();
		}
	}
	else {
		list = m_thumbs.value(condition);

		for (auto t : *list) {
			auto item = new QListWidgetItem(ui.listWidget);
			item->setIcon(QIcon(t->pixmap));
			item->setData(Qt::UserRole, m_condition->epochs().indexOf(t->epoch));
		}
		ui.listWidget->update();
	}
}

thumb *AwEpochMosaicWidget::newThumb(AwEpoch *epoch)
{
	auto t = new thumb;
	t->epoch = epoch;
	if (!epoch->isLoaded())
		m_condition->loadEpoch(epoch);
	m_signalView->setEpoch(epoch);
	m_signalView->repaint();
	t->pixmap = QPixmap(QSize(800, 600));
	t->pixmap = m_signalView->grab();
	t->pixmap = t->pixmap.scaled(m_iconSize, Qt::KeepAspectRatio);
	return t;
}