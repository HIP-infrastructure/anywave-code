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

// This plugin is designed to write values in a trigger channel of a particular FileReader

#include <AwProcessInterface.h>
class AwTriggerWriter : public AwProcess
{
	Q_OBJECT
	Q_INTERFACES(AwProcess)

public:
	AwTriggerWriter();
	bool showUi();
	void run();
protected:
	QString m_triggerChannel;
};

class AwTriggerWriterPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_INTERFACES(AwProcessPlugin)
public:
	AwTriggerWriterPlugin();

	AwTriggerWriter *newInstance() { return new AwTriggerWriter(); }
};
