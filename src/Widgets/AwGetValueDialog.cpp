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
#include <widget/AwGetValueDialog.h>

#include "ui_AwGetValueDialog.h"

AwGetValueDialog::AwGetValueDialog(const QString& title, QWidget *parent)
	: QDialog(parent)
{
	m_ui = new Ui::AwGetValueDialogUi;
	m_ui->setupUi(this);
	setWindowTitle(title);
}

AwGetValueDialog::~AwGetValueDialog()
{
	delete m_ui;
}


QString AwGetValueDialog::value()
{
	return m_ui->lineEdit->text();
}