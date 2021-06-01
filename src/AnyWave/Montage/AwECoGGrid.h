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

#include <QStringList>



class AwECoGGrid
{
public:
	AwECoGGrid(int rows, int cols, const QStringList& labels);
	enum Orientation { Horizontal, Vertical, Both };

	inline int rows() { return m_rows;  }
	inline int cols() { return m_cols; }
	inline int orientation() { return m_orientation; }
	inline QStringList& labels() { return m_electrodes; }
	void setOrientation(int orientation);
protected:
	int m_rows, m_cols;
	int m_orientation;
	QStringList m_electrodes;
};

using AwECoGGrids = QList<AwECoGGrid *>;