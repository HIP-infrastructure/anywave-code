/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
