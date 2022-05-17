#include "RegisterInject.h"
#include "settings.h"
#include <QMessageBox>
#include <utils/json.h>
#include <AwKeys.h>
#include <QFileInfo>
#include <QDir>
#include <QRegularExpression>

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
	category = "Process:File Operation:Register EEG and Inject into MEG";
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
	// Check  if we have output_dir specified. Otherwise, set output_dir to eeg file data dir
	if (!pdi.input.settings.contains(keys::output_dir)) {
		QFileInfo fi(pdi.input.settings.value("eeg_file").toString());
		pdi.input.settings[keys::output_dir] = fi.absolutePath();
	}
	QString outputDir = pdi.input.settings.value(keys::output_dir).toString();
	sendMessage("Running temporal registration...");
	connect(m_temporal, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
	m_temporal->run();
	sendMessage("Finished.");
	// modify eeg_file and output dir for injection
	QFileInfo fi(pdi.input.settings.value("eeg_file").toString());
	QString baseAdesFile = fi.fileName().remove(QRegularExpression(".[0-9a-z]+$"));
	// search in output_dir the resulting .ades file (must start with baseAdesFile name and finished with .ades)
	QDir dir(outputDir);
	const auto files = dir.entryList({"*.ades"}, QDir::Files);
	QString outputAdes;
	for (const auto& file : files) {
		if (file.startsWith(baseAdesFile) && file.endsWith(".ades")) {
			outputAdes = QString("%1/%2").arg(outputDir).arg(file);
			break;
		}
	}
	if (outputAdes.isEmpty()) {
		sendMessage(QString("Error: no output result file found in %1 after temporal registration has run").arg(outputDir));
		return;
	}
	pdi.input.settings.remove(keys::output_dir);
	pdi.input.settings["eeg_file"] = outputAdes;
	sendMessage("Running eeginto4D...");
	AwUniteMaps(m_eeginto4d->pdi.input.settings, this->pdi.input.settings);
	connect(m_eeginto4d, SIGNAL(progressChanged(const QString&)), this, SIGNAL(progressChanged(const QString&)));
	m_eeginto4d->runFromCommandLine();
	sendMessage("Finished");
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