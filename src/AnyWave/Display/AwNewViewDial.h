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
#ifndef AWNEWVIEWDIAL_H
#define AWNEWVIEWDIAL_H

#include <QDialog>
#include "ui_AwNewViewDial.h"
#include <QCheckBox>

class AwNewViewDial : public QDialog
{
	Q_OBJECT

public:
	AwNewViewDial(QWidget *parent = 0);
	~AwNewViewDial();
	inline QList<int>& filters() { return m_filters; }
protected:
	void changeEvent(QEvent*);
public slots:
	void accept();
	void all();
	void none();
private:
	Ui::AwNewViewDialUi m_ui;

	QMap<QCheckBox *, int> m_checkBoxes;
	QVector<int> m_types;				// corresponding channel types
	QList<int> m_filters;				// selected filters.
};

#endif // AWNEWVIEWDIAL_H
