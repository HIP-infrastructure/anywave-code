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
#ifndef AW_PROCESS_GUI_WIDGET_H
#define AW_PROCESS_GUI_WIDGET_H
#include <AwProcess.h>
#include <AwGlobal.h>
#include <QWidget>

class AwGUIProcess;

class AW_PROCESS_EXPORT AwProcessGUIWidget : public QWidget
{
	Q_OBJECT
public:
	AwProcessGUIWidget(AwGUIProcess *process, QWidget *parent) : QWidget(parent) { m_process = process; }
signals:
	void closed();
protected:
	/** reimplementing close event to send the closed signal to AnyWave **/
	void closeEvent(QCloseEvent *event);
	AwGUIProcess *m_process;
};

#endif