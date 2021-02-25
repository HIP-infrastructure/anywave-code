#include "H2Graphs.h"
#include <AwKeys.h>

H2Graphs *H2Graphs::m_instance = Q_NULLPTR;

H2GraphsPlugin::H2GraphsPlugin()
{
	name = "Correlation Graphs";
	description = "Show correlation graphs.";
	category = "Process:Correlation:Correlation Graphs";
	type = AwProcessPlugin::GUI;
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
