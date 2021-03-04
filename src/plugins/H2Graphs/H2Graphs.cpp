#include "H2Graphs.h"
#include <AwKeys.h>

H2Graphs *H2Graphs::m_instance = Q_NULLPTR;

H2GraphsPlugin::H2GraphsPlugin()
{
	name = "Correlation Graphs";
	description = "Visualise h2/r2 connectivity graph.";
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

	for (auto f : args) 
		m_widget->addGraphSet(f);
	m_widget->show();
}
