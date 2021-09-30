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
#ifndef AWADDTARGETCHANNELS_H
#define AWADDTARGETCHANNELS_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwAddTargetChannels.h"

class AW_WIDGETS_EXPORT AwAddTargetChannels : public QDialog
{
	Q_OBJECT

public:
	AwAddTargetChannels(const QStringList& labels, QWidget *parent = 0);
	inline QStringList& selectedChannels() { return m_selectedChannels; }
protected:
	void changeEvent(QEvent *);
public slots:
	void accept();
private:
	Ui::AwAddTargetChannelsUI m_ui;
	QStringList m_selectedChannels;
};

#endif // AWADDTARGETCHANNELS_H
