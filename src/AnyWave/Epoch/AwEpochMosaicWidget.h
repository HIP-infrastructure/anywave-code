#pragma once

#include <QWidget>
#include "ui_AwEpochMosaicWidget.h"
#include "AwEpochVisuWidget.h"
#include <epoch/AwEpochTree.h>

#include "AwEpochThumbs.h"

class AwEpochMosaicWidget : public QWidget
{
	Q_OBJECT

public:
	AwEpochMosaicWidget(AwEpochTree *condition, AwEpochVisuWidget *parent);
	~AwEpochMosaicWidget();
	void setThumbs(thumbList *thumbs);
	void setCondition(AwEpochTree *condition) { m_condition = condition; }
public slots:
	void changeEpoch(QListWidgetItem *item);
signals:
	void itemClicked(AwEpochTree *condition, int index);
private:
	Ui::AwEpochMosaicWidget ui;
	AwEpochVisuWidget *m_parent;
	AwEpochTree *m_condition;
};
