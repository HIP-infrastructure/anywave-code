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
#pragma once

#include <QDialog>
#include "ui_AwAddEditBatchDialog.h"
class AwBatchItem;

class AwAddEditBatchDialog : public QDialog
{
	Q_OBJECT

public:
	AwAddEditBatchDialog(AwBatchItem *item, QWidget *parent = Q_NULLPTR);
	~AwAddEditBatchDialog();
public slots:
	void accept() override;
	int exec() override;
private slots:
	void saveProfile();
	void applyProfile();
	void browseInputDir();
	void setFileProperty();
private:
	Ui::AwAddEditBatchDialogUi m_ui;
	AwBatchItem *m_item, *m_itemCopy;
	QMap<QString, QWidget *> m_widgets;	// store widgets based on keys defined in json ui
	QString m_errorString, m_homeDir;

	void setupParamsUi();
	void fetchParams();
	void setParams();
	void fetchFiles();
};
