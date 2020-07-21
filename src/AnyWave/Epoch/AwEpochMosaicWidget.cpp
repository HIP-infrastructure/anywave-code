#include "AwEpochMosaicWidget.h"
#include "AwEpochManager.h"
#include <widget/AwWaitWidget.h>


AwEpochMosaicWidget::AwEpochMosaicWidget(AwEpochTree *condition, AwEpochVisuWidget *parent)
	: QWidget(Q_NULLPTR)
{
	ui.setupUi(this);
	ui.listWidget->setViewMode(QListWidget::IconMode);
	ui.listWidget->setSpacing(5);
	ui.listWidget->setLayoutMode(QListWidget::Batched);
	ui.listWidget->setResizeMode(QListWidget::Adjust);

	m_parent = parent;
	if (condition == Q_NULLPTR)
		condition = AwEpochManager::instance()->conditions().first();
	else 
		m_condition = condition;
	setWindowTitle(QString("Icon view for condition %1").arg(condition->name()));
	ui.listWidget->setIconSize(QSize(200, 150));
	connect(ui.listWidget, &QListWidget::itemClicked, this, &AwEpochMosaicWidget::changeEpoch);
}

AwEpochMosaicWidget::~AwEpochMosaicWidget()
{
}

void AwEpochMosaicWidget::setThumbs(thumbList *thumbs)
{
	int i = 0;
	for (auto t : *thumbs) {
		auto item = new QListWidgetItem(ui.listWidget);
		item->setIcon(QIcon(t->pixmap));
		item->setData(Qt::UserRole, i);
		item->setToolTip(QString("Epoch %1").arg(i));
		i++;
	};
	ui.listWidget->update();
}


void AwEpochMosaicWidget::changeEpoch(QListWidgetItem *item)
{
	emit itemClicked(m_condition, item->data(Qt::UserRole).toInt());
}