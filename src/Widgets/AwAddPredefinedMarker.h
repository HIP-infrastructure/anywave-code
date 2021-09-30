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
#ifndef AWADDPREDEFINEDMARKER_H
#define AWADDPREDEFINEDMARKER_H
#include <AwGlobal.h>
#include <QDialog>
#include "ui_AwAddPredefinedMarker.h"
class AwMarker;

class AW_WIDGETS_EXPORT AwAddPredefinedMarker : public QDialog
{
	Q_OBJECT

public:
	AwAddPredefinedMarker(QWidget *parent = 0);
	~AwAddPredefinedMarker();

	AwMarker *marker() { return m_marker; }
protected:
	void changeEvent(QEvent *);
public slots:
	void accept();
private:
	Ui::AwAddPredefinedMarkerUi m_ui;
	AwMarker *m_marker;
};

#endif // AWADDPREDEFINEDMARKER_H
