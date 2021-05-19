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
#ifndef AWDISPLAYSETUPTOOLBAR_H
#define AWDISPLAYSETUPTOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include "ui_AwDisplaySetupToolBar.h"

class AwDisplaySetupToolBar : public QWidget, public Ui::AwDisplaySetupToolBarUI
{
	Q_OBJECT

public:
	AwDisplaySetupToolBar(QStringList& setups, QWidget *parent = 0);
	~AwDisplaySetupToolBar();

	inline QToolBar *toolBar() { return m_toolBar; }

	void addSetup(const QString& setup);
	void removeSetup(const QString& setup);
protected:
	void changeEvent(QEvent *);
public slots:
	void setCurrentSetup(const QString& name);
	void retranslate();
signals:
	void setupChanged(const QString& name);
	void currentSetupSaveRequest();
	void currentSetupWillBeDeteted();
protected:
	QToolBar *m_toolBar;
	QStringList m_setups;	// name of all setups
};

#endif // AWDISPLAYSETUPTOOLBAR_H
