#include "RegisterInject.h"
#include "settings.h"
#include <QMessageBox>
#include <utils/json.h>
#include <AwKeys.h>

RegisterInject::RegisterInject()
{
	setInputFlags(Aw::ProcessIO::GetReaderPlugins);
}


RegisterInjectPlugin::RegisterInjectPlugin()
{
	type = AwProcessPlugin::Background;
	name = "RegisterInject";
	version = "1.0.0";
	description = "Temporal register eeg and meg file then inject eeg data into meg file";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData | Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/args.json");
}


bool RegisterInject::showUi()
{
	settings ui;
	bool eeg_found = false, meg_found = false;

	// find at least 4DNi plugin
	for (auto plugin : pdi.input.readers) {
		if (!meg_found) {
			if (plugin->name == "4DNI Reader") {
				meg_found = true;
				break;
			}
		}
	}
	if (!meg_found) {
		QMessageBox::critical(nullptr, tr("Missing reader plugins"), tr("This plugin requires the 4DNI Reader plugin"));
		return false;
	}
	if (ui.exec() == QDialog::Accepted) {
		auto args = pdi.input.settings;
		args["eeg_file"] = ui.eegFile;
		args["meg_file"] = ui.megFile;
		pdi.input.settings = args;
		return true;
	}
	return false;
}

bool RegisterInject::init()
{
	m_temporal = getProcessByName("temporal_registration");
	m_eeginto4d = getProcessByName("eeginto4D");
	if (m_temporal == nullptr) {
		m_errorString = "temporal registration plugin is not present.";
		return false;
	}
	if (m_eeginto4d == nullptr) {
		m_errorString = "Error: EEGinto4D plugin is not present.";
		return false;
	}
	return true;
}

void RegisterInject::run()
{
	AwUniteMaps(m_temporal->pdi.input.settings, this->pdi.input.settings);
	sendMessage("Running temporal registration on files...");
	m_temporal->run();
	sendMessage("Finished.");
}

void RegisterInject::runFromCommandLine()
{
	auto eegFile = pdi.input.settings.value("eeg_file").toString();
	auto megFile = pdi.input.settings.value("meg_file").toString();
	if (eegFile.isEmpty() || megFile.isEmpty()) {
		sendMessage("Error: Please specify eeg_file and meg_file");
		return;
	}
	run();
}