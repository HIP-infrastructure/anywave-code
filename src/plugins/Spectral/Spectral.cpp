#include "Spectral.h"
#include "SIWidget.h"
#include <QMessageBox>
#include <AwKeys.h>
#include "InputMarkersDial.h"

SpectralPlugin::SpectralPlugin()
{
	name = QString(tr("Spectral Informations"));
	category = "Process:Signal:Power Spectral Density";
	description = QString(tr("Compute and show spectral informations."));
	type = AwProcessPlugin::Background;
	
}

Spectral::Spectral()
{
	pdi.addInputChannel(AwProcessDataInterface::AnyChannels, 0, 0);
	m_widget = nullptr;
	setInputFlags(Aw::ProcessInput::GetDurationMarkers);
}

Spectral::~Spectral()
{
	if (m_widget)
		delete m_widget;
}

int Spectral::initialize()
{
	clean();

}

bool Spectral::showUi()
{
	auto fd = pdi.input.settings.value(keys::file_duration).toFloat();
	// check for input markers (only duration markers is assumed here)
	if (!pdi.input.markers().isEmpty()) {
		InputMarkersDial dlg(pdi.input.markers());
		if (dlg.exec() == QDialog::Accepted) {
			if (!dlg.m_skippedLabels.isEmpty() || !dlg.m_usedLabels.isEmpty()) {
				auto markers = AwMarker::duplicate(pdi.input.markers());
				auto inputMarkers = AwMarker::getInputMarkers(markers, dlg.m_skippedLabels, dlg.m_usedLabels, fd);
				if (inputMarkers.isEmpty()) {
					pdi.input.clearMarkers();
					pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
				}
				else {
					pdi.input.clearMarkers();
					pdi.input.setNewMarkers(inputMarkers);
				}
			}
			else {
				if (QMessageBox::question(nullptr, "Data Length", "Compute on whole data?", QMessageBox::Yes | QMessageBox::No) ==
					QMessageBox::No)
					return;
				pdi.input.clearMarkers();
				pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
			}
		}
		else 
			return false;
	}
	else {
		pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
	}
	//m_widget = new SIWidget(this);
	SIWidget dlg;
	if (dlg.exec() != QDialog::Accepted)
		return false;
	pdi.input.settings["time_window"] = dlg.timeWindow;
	pdi.input.settings["overlap"] = dlg.overlap;
	pdi.input.settings["window"] = dlg.windowType;
	return true;
}

void Spectral::run(const QStringList& args)
{
	auto fd = pdi.input.settings.value(keys::file_duration).toFloat();
	// check for input markers (only duration markers is assumed here)
	if (!pdi.input.markers().isEmpty()) {
		InputMarkersDial dlg(pdi.input.markers());
		if (dlg.exec() == QDialog::Accepted) {
			if (!dlg.m_skippedLabels.isEmpty() || !dlg.m_usedLabels.isEmpty()) {
				auto markers = AwMarker::duplicate(pdi.input.markers());
				auto inputMarkers = AwMarker::getInputMarkers(markers, dlg.m_skippedLabels, dlg.m_usedLabels, fd);
				if (inputMarkers.isEmpty()) {
					pdi.input.clearMarkers();
					pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
				}
				else {
					pdi.input.clearMarkers();
					pdi.input.setNewMarkers(inputMarkers);
				}
			}
			else {
				if (QMessageBox::question(nullptr, "Data Length", "Compute on whole data?", QMessageBox::Yes | QMessageBox::No) ==
					QMessageBox::No)
					return;
				pdi.input.clearMarkers();
				pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
			}
		}
		else return;
	}
	else {
		pdi.input.addMarker(new AwMarker("whole_data", 0., fd));
	}
	m_widget = new SIWidget(this);
	// register our widget to auto close the plugin when the user closes the widget
//	registerGUIWidget(m_widget);
	//// check input channel
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
	m_widget->show();
}