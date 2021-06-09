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
#ifndef AW_MESSAGEBOX_H
#define AW_MESSAGEBOX_H
#include <QMessageBox>
#include <AwGlobal.h>

class  AW_WIDGETS_EXPORT AwMessageBox {
public:
	static int information(QWidget *parent, const QString& title, const QString& text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,	QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static int critical(QWidget *parent, const QString& title, const QString& text,
		QMessageBox::StandardButtons buttons = QMessageBox::Ok,	QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
	static int question(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons = QMessageBox::Yes|QMessageBox::No, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton);
};

#endif