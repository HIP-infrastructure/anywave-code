#include "AwEpochMosaicWidget.h"


AwEpochMosaicWidget::AwEpochMosaicWidget(AwEpochVisuWidget *parent)
	: QWidget(Q_NULLPTR)
{
	ui.setupUi(this);
	m_parent = parent;
	m_signalView = new AwEpochSignalView();
	// use a size of 800x600 pixels
	m_signalView->resize(QSize(800, 600));
}

AwEpochMosaicWidget::~AwEpochMosaicWidget()
{
}
