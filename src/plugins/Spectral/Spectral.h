#pragma once

#include "spectral_global.h"
#include <AwProcessInterface.h>
class SIWidget;

class SPECTRAL_EXPORT Spectral : public AwGUIProcess
{
    Q_OBJECT
    Q_INTERFACES(AwGUIProcess)
public:
    Spectral();
	~Spectral();

    void run(const QStringList& args) override;	// main execution entry point of the plugin
protected:
	SIWidget* m_widget;
};


class SPECTRAL_EXPORT SpectralPlugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
	Q_INTERFACES(AwProcessPlugin)
public:
	SpectralPlugin();
	AW_INSTANTIATE_PROCESS(Spectral)
};