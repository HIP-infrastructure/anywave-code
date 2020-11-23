#include "Spectral.h"
#include "SIWidget.h"
#include <QMessageBox>
#include <AwKeys.h>

SpectralPlugin::SpectralPlugin()
{
	name = QString(tr("Signal Spectral Information"));
	description = QString(tr("Show spectral informations."));
	type = AwProcessPlugin::GUI;
	setFlags(Aw::ProcessFlags::PluginAcceptsTimeSelections);
}

Spectral::Spectral()
{
	//pdi.setInputFlags(Aw::ProcessInput::ProcessRequiresChannelSelection);
	// Limit the usage to 3 channels max
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 0, 0);
	m_widget = nullptr;
}

Spectral::~Spectral()
{
	if (m_widget)
		delete m_widget;
}

void Spectral::run(const QStringList& args)
{
	m_widget = new SIWidget(this);
	// register our widget to auto close the plugin when the user closes the widget
	registerGUIWidget(m_widget);
	// check input channel
	auto channels = pdi.input.channels();
	if (channels.isEmpty()) {
		auto answer = QMessageBox::information(nullptr, "Power Spectral Density", "No channels selected, run for all channels?",
			QMessageBox::Yes | QMessageBox::No);
		if (answer == QMessageBox::Yes) {
			QVariantMap cfg;
			AwChannelList result;
			cfg.insert(keys::channels_source, keys::channels_source_raw);
			selectChannels(cfg, &result);
			if (result.isEmpty())
				return;
			pdi.input.setNewChannels(result);
		}
		else
			return; 
	}
	
	requestData(&pdi.input.channels(), 0., -1, true);
	m_widget->compute();
	m_widget->show();
}