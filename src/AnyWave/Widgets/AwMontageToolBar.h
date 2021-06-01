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
#ifndef AWMONTAGETOOLBAR_H
#define AWMONTAGETOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include <QMenu>
#include "ui_AwMontageToolBar.h"
#include "Montage/AwMontageManager.h"

class AwMontageToolBar : public QWidget
{
	Q_OBJECT

public:
	AwMontageToolBar(QWidget *parent = 0, bool withToolBar = false);
	~AwMontageToolBar();
	void setLabelInfo(const QString& label);
	QToolBar *toolBar() { return m_toolBar; }
protected:
	void changeEvent(QEvent *);
public slots:
	void updateQuickMontages();
	void loadQuickMontage();
protected:
	AwMontageManager *m_mm;
	QToolBar *m_toolBar;
	QMenu m_menu;
private:
	Ui::AwMontageToolBarUi ui;
};

#endif // AWMONTAGETOOLBAR_H
