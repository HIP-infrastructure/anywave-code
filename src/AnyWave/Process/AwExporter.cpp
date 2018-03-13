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
{}



void AwExporter::run()
{
	AwMarkerList output_markers = pdi.input.markers;
	if (!m_selectedMarkers.isEmpty()) {
		AwMarkerList selection = AwMarker::invertMarkerSelection(m_selectedMarkers, "selection", pdi.input.reader()->infos.totalDuration());
		output_markers = AwMarker::cutAroundMarkers(pdi.input.markers, m_selectedMarkers);
		if (selection.isEmpty()) {
			sendMessage(QString("No data to read after markers were skipped."));
			return;
		}
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
				AwFiltering::filter(m_channels);
			}
			else {
				AwChannel::clearFilters(m_channels);
				m_foptions.setFilters(m_channels);
				requestData(&m_channels, m);
				if (endOfData()) {
					sendMessage("Error reading data.");
					while (!channelVectors.isEmpty())
						delete channelVectors.takeFirst();
					return;
				}
			}
			for (int i = 0; i < m_channels.size(); i++) {
				AwChannel *c = m_channels.at(i);
				QVector<float> vector = c->toVector();
				channelVectors.at(i)->append(vector);
			}
			sendMessage("Done.");
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
	else {  // No markers to skip
		sendMessage("Reading data...");
		if (m_downSample) {
			requestData(&m_channels, 0., -1.0, true);
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
	block->setMarkers(output_markers);
	if (writer->createFile(m_path) != AwFileIO::NoError) {
		sendMessage(tr("Error creating output file."));
		m_plugin->deleteInstance(writer);
		while (!m_selectedMarkers.isEmpty())
			delete m_selectedMarkers.takeFirst();
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
	QHash<QString, AwFileIOPlugin *> writers;
	foreach (AwFileIOPlugin *p, pdi.input.writers) {
		writers.insert(p->name, p);
		ui.extensions << p->fileExtension;
		ui.writers << p->name;
	}

	// separate ICA channels from others, if any
	foreach(AwChannel *c, pdi.input.channels) {
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

		m_selectedMarkers = ui.selectedMarkers;
		m_downSample = ui.downSample;
		return true;
	}
	return false;
}