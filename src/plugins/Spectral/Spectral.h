#pragma once

#include "spectral_global.h"
#include <AwProcessInterface.h>
class SIWidget;

class SPECTRAL_EXPORT Spectral : public AwProcess
{
    Q_OBJECT
    Q_INTERFACES(AwProcess)
public:
    Spectral();
	~Spectral();
	enum Windows { None, Hanning, Hamming };

  //  void run(const QStringList& args) override;	// main execution entry point of the plugin
	bool showUi() override;
	void run() override;
	void runFromCommandLine() override;
protected:
	int initialize();

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