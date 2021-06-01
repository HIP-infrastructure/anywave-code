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
#ifndef AWVIEWSETUP_H
#define AWVIEWSETUP_H
class AwDisplaySetup;
#include <widget/SignalView/AwViewSettings.h>
#include <QObject>

class AwViewSetup : public AwViewSettings
{
	Q_OBJECT
public:
	AwViewSetup(QObject *parent);
	AwViewSetup(AwViewSetup *source, QObject *parent);
	inline AwDisplaySetup *parent() { return m_parent; }
protected:
	AwDisplaySetup *m_parent;
};

#endif