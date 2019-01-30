/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
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
	setFlags(Aw::ProcessFlags::ProcessIsScriptable|Aw::ProcessFlags::ProcessHasInputUi);
	pdi.addInputParameter(Aw::ProcessInput::GetWriterPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetCurrentMontage|Aw::ProcessInput::ProcessIgnoresChannelSelection, "0-n");
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, "0-n");
	m_decimateFactor = 1;
}

AwExporter::~AwExporter() 
{
	while (!m_skippedMarkers.isEmpty())
		delete m_skippedMarkers.takeFirst();
	while (!m_exportedMarkers.isEmpty())
		delete m_exportedMarkers.takeFirst();
}

void AwExporter::run()
{
	AwMarkerList output_markers = pdi.input.markers;
	AwMarkerList selection;
	bool skip = !m_skippedMarkers.isEmpty();
	bool use = !m_exportedMarkers.isEmpty();
	AwMarkerList filteredMarkers;
	bool isDecimate = m_decimateFactor > 1;

	if (skip || use) {
		auto skipped = AwMarker::getAllLabels(m_skippedMarkers);
		auto used = AwMarker::getAllLabels(m_exportedMarkers);
		filteredMarkers = AwMarker::applySelectionFilter(pdi.input.markers, skipped, used, pdi.input.reader()->infos.totalDuration());
		if (filteredMarkers.isEmpty()) {
			sendMessage("Nothing to export considering skip/export markers options set.");
			return;
		}
	}

	pdi.input.filterSettings.apply(m_channels);

	sendMessage("Loading data, filtering and maybe decimating...");
	if (filteredMarkers.isEmpty()) {  // get the whole data 
		if (isDecimate) {
			requestData(&m_channels, 0.0f, -1.f, true);
			AwFiltering::downSample(m_channels, m_decimateFactor);
		}
		else {
			requestData(&m_channels, 0.0f, -1.f);
		}
		sendMessage("Done.");
	}
	else {
		if (skip || use) {
			// concatenate chunks into one 
			if (skip && !use)
				output_markers = AwMarker::cutAroundMarkers(pdi.input.markers, m_skippedMarkers);
			if (use && !skip)
				;
			if (isDecimate) {
				// requestData will concatenate chunks.
				requestData(&m_channels, &m_skippedMarkers, true);
				AwFiltering::downSample(m_channels, m_decimateFactor);
			}
			else {
				requestData(&m_channels, &m_skippedMarkers);
			}
			sendMessage("Done.");
		}

	}

	AwFileIO *writer = m_plugin->newInstance();
	writer->setPlugin(m_plugin);
	writer->infos.setChannels(m_channels);
	AwBlock *block = writer->infos.newBlock();
	block->setSamples(m_channels.first()->dataSize());
	block->setDuration((float)m_channels.first()->dataSize() / m_channels.first()->samplingRate());
	writer->infos.setDate(pdi.input.reader()->infos.recordingDate());
	writer->infos.setTime(pdi.input.reader()->infos.recordingTime());
	writer->infos.setISODate(pdi.input.reader()->infos.isoDate());

	if (!output_markers.isEmpty())
		block->setMarkers(output_markers);
	if (writer->createFile(m_path) != AwFileIO::NoError) {
		sendMessage(tr("Error creating output file."));
		m_plugin->deleteInstance(writer);
		return;
	}
	sendMessage(tr("Writing data..."));
	writer->writeData(&m_channels);
	sendMessage("Done.");
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
	for (auto c : pdi.input.channels) {
		if (c->isICA())
			m_ICAChannels << c;
		else 
			ui.channels << c;
	}

	ui.markers = pdi.input.markers;
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
		if (ui.skipMarkers)
			m_skippedMarkers = ui.skippedMarkers;
		if (ui.exportMarkers)
			m_exportedMarkers = ui.exportedMarkers;
		m_decimateFactor = ui.decimateFactor;
		return true;
	}
	return false;
}