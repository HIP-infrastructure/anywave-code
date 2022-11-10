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
	~TFWavelet2();
	enum Wavelet { Gabor, Morlay};

	void run() override;	// main execution entry point of the plugin
public slots:
	void addMarker(AwMarker *m) {
		AwSharedMarkerList markers;
		markers << AwSharedMarker(m);

		emit sendMarkers(&markers); pdi.input.addMarkers(markers); m_widget->signalView()->setMarkers(pdi.input.markers()); m_widget->updateBaselineOptions();
	}
protected:
	TFSettings m_settings;
	TFWidget *m_widget;
};


class TFWAVELET2_EXPORT TFWavelet2Plugin : public AwProcessPlugin
{
	Q_OBJECT
	Q_PLUGIN_METADATA(IID AwProcessPlugin_IID)
    Q_INTERFACES(AwProcessPlugin)
public:
	TFWavelet2Plugin();
	AW_INSTANTIATE_PROCESS(TFWavelet2);
};