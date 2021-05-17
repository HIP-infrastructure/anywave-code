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
#ifndef AWDOCKADDMARKER_H
#define AWDOCKADDMARKER_H

#include <QDockWidget>
class AwMarkerInspector;
class AwDockAddMarker : public QDockWidget
{
	Q_OBJECT

public:
	AwDockAddMarker(const QString& title, QWidget *parent = 0, Qt::WindowFlags flags = 0);
	~AwDockAddMarker();

	inline AwMarkerInspector *widget() { return m_inspector; }
public slots:
	void setVisible(bool visible);
signals:
	void addingMarkerStopped();
private:
	AwMarkerInspector *m_inspector;
};

#endif // AWDOCKADDMARKER_H
