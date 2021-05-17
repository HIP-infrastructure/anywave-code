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
#pragma once

#include <QWidget>
#include "ui_MFVGUI.h"
#include <process/AwProcessGUIWidget.h>
#include <widget/SignalView/AwBaseSignalView.h>

class MFVGUI : public AwProcessGUIWidget
{
	Q_OBJECT

public:
	MFVGUI(AwGUIProcess *process, QWidget *parent = Q_NULLPTR);
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
