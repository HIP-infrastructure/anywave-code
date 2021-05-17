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
#ifndef AWPROCESSWIDGET_H
#define AWPROCESSWIDGET_H

#include <QWidget>
#include "ui_AwProcessWidget.h"
#include <AwProcessInterface.h>
#include "AwProcessLogWindow.h"
#include <QTimer>

class AwProcessWidget : public QWidget
{
	Q_OBJECT

public:
	AwProcessWidget(AwProcess *process, QWidget *parent = 0);
	~AwProcessWidget();
	inline bool isActive() { return m_isActive; }
	void clear();
protected:
	void changeEvent(QEvent *);
public slots:
	void setIdle();
	void setRunning();
	void setFinished();
	void setProgression(int value);
	void stop();
	void showResults();
	void showLog();
	void removeMe();
protected slots:
	void makeShowLogBlink();
	void updateNewLog();
signals:
	void removed();
	void removeClicked(AwProcessWidget *widget);
private:
	Ui::AwProcessWidgetUi m_ui;
	bool m_logUpdated;
	bool m_isActive;
	AwProcessLogWindow *m_logWindow;
	QTimer *m_timer;
	AwProcess *m_process;
};

#endif // AWPROCESSWIDGET_H
