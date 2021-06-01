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

#include <QObject>
#include "AwBatchGUI.h"

class AwBatchManager : public QObject
{
	Q_OBJECT

public:
	~AwBatchManager();
	// utilities static methods
	static AwBatchManager *instance();
	static bool isInstantiated() { return m_instance != nullptr; }
	// GUI Specific
	AwBatchGUI *ui() { return m_ui; }
protected:
	AwBatchManager();
	AwBatchGUI *m_ui;
	static AwBatchManager *m_instance;
};
