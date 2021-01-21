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
#include <AwCore.h>
#include <AwKeys.h>


AwExporterPlugin::AwExporterPlugin()
{
	name = QString("File Exporter");
	description = QString(tr("Export data to a file"));
	version = "1.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
}

AwExporter::AwExporter() : AwProcess()
{
	setInputFlags(Aw::ProcessIO::GetAllMarkers| Aw::ProcessIO::GetWriterPlugins | Aw::ProcessIO::GetReaderPlugins | Aw::ProcessIO::GetCurrentMontage
		| Aw::ProcessIO::IgnoreChannelSelection);
	m_decimateFactor = 1;
}

AwExporter::~AwExporter() 
{
}

void AwExporter::runFromCommandLine()
{
	auto args = pdi.input.settings;
	AwFileIO *reader = pdi.input.reader();
	auto inputFile = args.value(keys::input_file).toString();
	auto outputFile = args.value(keys::output_file).toString();
	// check for the outputs
	auto writerName = args["output_writer"].toString();

	QString outputPath = outputFile;
	if (args.contains(keys::output_dir))
		outputPath = QString("%1/%2").arg(args.value(keys::output_dir).toString()).arg(outputFile);

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
	AwMarkerList markers;
	if (args.contains(keys::marker_file))
		markers = AwMarker::load(args.value(keys::marker_file).toString());

	if (markers.isEmpty())
		m_inputMarkers << new  AwMarker("global", 0, endTimePos);
	else {
		if (args.contains(keys::skip_markers) || args.contains(keys::use_markers)) {
			auto skippedLabels = args.value(keys::skip_markers).toStringList();
			auto usedLabels = args.value(keys::use_markers).toStringList();
			m_outputMarkers = markers;
			m_inputMarkers = AwMarker::getInputMarkers(m_outputMarkers, skippedLabels, usedLabels, endTimePos);
		}
		else { // no use neither skip options => read all the data and export all the markers
			m_inputMarkers << new  AwMarker("global", 0, endTimePos);
			m_outputMarkers = AwMarker::duplicate(markers);
		}
	}
	writer->infos.setChannels(pdi.input.channels());
	AwBlock *block = writer->infos.newBlock();

	if (!m_outputMarkers.isEmpty()) {
		block->setMarkers(m_outputMarkers);
		AW_DESTROY_LIST(m_outputMarkers);
	}

	sendMessage("Loading data...");
	requestData(&pdi.input.channels(), &m_inputMarkers);
	sendMessage("Done.");
	AW_DESTROY_LIST(m_inputMarkers);

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
	sendMessage("Writing data...");
	writer->writeData(&pdi.input.channels());
	sendMessage("Done.");
	writer->cleanUpAndClose();
	writer->plugin()->deleteInstance(writer);
}

void AwExporter::run()
{
	bool isDecimate = m_decimateFactor > 1;

	AwFileIO *writer = m_ioPlugin->newInstance();
	writer->setPlugin(m_ioPlugin);
	
	AwBlock *block = writer->infos.newBlock();
	
	sendMessage("Loading data...");
	if (isDecimate) {
		AwChannel::clearFilters(m_channels);
		requestData(&m_channels, &m_inputMarkers, true);
		AwFiltering::downSample(m_channels, m_decimateFactor);
		// apply filters set in the UI
		pdi.input.filterSettings.apply(m_channels);
		AwFiltering::filter(m_channels);
	}
	else {
		requestData(&m_channels, &m_inputMarkers);
	}
	sendMessage("Done.");
	AW_DESTROY_LIST(m_inputMarkers);

	block->setSamples(m_channels.first()->dataSize());
	block->setDuration((float)m_channels.first()->dataSize() / m_channels.first()->samplingRate());
	writer->infos.setDate(pdi.input.reader()->infos.recordingDate());
	writer->infos.setTime(pdi.input.reader()->infos.recordingTime());
	writer->infos.setISODate(pdi.input.reader()->infos.isoDate());

	// now that the channels have optionaly being renamed, create the output file.
	writer->infos.setChannels(m_channels);
	if (!m_outputMarkers.isEmpty()) {
		writer->infos.blocks().first()->setMarkers(m_outputMarkers);
		AW_DESTROY_LIST(m_outputMarkers);
	}

	if (writer->createFile(m_path) != AwFileIO::NoError) {
		sendMessage(tr("Error creating output file."));
		m_ioPlugin->deleteInstance(writer);
		return;
	}

	sendMessage(tr("Writing data..."));
	writer->writeData(&m_channels);
	sendMessage("Done.");
	writer->cleanUpAndClose();
	m_ioPlugin->deleteInstance(writer);
}

bool AwExporter::showUi()
{
	AwExporterSettings ui;
	ui.initialPath = QString("%1/NewFile").arg(pdi.input.settings.value(keys::data_dir).toString());
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
		m_ioPlugin = writers.value(ui.writer);
		
		if (QFile::exists(ui.filePath)) {
			if (AwMessageBox::information(0, tr("File"), tr("the file already exists. Overwrite?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
				return false;
		}
		m_path = ui.filePath;
		pdi.input.filterSettings = ui.filterSettings;
		/** Apply filters **/
		ui.filterSettings.apply(m_channels);

		// if Export All ICA channels is checked => add all ICA channels to export
		m_exportICAChannels = ui.exportICA;
		if (m_exportICAChannels) {
			m_ICAChannels = ui.icaChannels;
			QString extension = ui.extensions.value(ui.writers.indexOf(ui.writer));
			QString destFile = ui.filePath + extension + ".ica.mat";
			if (QFile::exists(destFile))
				QFile::remove(destFile);
			// copy the ICA.Mat fileto be used with the exported file.
			QFile::copy(pdi.input.settings.value(keys::ica_file).toString(), destFile);
		}
		else if (!ui.selectedICA.isEmpty()) // only a subset of ICA channels are selected for export
			m_ICAChannels = ui.selectedICA;

		// build the complete list of channels to export
		if (!m_ICAChannels.isEmpty())
			m_channels += m_ICAChannels;

		if (!ui.skippedMarkers.isEmpty() || !ui.usedMarkers.isEmpty()) {
			m_outputMarkers = AwMarker::duplicate(pdi.input.markers());
			m_inputMarkers = AwMarker::getInputMarkers(m_outputMarkers, ui.skippedMarkers, ui.usedMarkers, 
				pdi.input.settings.value(keys::file_duration).toFloat());
		}
		else {
			m_inputMarkers << new AwMarker("whole data", 0., pdi.input.settings.value(keys::file_duration).toFloat());
			m_outputMarkers = AwMarker::duplicate(pdi.input.markers());
		}

		m_decimateFactor = ui.decimateFactor;
		m_relabelChannels = ui.renameLabels;
		return true;
	}
	return false;
}