#include "Spectral.h"


SpectralPlugin::SpectralPlugin()
{
	name = QString(tr("Signal Spectral Information"));
	description = QString(tr("Show spectral informations."));
	type = AwProcessPlugin::GUI;
	setFlags(Aw::ProcessFlags::PluginAcceptsTimeSelections);
}

Spectral::Spectral()
{
	pdi.setInputFlags(Aw::ProcessInput::ProcessRequiresChannelSelection);
	// Limit the usage to 3 channels max
	//pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 3);
}