#pragma once

#include "spectral_global.h"
#include <AwProcessInterface.h>

class SIWidget;
class FFTIterations;

class SPECTRAL_EXPORT Spectral : public AwProcess
{
    Q_OBJECT
    Q_INTERFACES(AwProcess)
public:
    Spectral();
	~Spectral();
	enum Windows { None, Hanning, Hamming };

	bool showUi() override;
	void run() override;
	void runFromCommandLine() override;
protected:
	int initialize();
	void compute();

	SIWidget* m_widget;
	AwMarkerList m_goodMarkers; // markers kept as valid for input data.
	QMap<AwChannel*, FFTIterations*> m_results;
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