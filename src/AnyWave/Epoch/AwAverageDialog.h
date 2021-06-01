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
#include "ui_AwAverageDialog.h"
#include <epoch/AwEpochTree.h>
#include <epoch/AwEpochComputeSettings.h>
#include <filter/AwFilterSettings.h>

class AwAverageDialog : public QDialog
{
	Q_OBJECT

public:
	AwAverageDialog(QWidget *parent = Q_NULLPTR);
	~AwAverageDialog();

	QList<AwEpochTree *> selectedConditions;
	AwFilterSettings& filterSettings()  { return m_filterSettings; }
	inline bool isRawData() { return m_rawData; }
	inline bool isOfflineFiltering() { return m_offlineFiltering; }
public slots:
	void accept();
	void updateConditionSettings(const QModelIndex& index);
private:
	Ui::AwAverageDialogUi m_ui;
	bool m_rawData;
	bool m_offlineFiltering;
	AwEpochComputeSettings m_settings;
	AwFilterSettings m_filterSettings;
};
