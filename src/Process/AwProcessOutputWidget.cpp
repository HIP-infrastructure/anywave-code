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
#include <process/AwProcessOutputWidget.h>


AwProcessOutputWidget::AwProcessOutputWidget(AwBaseProcess* p, QWidget* w) : QWidget(w)
{
	m_process = p;
	// relay signals to process
	connect(this, SIGNAL(log(const QString&)), p, SIGNAL(log(const QString&)));
	connect(this, SIGNAL(sendCommand(int , QVariantList args)), p, SIGNAL(sendCommand(int, QVariantList args)));
	connect(this, SIGNAL(sendCommand(const QVariantMap&)), p, SIGNAL(sendCommand(const QVariantMap&)));
	connect(this, SIGNAL(sendMarkers(AwMarkerList *)), p, SIGNAL(sendMarkers(AwMarkerList*)));
	connect(this, SIGNAL(sendMarker(AwMarker*)), p, SIGNAL(sendMarker(AwMarker*)));
	connect(this, SIGNAL(dataConnectionRequested(AwDataClient*)), p, SIGNAL(dataConnectionRequested(AwDataClient*)));
}