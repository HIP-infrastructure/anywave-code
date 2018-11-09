#pragma once

#include <QWidget>
#include "ui_AwEpochMosaicWidget.h"
#include "AwEpochVisuWidget.h"

// use a AwEpochSignalView object to generate thumbnails.
#include <epoch/AwEpochSignalView.h>

class AwEpochMosaicWidget : public QWidget
{
	Q_OBJECT

public:
	AwEpochMosaicWidget(AwEpochVisuWidget *parent);
	~AwEpochMosaicWidget();
public slots:

private:
	Ui::AwEpochMosaicWidget ui;
	AwEpochVisuWidget *m_parent;
	AwEpochSignalView *m_signalView;
};
