#pragma once

#include <QWidget>
#include "ui_MFVGUI.h"
#include <process/AwProcessGUIWidget.h>
#include <widget/SignalView/AwBaseSignalView.h>

class MFVGUI : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	MFVGUI(QWidget *parent = Q_NULLPTR);
	~MFVGUI();

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
