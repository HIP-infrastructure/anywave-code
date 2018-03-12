#include "TFWavelet2.h"
#include "settingsui.h"

TFWavelet2Plugin::TFWavelet2Plugin()
{
	// install local translator, if any translation file is present.
	//	QString locale = QLocale::system().name();
	//	QString fullPath = ":/tfwavelet_" + locale;

	//	QTranslator *translator = new QTranslator;
	//	if (translator->load(fullPath))
	//		qApp->installTranslator(translator);
	name = QString(tr("Time Frequency using Wavelets"));
	description = QString(tr("Display T/F of selected channels."));
	type = AwProcessPlugin::GUI;
}

TFWavelet2::TFWavelet2()
{
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, QString("0-n"));
	// Limit the usage to 1 or 2 channels
	pdi.addInputParameter(Aw::ProcessInput::AnyChannels, QString("1-2"));
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::ProcessRequiresChannelSelection);
//	m_settings = new TFSettings;
//	m_signalView = new AwBaseSignalView(AwBaseSignalView::NoMarkerBar | AwBaseSignalView::EnableMarking | AwBaseSignalView::ViewAllChannels);
//	m_MEGMapReady = m_EEGMapReady = false;
//	// we have localization support, so provide the prefix for all the language files.
//	m_langFilePrefix = ":/tfwavelet";

}


void TFWavelet2::run()
{
	// open settings ui at first
	// init freq max to be sampling rate / 2
	m_settings.freq_max = pdi.input.channels.at(0)->samplingRate() / 2;
	SettingsUi dlg(&m_settings);
	
	if (dlg.exec() != QDialog::Accepted)
		return;

	m_widget = new TFWidget(&m_settings);
	// register our widget to auto close the plugin when the user closes the widget
	registerGUIWidget(m_widget);
	// connect the signal view client to the data server.
	connectClient(m_widget->signalView()->client());
	

	// we want to browse data through all the file
	m_widget->signalView()->setTotalDuration(pdi.input.fileDuration);
	m_widget->signalView()->setMarkers(m_markers);
	// set the channel to be displayed => compute the TF on those channels.
	m_widget->setChannels(pdi.input.channels);
	connect(m_widget->signalView()->scene(), SIGNAL(markerInserted(AwMarker *)), this, SLOT(addMarker(AwMarker *)));

	m_widget->show();
}



////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS
void TFWavelet2::setMarkers(const AwMarkerList& markers)
{
	AwGUIProcess::setMarkers(markers);
}