/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "AwExporter.h"
#include "AwExporterSettings.h"
#include <filter/AwFiltering.h>
#include <widget/AwMessageBox.h>


AwExporterPlugin::AwExporterPlugin()
{
	name = QString("File Exporter");
	description = QString(tr("Export data to a file"));
	version = "1.0";
	type = AwProcessPlugin::Background;
}

AwExporter::AwExporter() : AwProcess()
{
	setFlags(Aw::ProcessFlags::ProcessIsScriptable|Aw::ProcessFlags::ProcessHasInputUi|Aw::ProcessFlags::CanRunFromCommandLine);
	pdi.addInputParameter(Aw::ProcessInput::GetWriterPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetReaderPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetCurrentMontage|Aw::ProcessInput::ProcessIgnoresChannelSelection, "0-n");
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, "0-n");
	m_decimateFactor = 1;
}

AwExporter::~AwExporter() 
{
}

void AwExporter::runFromCommandLine()
{
	auto args = pdi.input.args();
	AwFileIO *reader = pdi.input.reader();
	auto inputFile = args["input_file"].toString();
	auto outputFile = args["output_file"].toString();
	// check for the outputs
	auto writerName = args["output_writer"].toString();

	QString outputPath = outputFile;
	if (args.contains("output_dir"))
		outputPath = QString("%1/%2").arg(args["output_dir"].toString()).arg(outputFile);

	AwFileIO *writer = Q_NULLPTR;
	for (auto p : pdi.input.writers) {
		if (p->name == writerName) {
			writer = p->newInstance();
			break;
		}
	 }
	if (writer == Q_NULLPTR) {
		sendMessage(QString("Plugin %1 not found.").arg(writerName));
		return;
	}

	auto endTimePos = reader->infos.totalDuration();
	AwMarkerList markers = pdi.input.markers(), skippedMarkers, output_markers, input_markers;
	if (args.contains("marker_file"))
		markers = AwMarker::load(args["marker_file"].toString());
	if (args.contains("skip_marker")) {
		auto labels = AwMarker::getMarkersWithLabel(markers, args["skip_marker"].toString());
		skippedMarkers = AwMarker::merge(labels);
		input_markers = AwMarker::invertMarkerSelection(skippedMarkers, "osef", endTimePos);
		output_markers = AwMarker::cutAroundMarkers(markers, skippedMarkers);
		/** destroy skipped markers as AwMarker::merge() duplicates marker objects **/
		qDeleteAll(skippedMarkers);
	}
	else
		output_markers = markers;

	writer->infos.setChannels(pdi.input.channels());
	AwBlock *block = writer->infos.newBlock();

	if (!output_markers.isEmpty())
		block->setMarkers(output_markers);

	if (input_markers.isEmpty())
		input_markers << new AwMarker("global", 0, endTimePos);

	sendMessage("loading data...");
	requestData(&pdi.input.channels(), &input_markers);
	sendMessage("Done.");

	block->setSamples(pdi.input.channels().first()->dataSize());
	block->setDuration((float)pdi.input.channels().first()->dataSize() / pdi.input.channels().first()->samplingRate());
	writer->infos.setDate(reader->infos.recordingDate());
	writer->infos.setTime(reader->infos.recordingTime());
	writer->infos.setISODate(reader->infos.isoDate());

	if (writer->createFile(outputPath) != AwFileIO::NoError) {
		sendMessage(QString("Error creating %1.").arg(outputPath));
		writer->plugin()->deleteInstance(writer);
		return;
	}
	sendMessage("Writting data...");
	writer->writeData(&pdi.input.channels());
	sendMessage("Done.");
	writer->cleanUpAndClose();
	writer->plugin()->deleteInstance(writer);

	while (!input_markers.isEmpty())
		delete input_markers.takeFirst();
	while (!output_markers.isEmpty())
		delete output_markers.takeFirst();
}

void AwExporter::run()
{
	auto output_markers = AwMarker::duplicate(pdi.input.markers());
	bool skip = !m_skippedMarkers.isEmpty();
	bool use = !m_exportedMarkers.isEmpty();
	bool isDecimate = m_decimateFactor > 1;
	auto skipLabels = AwMarker::getAllLabels(m_skippedMarkers);
	auto useLabels = AwMarker::getAllLabels(m_exportedMarkers);

	auto inputMarkers = AwMarker::getInputMarkers(AwMarker::sort(output_markers), skipLabels, useLabels, pdi.input.reader()->infos.totalDuration());

	AwFileIO *writer = m_plugin->newInstance();
	writer->setPlugin(m_plugin);
	
	AwBlock *block = writer->infos.newBlock();

	if (!output_markers.isEmpty())
		block->setMarkers(output_markers);

	sendMessage("Loading data...");
	if (isDecimate) {
		requestData(&m_channels, &inputMarkers, true);
		AwFiltering::downSample(m_channels, m_decimateFactor);
	}
	else {
		requestData(&m_channels, &inputMarkers);
	}
	sendMessage("Done.");
	qDeleteAll(inputMarkers);

	block->setSamples(m_channels.first()->dataSize());
	block->setDuration((float)m_channels.first()->dataSize() / m_channels.first()->samplingRate());
	writer->infos.setDate(pdi.input.reader()->infos.recordingDate());
	writer->infos.setTime(pdi.input.reader()->infos.recordingTime());
	writer->infos.setISODate(pdi.input.reader()->infos.isoDate());

	if (m_relabelChannels) {
		// remove 0 in label likely (A01 will become A1).
		// check if the renaming does not create doublons !
		QMap<QString, AwChannel *> map;
		for (auto c : m_channels)
			map[c->name()] = c;
		for (auto c : m_channels) {
			if (c->isEEG() || c->isSEEG()) {
				auto label = c->name();
				QString newLabel;
				QRegularExpression re("\\d+$");
				QRegularExpressionMatch match = re.match(label);
				if (match.hasMatch()) {
					QString elec = label.remove(re);
					int number = match.captured(0).toInt();
					newLabel = QString("%1%2").arg(elec).arg(number);
					// search if newLabel already exists = do not create doublons.
					if (map.contains(newLabel)) {
						auto channel = map[newLabel];
						// rename existing label by adding "-" between name and number.
						QString label = QString("%1_%2").arg(elec).arg(number);
						channel->setName(label);
						map.remove(newLabel);
						map[label] = channel;
					}
					else {
						map.remove(c->name());
						map[newLabel] = c;
					}
				}
			}
		}
	}
	// now that the channels have optionaly being renamed, create the output file.
	writer->infos.setChannels(m_channels);

	if (writer->createFile(m_path) != AwFileIO::NoError) {
		sendMessage(tr("Error creating output file."));
		m_plugin->deleteInstance(writer);
		return;
	}



	sendMessage(tr("Writting data..."));
	writer->writeData(&m_channels);
	sendMessage("Done.");
	if (use || skip) // output markers contains duplicated markers => delete them
		while (!output_markers.isEmpty())
			delete output_markers.takeFirst();
	writer->cleanUpAndClose();
	m_plugin->deleteInstance(writer);
}

bool AwExporter::showUi()
{
	AwExporterSettings ui;
	ui.initialPath = QString("%1/NewFile").arg(pdi.input.dataFolder);
	QHash<QString, AwFileIOPlugin *> writers;
	for (auto p : pdi.input.writers) {
		writers.insert(p->name, p);
		ui.extensions << p->fileExtension;
		ui.writers << p->name;
	}

	// separate ICA channels from others, if any
	for (auto c : pdi.input.channels()) {
		if (c->isICA())
			m_ICAChannels << c;
		else 
			ui.channels << c;
	}

	ui.markers = pdi.input.markers();
	ui.icaChannels = m_ICAChannels;
	ui.filterSettings = pdi.input.filterSettings;

	if (ui.exec() == QDialog::Accepted) {
		if (ui.useCurrentMontage)
			m_channels = ui.channels;
		else
			m_channels = ui.selectedChannels;
		m_plugin = writers.value(ui.writer);

		if (QFile::exists(ui.filePath)) {
			if (AwMessageBox::information(0, tr("File"), tr("the file already exists. Overwrite?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
				return false;
		}
		m_path = ui.filePath;
		pdi.input.filterSettings = ui.filterSettings;

		// if Export All ICA channels is checked => add all ICA channels to export
		m_exportICAChannels = ui.exportICA;
		if (m_exportICAChannels) 
			m_ICAChannels = ui.icaChannels;
		else if (!ui.selectedICA.isEmpty()) // only a subset of ICA channels are selected for export
			m_ICAChannels = ui.selectedICA;

		// build the complete list of channels to export
		if (!m_ICAChannels.isEmpty())
			m_channels += m_ICAChannels;
		m_skippedMarkers = ui.skippedMarkers();
		m_exportedMarkers = ui.usedMarkers();
		m_decimateFactor = ui.decimateFactor;
		m_relabelChannels = ui.renameLabels;
		return true;
	}
	return false;
}