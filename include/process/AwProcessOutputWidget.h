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
#include <AwGlobal.h>
#include <QWidget>
#include <AwProcessInterface.h>

class AW_PROCESS_EXPORT AwProcessOutputWidget : public QWidget
{
	Q_OBJECT
public:
	explicit AwProcessOutputWidget(AwBaseProcess* process = nullptr, QWidget *parent = nullptr);
signals:
	// copy signals that a process can emit
	void log(const QString& log);
	void sendCommand(int command, QVariantList args);
	void sendCommand(const QVariantMap& );
	void sendMarkers(AwMarkerList* markers);
	void sendMarker(AwMarker* marker);
	void dataConnectionRequested(AwDataClient* client);
protected:
	AwBaseProcess* m_process;
};