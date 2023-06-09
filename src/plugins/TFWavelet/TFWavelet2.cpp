#include "TFWavelet2.h"
#include "settingsui.h"
#include <AwKeys.h>

TFWavelet2Plugin::TFWavelet2Plugin()
{
	name = QString(tr("Time Frequency"));
	version = "2.0.0";
	description = "Compute Time/Frequency using wavelet";
	category = "Process:Signal:Time Frequency";
	type = AwProcessPlugin::GUI;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

TFWavelet2::TFWavelet2()
{
	setInputFlags(Aw::ProcessIO::GetAllMarkers);
	setInputModifiers(Aw::ProcessIO::modifiers::RequireChannelSelection);
	// Limit the number of channels
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 1, 5);
}

TFWavelet2::~TFWavelet2()
{
	delete m_widget;
}


void TFWavelet2::run()
{
	// open settings ui at first
	// init freq max to be sampling rate / 2
	m_settings.freq_max = pdi.input.channels().at(0)->samplingRate() / 2;
	SettingsUi dlg(&m_settings);
	
	if (dlg.exec() != QDialog::Accepted)
		return;

	m_widget = new TFWidget(&m_settings, this);
	// register our widget to auto close the plugin when the user closes the widget
	registerGUIWidget(m_widget);
	// connect the signal view client to the data server.
	connectClient(m_widget->signalView()->client());
	
	// we want to browse data through all the file
	m_widget->signalView()->setTotalDuration(pdi.input.settings[keys::file_duration].toDouble());
	m_widget->signalView()->setMarkers(pdi.input.markers());
	// set the channels to be displayed => compute the TF on those channels.
//	m_widget->setChannels(pdi.input.channels());
	m_widget->setChannels(pdi.input.sharedChannels()); 
	connect(m_widget->signalView()->scene(), SIGNAL(markerInserted(AwMarker *)), this, SLOT(addMarker(AwMarker *)));

	m_widget->show();
}