// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "AwExporter.h"
#include <filter/AwFiltering.h>
#include <widget/AwMessageBox.h>
#include <AwCore.h>
#include <AwKeys.h>
#include <utils/json.h>
#include "AwExportWizardDial.h"
#include "Marker/AwMarkerManager.h"
#include <QRegularExpression>
#include <QDir>
#include <utils/bids.h>

namespace Exporter {
	constexpr auto decimate_factor = "decimate_factor";
};


AwExporterPlugin::AwExporterPlugin()
{
	name = QString("Export");
	description = QString("Export data to a file");
	category = "Process:File Operation:Export To File";
	version = "1.1.1";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
	m_settings[keys::json_batch] = AwUtilities::json::fromJsonFileToString(":/json/file_exporter.json");
	m_helpUrl = "Save to file::File Operations::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_exporter";
}

AwExporter::AwExporter() : AwProcess()
{
	setInputFlags(Aw::ProcessIO::GetAllMarkers | Aw::ProcessIO::GetWriterPlugins | Aw::ProcessIO::GetReaderPlugins
		| Aw::ProcessIO::GetCurrentMontage);
	setInputModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
}

AwExporter::~AwExporter() 
{
}

void AwExporter::runFromCommandLine()
{
	auto args = pdi.input.settings;
	// check for required options
	if (!args.contains(keys::output_file)) {
		sendMessage("output_file option is missing.");
		return;
	}
	if (!args.contains("output_writer"))  // default to ADES
		args["output_writer"] = QString("anywave ades format");

	auto outputFile = args.value(keys::output_file).toString();
	// check for the outputs
	auto writerName = args.value("output_writer").toString().toLower().simplified();
	if (writerName.isEmpty()) {
		sendMessage("No output plugin specified.");
		return;
	}
	int decimateFactor = 1;
	if (args.contains(Exporter::decimate_factor))
		decimateFactor = args.value(Exporter::decimate_factor).toInt();

	QString outputPath = outputFile;
	if (args.contains(keys::output_dir))
		outputPath = QString("%1/%2").arg(args.value(keys::output_dir).toString()).arg(outputFile);

	AwFileIO *writer = nullptr;
	for (auto p : pdi.input.writers) {
		if (p->name.toLower().simplified() == writerName) {
			writer = p->newInstance();
			break;
		}
	 }

	if (writer == nullptr) {
		sendMessage(QString("Plugin %1 not found.").arg(writerName));
		return;
	}

	float endTimePos = args.value(keys::file_duration).toFloat();
	auto markers = pdi.input.markers();
	// remove markers that target channels if the targeted channels were removed while exporting...
	if (markers.size()) {
		AwMarkerList removed;
		QStringList labels = AwChannel::getLabels(pdi.input.channels());
		for (auto marker : markers) {
			QStringList targets = marker->targetChannels();
			for (const auto& t : targets) {
				if (!labels.contains(t))
					removed.append(marker);
			}
		}
		if (removed.size()) {
			for (auto m : removed) {
				markers.removeAll(m);
				delete m;
			}
			pdi.input.setMarkers(markers);
		}
	}
	AwBlock* block = writer->infos.newBlock();
	AwMarker global("global", 0., endTimePos);
	if (isUseSkipMarkersApplied()) {
		auto merged = AwMarker::merge(pdi.input.markers());
		auto modified = pdi.input.modifiedMarkers();
		block->setMarkers(modified);
		pdi.input.setNewMarkers(merged);
		m_inputMarkers = merged;
	}
	else {
		block->setMarkers(pdi.input.markers());
		m_inputMarkers << &global;  // the trick is to avoid memory leak as we don't destroy m_inputMarkers list.
	}

	AwChannelList inputChannels = pdi.input.channels();

	if (decimateFactor > 1) {
		sendMessage("Loading data...");
		//	AwChannel::clearFilters(m_channels);
	   requestData(&inputChannels, &m_inputMarkers, true);
	   sendMessage("Done.");
	   sendMessage("Downsampling...");
	   AwFiltering::downSample(inputChannels, decimateFactor);
		// apply filters set in the UI
		AwFiltering::filter(inputChannels);
		sendMessage("Done.");
	}
	else {
		sendMessage("Loading data...");
		requestData(&inputChannels, &m_inputMarkers);
		sendMessage("Done.");
	}

	// set channels to the writer object AFTER loading and/or not downsampling
	// duplicate input channels before set them to writer as writer object takes ownership of channels and will destroy them 
	writer->infos.setChannels(AwChannel::duplicateChannels(inputChannels));

	block->setSamples(pdi.input.channels().first()->dataSize());
	block->setDuration((float)pdi.input.channels().first()->dataSize() / pdi.input.channels().first()->samplingRate());
	writer->infos.setDate(args.value(keys::date).toString());
	writer->infos.setTime(args.value(keys::time).toString());
	writer->infos.setISODate(args.value(keys::iso_date).toString());

	if (writer->createFile(outputPath) != AwFileIO::NoError) {
		sendMessage(QString("Error creating %1.").arg(outputPath));
		writer->cleanUpAndClose();
		delete writer;
		return;
	}
	sendMessage("Writing data...");
	writer->writeData(&inputChannels);
	sendMessage("Done.");

	bool isBids = pdi.input.settings.contains(keys::bids_file_path);
	if (isBids) {
		auto outputFile = pdi.input.settings.value(keys::output_file).toString();
		auto destFile = outputFile;
		sendMessage("Preparing files for bids...");
		// get modality
		QString modality;
		if (outputFile.contains("ieeg"))
			modality = "_ieeg";
		else if (outputFile.contains("eeg"))
			modality = "_eeg";
		else
			modality = "_meg";

		auto map = AwUtilities::bids::getKeysValue(outputFile);

		destFile = destFile.remove(modality);
		destFile = destFile.remove(".vhdr");
		if (!map.contains("proc"))
			sendMessage("proc key was expected but not found. Cannot copy side files like .mrk .mtg .bad");
		else {
			// get proc value
			auto proc = map.value("proc");

			outputFile = outputFile.remove(QString("proc-%1").arg(proc));
			outputFile = outputFile.remove(modality);
			outputFile = outputFile.remove(".vhdr");
			
			QFile::copy(QString("%1_channels.tsv").arg(outputFile), QString("%1_channels.tsv").arg(destFile));
			QFile::copy(QString("%1_events.tsv").arg(outputFile), QString("%1_events.tsv").arg(destFile));
			QFile::copy(QString("%1%2.json").arg(outputFile).arg(modality), QString("%1_%2.json").arg(destFile).arg(modality));
			auto userDerivatives = pdi.input.settings.value(keys::bids_user_derivatives_folder).toString();
			auto files = QDir(userDerivatives).entryList(QDir::Files);
			QFileInfo fi(outputFile);
			auto baseFileName = fi.fileName();
			for (auto const& f : files) {
				if (f.startsWith(baseFileName)) {
					auto extension = QFileInfo(f).suffix();
					QFile::copy(QString("%1/%2").arg(userDerivatives).arg(f), 
						QString("%1/%2_proc-%3%4.vhdr.%5").arg(userDerivatives).arg(baseFileName).arg(proc)
					.arg(modality).arg(extension));
				}
			}
		}

	}
	writer->cleanUpAndClose();
	delete writer;
}

void AwExporter::run()
{
	runFromCommandLine();
}

bool AwExporter::showUi()
{
	// detect bids
	QString filePath = pdi.input.settings.value(keys::bids_file_path).toString();
	bool isBids = !filePath.isEmpty();
	AwExportWizardDial dlg(pdi.input.settings);
	if (dlg.exec() == QDialog::Accepted) {
		// clear output_dir option as we don't run in command line mode
		pdi.input.settings.remove(keys::output_dir);
		pdi.input.settings[keys::output_file] = dlg.filePath;
		pdi.input.setNewChannels(AwChannel::duplicateChannels(dlg.channels));
		pdi.input.settings["output_writer"] = dlg.writerPluginName;
		pdi.input.filterSettings = dlg.filterSettings();
		pdi.input.filterSettings.apply(pdi.input.channels());
		pdi.input.setNewMarkers(AwMarkerManager::instance()->getMarkers(), true);
		if (dlg.skipMarkers.size()) {
			pdi.input.settings[keys::skip_markers] = dlg.skipMarkers;
		}
		if (dlg.useMarkers.size()) {
			pdi.input.settings[keys::use_markers] = dlg.useMarkers;
		}
		return true;

	}
	return false;
}