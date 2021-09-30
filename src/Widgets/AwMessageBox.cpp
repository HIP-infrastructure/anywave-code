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
#include <widget/AwMessageBox.h>
#include <QIcon>
#define icon_path ":/images/AnyWave_icon.png"

int AwMessageBox::information(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons,	QMessageBox::StandardButton defaultButton)
{
	QMessageBox box(QMessageBox::NoIcon, title, text, buttons, parent);
	box.setIconPixmap(QPixmap::fromImage(QImage(icon_path)));
	box.setDefaultButton(defaultButton);
	box.setWindowIcon(QIcon(icon_path));
	return box.exec();
}
int AwMessageBox::critical(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons,	QMessageBox::StandardButton defaultButton)
{
	QMessageBox box(QMessageBox::NoIcon, title, text, buttons, parent);
	box.setIconPixmap(QPixmap::fromImage(QImage(icon_path)));
	box.setDefaultButton(defaultButton);
	box.setWindowIcon(QIcon(icon_path));
	return box.exec();
}
int AwMessageBox::question(QWidget *parent, const QString& title, const QString& text, 
		QMessageBox::StandardButtons buttons, QMessageBox::StandardButton defaultButton)
{
	QMessageBox box(QMessageBox::NoIcon, title, text, buttons, parent);
	box.setIconPixmap(QPixmap::fromImage(QImage(icon_path)));
	box.setDefaultButton(defaultButton);
	box.setWindowIcon(QIcon(icon_path));
	return box.exec();
}