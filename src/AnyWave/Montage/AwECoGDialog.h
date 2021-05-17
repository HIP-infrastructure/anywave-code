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
#include "ui_AwECoGDialog.h"
#include <AwChannel.h>

class AwECoGDialog : public QDialog
{
	Q_OBJECT

public:
	AwECoGDialog(const AwChannelList& channels, QWidget *parent = Q_NULLPTR);
	~AwECoGDialog();

	AwChannelList& channels() { return m_channels; }
public slots:
	void accept() override;
	void addGridStrip();
	void reset();
protected slots:
	void addGrid();
private:
	Ui::AwECoGDialogUi ui;
	AwChannelList m_channels;
	QMap<QString, AwChannel *> m_channelsMap;
};
