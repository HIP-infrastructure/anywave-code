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

#include <AwProcessInterface.h>
#include <QtCore>

class AwTriggerParser : public AwProcess
{
	Q_OBJECT
	Q_INTERFACES(AwProcess)
public:
	AwTriggerParser();
	~AwTriggerParser();
	void detect();
	void run() override;
	void runFromCommandLine() override;
	bool showUi();
private:
	AwChannelList m_triggers;
	int m_maskValue;
	bool m_parseNegative;
	AwSharedMarkerList m_markers;

	void clearMarkers();	// clean markers object when the process has finished or before starting it.
};


class AwTriggerParserPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwTriggerParserPlugin();
	virtual ~AwTriggerParserPlugin() {}
	AwTriggerParser *newInstance() { return new AwTriggerParser; }
};