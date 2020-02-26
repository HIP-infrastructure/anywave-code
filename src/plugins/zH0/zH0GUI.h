#pragma once

#include <QWidget>
#include "ui_zH0GUI.h"
#include <process/AwProcessGUIWidget.h>
#include <widget/SignalView/AwBaseSignalView.h>

class zH0GUI : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	zH0GUI(QWidget *parent = Q_NULLPTR);
	~zH0GUI();

	AwBaseSignalView *signalView() { return m_signalView; }
	void setChannels(const AwChannelList& channels);
public slots:
	void add();
	void clear();
	void apply();
private:
	Ui::zH0GUI m_ui;
	AwBaseSignalView *m_signalView;
	AwChannelList m_channels, m_currentChannels;
};
