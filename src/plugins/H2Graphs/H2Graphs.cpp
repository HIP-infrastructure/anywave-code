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
#include "H2Graphs.h"
#include <AwKeys.h>

H2Graphs *H2Graphs::m_instance = Q_NULLPTR;

H2GraphsPlugin::H2GraphsPlugin()
{
	name = "Correlation Graphs";
	version = "1.1.0";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData);
	description = "Visualise h2/r2 connectivity graphs.";
	category = "Process:Correlation:h2/r2 Connectiviy Graphs";
	type = AwProcessPlugin::GUI;
	m_helpUrl = "h2/r2 Graphs::Correlation::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_h2Graphs";
}

H2Graphs::H2Graphs()
{
	m_widget = nullptr;
	m_instance = this;
}

H2Graphs *H2Graphs::instance()
{
	return m_instance;
}

H2Graphs::~H2Graphs()
{
	if (m_widget)
		delete m_widget;
}

void H2Graphs::run()
{
	auto args = pdi.input.settings.value(keys::args).toStringList();

	m_widget = new GraphManagerWidget(this);
	// register our widget to auto close the plugin when the user closes the widget
	registerGUIWidget(m_widget);
	bool ok = true;
	for (auto f : args) {
		if (m_widget->addGraphSet(f) != 0) {
			ok = false;
			break;
		}
	}
	if (ok)
		m_widget->show();
	else {

	}
}
