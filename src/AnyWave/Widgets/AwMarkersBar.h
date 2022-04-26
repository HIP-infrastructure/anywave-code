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
#ifndef AWMARKERSBAR_H
#define AWMARKERSBAR_H

#include <QFrame>
#include "Display/AwDisplaySetup.h"
#include <widget/SignalView/AwBaseMarkerBar.h>
#include <QMenu>

class AwMarkersBar : public AwBaseMarkerBar
{
	Q_OBJECT

public:
	AwMarkersBar(AwDisplayPhysics *phys, QWidget *parent = nullptr) : AwBaseMarkerBar(phys, parent) {}
protected:
	void contextMenuEvent(QContextMenuEvent *e);
signals:
	void markersWindowNeeded();
private slots:
	void launchProcess();
};

#endif // AWMARKERSBAR_H
