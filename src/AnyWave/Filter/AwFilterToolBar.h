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
#ifndef AWFILTERTOOLBAR_H
#define AWFILTERTOOLBAR_H

#include <QWidget>
#include <QToolBar>
#include "ui_AwFilterToolBar.h"

class AwFilterToolBar : public QWidget, public Ui::AwFilterToolBarClass
{
	Q_OBJECT

public:
	AwFilterToolBar(QWidget *parent = 0);
	~AwFilterToolBar();

	inline QToolBar *toolBar() { return m_toolBar; }
protected:
	void changeEvent(QEvent *);
signals:
	void filterButtonClicked();
protected:
	QToolBar *m_toolBar;

};

#endif // AWFILTERTOOLBAR_H
