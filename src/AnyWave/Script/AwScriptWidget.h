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
#ifndef AWSCRIPTWIDGET_H
#define AWSCRIPTWIDGET_H

#include <QWidget>
namespace Ui {class AwScriptWidgetUi;};
#include "AwScript.h"
#include "Process/AwProcessLogWindow.h"
#include <QTimer>

class AwScriptWidget : public QWidget
{
	Q_OBJECT

public:
	AwScriptWidget(AwScript *script, QWidget *parent = 0);
	~AwScriptWidget();

	inline bool isActive() { return m_isActive; }
public slots:
	void setRunning();
	void setFinished();
	void setMessage(const QString& message);
	void stop();
	void showLog();
	void removeMe();
protected slots:
	void makeShowLogBlink();
signals:
	void removed();
	void removeClicked(AwScriptWidget *widget);
private:
	void addLog(const QString& message);

	Ui::AwScriptWidgetUi *ui;
	AwProcessLogWindow *m_logWindow;
	bool m_logUpdated;
	bool m_isActive;
	AwScript *m_script;
	QTimer *m_timer;
};

#endif // AWSCRIPTWIDGET_H
