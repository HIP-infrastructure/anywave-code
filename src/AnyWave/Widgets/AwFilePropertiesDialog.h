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
#ifndef AWFILEPROPERTIESDIALOG_H
#define AWFILEPROPERTIESDIALOG_H

#include <QDialog>
#include "ui_AwFilePropertiesDialog.h"

class AwFileIO;
class AwFilePropertiesModel;

class AwFilePropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	AwFilePropertiesDialog(AwFileIO *reader, QWidget *parent = 0);
	~AwFilePropertiesDialog();
protected:
	void changeEvent(QEvent *);
public slots:
	void exportLabels();
private:
	Ui::AwFilePropertiesDialogUi m_ui;
	AwFilePropertiesModel *m_model;
	AwFileIO* m_reader;
};

#endif // AWFILEPROPERTIESDIALOG_H
