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
#include <AwFiltering.h>
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
	m_downSample = 0.;
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
	if (!m_skippedMarkers.isEmpty()) {
		sendMessage(QString("Skipping some markers from data..."));
		selection = AwMarker::invertMarkerSelection(m_skippedMarkers, "selection", pdi.input.reader()->infos.totalDuration());
		output_markers = AwMarker::cutAroundMarkers(pdi.input.markers, m_skippedMarkers);
		if (selection.isEmpty()) {
			sendMessage(QString("No data to read after markers were skipped. Aborted."));
			return;
		}
	}
	if (!m_exportedMarkers.isEmpty()) {
		sendMessage(QString("Exporting marked selections..."));
		selection = m_exportedMarkers;
	}
	// process export with skipped or exported markers
	if (!selection.isEmpty()) {
		typedef QList<QVector<float> > VectorList;
		QList<VectorList *> channelVectors;
		for (auto c : m_channels) 
			channelVectors << new VectorList;
		for (auto m : selection) {
			sendMessage(QString("Reading data between %1s and %2s ...").arg(m->start()).arg(m->end()));
			if (m_downSample) {
				requestData(&m_channels, m, true);
				if (endOfData()) {
					sendMessage("Error reading data.");
					while (!channelVectors.isEmpty())
						delete channelVectors.takeFirst();
					return;
				}
				sendMessage(QString("Downsampling data..."));
				AwFiltering::downSample(m_channels, m_downSample);
				sendMessage("Done.");
				AwChannel::clearFilters(m_channels);
				m_foptions.setFilters(m_channels);
				sendMessage(QString("Filtering..."));
				AwFiltering::filter(m_channels);
				sendMessage("Done.");
			}
			else {
				AwChannel::clearFilters(m_channels);
				m_foptions.setFilters(m_channels);
				sendMessage(QString("Reading and filtering..."));
				requestData(&m_channels, m);
				if (endOfData()) {
					sendMessage("Error reading data.");
					while (!channelVectors.isEmpty())
						delete channelVectors.takeFirst();
					return;
				}
				sendMessage("Done.");
			}
			for (int i = 0; i < m_channels.size(); i++) {
				AwChannel *c = m_channels.at(i);
				QVector<float> vector = c->toVector();
				channelVectors.at(i)->append(vector);
			}
		}
		// Concatenate all data chunks into one piece for each channels
		for (int i = 0; i < m_channels.size(); i++) {
			AwChannel *c = m_channels.at(i);
			qint64 totalSamples = 0;
			VectorList *vl = channelVectors.at(i);

			for (int j = 0; j < vl->size(); j++) {
				QVector<float> v = vl->at(j);
				totalSamples += v.size();
			}
			float *dest = c->newData(totalSamples);
			for (int j = 0; j < vl->size(); j++) {
				QVector<float> v = vl->at(j);
				float *source = v.data();
				for (int k = 0; k < v.size(); k++)
					*dest++ = *source++;
			}
		}
		while (!channelVectors.isEmpty())
			delete channelVectors.takeFirst();
	} 
	else {  // Export all data, no markers used.
		sendMessage("Reading data...");
		if (m_downSample) {
			requestData(&m_channels, 0., -1.0, true);
			sendMessage("Done.");
			if (endOfData()) {
				sendMessage("Error reading data.");
				return;
			}
			sendMessage(QString("Downsampling data..."));
			AwFiltering::downSample(m_channels, m_downSample);
			sendMessage("Done.");
			AwChannel::clearFilters(m_channels);
			m_foptions.setFilters(m_channels);
			AwFiltering::filter(m_channels);
		}
		else {
			AwChannel::clearFilters(m_channels);
			m_foptions.setFilters(m_channels);
			requestData(&m_channels, 0., -1.0);
			if (endOfData()) {
				sendMessage("Error reading data.");
				return;
			}
		}
		sendMessage("Done.");
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
		m_foptions = ui.foptions;

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
		m_downSample = ui.downSample;
		return true;
	}
	return false;
}