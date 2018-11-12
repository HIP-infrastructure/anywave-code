#pragma once

#include <QWidget>
#include "ui_AwEpochMosaicWidget.h"
#include "AwEpochVisuWidget.h"

// use a AwEpochSignalView object to generate thumbnails.
#include <epoch/AwEpochSignalView.h>
#include <epoch/AwEpochTree.h>

typedef struct {
	AwEpoch *epoch;
	QPixmap pixmap;
} thumb;

typedef QList<thumb *> thumbList;

class AwEpochMosaicWidget : public QWidget
{
	Q_OBJECT

public:
	AwEpochMosaicWidget(AwEpochTree *condition, AwEpochVisuWidget *parent);
	~AwEpochMosaicWidget();
	void setThumbSize(const QSize& size) { m_iconSize = size; ui.listWidget->setIconSize(m_iconSize); }
public slots:
	void changeCondition(const QString& condition);
private:
	Ui::AwEpochMosaicWidget ui;
	AwEpochVisuWidget *m_parent;
	AwEpochSignalView *m_signalView;
	AwEpochTree *m_condition;
	QHash<QString, thumbList *> m_thumbs;
	QSize m_iconSize;
	thumb *newThumb(AwEpoch *epoch);
};
