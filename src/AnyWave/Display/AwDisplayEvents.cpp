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
#include "AwDisplay.h"
#include "AwDisplaySetup.h"
#include "AwViewSetup.h"

// this source file only contains code to handle events

void AwDisplay::processEvent(QSharedPointer<AwEvent> e)
{
	auto data = e->data();
	// pass event to the views
	for (AwSignalView* v : m_signalViews)
		v->processEvent(e);

	// handle event that relies on display manager itself
	switch (e->id())
	{
	case AwEvent::AddNewView:
	{
		QStringList filters = data.value("filters").toStringList();
		AwViewSetup* ns = m_setup->newViewSetup();
		if (filters.size()) {
			ns->filters.clear();
			for (const QString& f : filters) 
				ns->filters.append(AwChannel::stringToType(f));
		}
		addSignalView(ns);
	}
	break;
	}
}