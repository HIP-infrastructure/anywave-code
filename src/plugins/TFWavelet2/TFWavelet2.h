#pragma once

#include "tfwavelet2_global.h"
#include <AwProcessInterface.h>
#include "tf_settings.h"
#include "TFWidget.h"


class TFWAVELET2_EXPORT TFWavelet2 : public AwGUIProcess
{
	Q_OBJECT
	Q_INTERFACES(AwGUIProcess)
public:
	TFWavelet2();
	enum Wavelet { Gabor, Morlay};

	void run();	// main execution entry point of the plugin
public slots:
	// overriden setMarkers to handle new markers coming from AnyWave
	void setMarkers(const AwMarkerList& markers) override;
	void addMarker(AwMarker *m) {
		emit sendMarker(m); m_markers.append(m); m_widget->signalView()->setMarkers(m_markers);
	}
protected:
	TFSettings m_settings;
	TFWidget *m_widget;
};


class TFWavelet2Plugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
    Q_INTERFACES(AwProcessPlugin)
public:
	TFWavelet2Plugin();
	TFWavelet2 *newInstance() { return new TFWavelet2(); }
};