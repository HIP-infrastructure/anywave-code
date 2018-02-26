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
#ifndef AW_STATUS_BAR_PROCESS_INDICATOR_WIDGET_H
#define AW_STATUS_BAR_PROCESS_INDICATOR_WIDGET_H

#include <QWidget>
#include "ui_AwStatusBarProcessIndicatorWidget.h"
#include "AwProcessIndicator.h"
#include <QMenu>
using namespace Ui;

class AwStatusBarProcessIndicatorWidget : public AwProcessIndicatorWidget, public AwStatusBarProcessIndicatorWidgetClass
{
	Q_OBJECT

public:
	AwStatusBarProcessIndicatorWidget(AwProcess *process, QMenu *menu = NULL, QWidget *parent = 0);
	~AwStatusBarProcessIndicatorWidget() {}

public slots:
	void setIdle();
	void setRunning();
	void setFinished();
	void setMessage(const QString& message) { m_message = message; updateMessage(); }
	void setProgression(int value);

private slots:
	void stopProcess();

private:
	void updateMessage();

	QString m_processName;
	QTime m_timeStarted;
	QMenu *m_menu;
	QAction *m_actStop;
	QString m_message;
	bool m_isRunning;
	int m_progress;
};

class AwStatusBarProcessIndicator : public AwProcessIndicator
{
	Q_OBJECT
public:
	AwStatusBarProcessIndicator(QObject *parent = 0) : AwProcessIndicator(parent) {}
	AwProcessIndicatorWidget * newWidget(AwProcess *process, QMenu *menu, QWidget *parent);
};
#endif // AW_PROCESS_INDICATOR_WIDGET_H
