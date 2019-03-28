/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

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
	void setMessage(const QString& message);
	void setProgression(int value);
	void stop();
	void showResults();
	void showLog();
	void removeMe();
protected slots:
	void makeShowLogBlink();
signals:
	void removed();
	void removeClicked(AwProcessWidget *widget);
private:
	void addLog(const QString& message);

	Ui::AwProcessWidgetUi m_ui;
	bool m_logUpdated;
	bool m_isActive;
	AwProcessLogWindow *m_logWindow;
	QTimer *m_timer;
	AwProcess *m_process;
};

#endif // AWPROCESSWIDGET_H
