#pragma once

#include <QWidget>
#include "ui_SIWidget.h"
//#include <widget/SignalView/AwBaseSignalView.h>
#include <process/AwProcessGUIWidget.h>
//#define QCUSTOMPLOT_USE_LIBRARY
//#include "qcustomplot.h"

class SIWidget : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	SIWidget(AwGUIProcess *process, QWidget *parent = nullptr);
	~SIWidget();

//	void setChannels(const AwChannelList& channels);
//	AwBaseSignalView *signalView() { return m_signalView; }
public slots:
	void compute();
private:
	Ui::SIWidgetUi  m_ui;
//	AwBaseSignalView *m_signalView;
	int m_layoutRow;
	//QList<QCustomPlot *> m_plots;
};
