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
#include <qtableview.h>
#include <AwMarker.h>
#include <AwGlobal.h>

class AW_WIDGETS_EXPORT AwInputMarkerTableView : public QTableView
{
	Q_OBJECT
public:
	AwInputMarkerTableView(QWidget *parent = Q_NULLPTR);
	void setFlags(int flags);

	enum Flags { MultiSelection = 1, ShowLabel = 2, ShowDuration = 4, ShowPosition = 8, 
	ShowColor = 16, ShowTargets = 32, ShowValue = 64, ShowAll = 126};
public slots:
	void setMarkers(const AwMarkerList& markers);
protected:
	int m_flags;
};