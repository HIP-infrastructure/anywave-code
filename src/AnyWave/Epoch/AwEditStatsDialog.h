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
#include "ui_AwEditStatsDialog.h"
//#include "AwStatsSettings.h"
#include <epoch/AwEpochAverageChannel.h>
class AwEpochTree;

class AwEditStatsDialog : public QDialog
{
	Q_OBJECT

public:
//	AwEditStatsDialog(AwStatsSettings *settings, QWidget *parent = Q_NULLPTR);
	AwEditStatsDialog(const QList<AwEpochAverageChannel *>& channels, AwEpochTree *condition, QWidget *parent = Q_NULLPTR);
	~AwEditStatsDialog();

	QList<AwEpochAverageChannel *> selectedChannels;
	bool showErrorType;
public slots:
	void removeSelectedRows();
	void addRow();
	void accept();
	void reject();
private:
	Ui::AwEditStatsDialogUi m_ui;
	QList<AwEpochAverageChannel *> m_channels;
	AwEpochTree *m_condition;
	QStringList m_statChannelLabels, m_allLabels;
//	AwStatsSettings *m_settings;
};
