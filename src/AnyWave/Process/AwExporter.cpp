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
	if (m_downSample) {
		sendMessage("Loading data...");
		requestData(&m_channels, 0, -1, true);
		sendMessage("Done.");
		sendMessage("Downsampling...");
		AwFiltering::downSample(m_channels, m_downSample);
		sendMessage("Done.");
		AwChannel::clearFilters(m_channels);
		m_foptions.setFilters(m_channels);
		sendMessage("Applying filters...");
		AwFiltering::filter(m_channels);
		sendMessage("Done.");
	}
	else {
		AwChannel::clearFilters(m_channels);
		m_foptions.setFilters(m_channels);
		sendMessage("Loading and filtering...");
		requestData(&m_channels, 0., -1.);
		sendMessage("Done.");
	}

	//// clean filters for channels and apply those specified by the user
	//for (auto c : m_channels) {
	//	c->setLowFilter(0);
	//	c->setHighFilter(0);
	//	c->setNotch(0);
	//	switch (c->type()) {
	//	case AwChannel::EEG:
	//	case AwChannel::SEEG:
	//		if (m_foptions.eegHP)
	//			c->setHighFilter(m_foptions.eegHP);
	//		if (m_foptions.eegLP)
	//			c->setLowFilter(m_foptions.eegLP);
	//		if (m_foptions.eegNotch)
	//			c->setNotch(m_foptions.eegNotch);
	//		break;
	//	case AwChannel::MEG:
	//	case AwChannel::GRAD:
	//		if (m_foptions.megHP)
	//			c->setHighFilter(m_foptions.megHP);
	//		if (m_foptions.megLP)
	//			c->setLowFilter(m_foptions.megLP);
	//		if (m_foptions.megNotch)
	//			c->setNotch(m_foptions.megNotch);
	//		break;
	//	case AwChannel::EMG:
	//	case AwChannel::ECG:
	//		if (m_foptions.emgHP)
	//			c->setHighFilter(m_foptions.emgHP);
	//		if (m_foptions.emgLP)
	//			c->setLowFilter(m_foptions.emgLP);
	//		if (m_foptions.emgNotch)
	//			c->setNotch(m_foptions.emgNotch);
	//		break;
	//	}
	//}

	//if (!m_downSample) {
	//	emit progressChanged(tr("Loading and filtering data...."));
	//}
	//else {
	//	emit progressChanged(tr("Loading, downsampling and filtering data..."));
	//}


	//requestData(&m_channels, 0, -1, m_downSample);

	if (endOfData()) {
		emit progressChanged(tr("Error loading data."));
		AwChannel::clearData(m_channels);
		//foreach (AwChannel *c, m_channels)
		//	c->clearData();
		return;
	}
	emit progressChanged(tr("Done."));

	AwFileIO *writer = m_plugin->newInstance();
	writer->setPlugin(m_plugin);
	writer->infos.setChannels(m_channels);
	AwBlock *block = writer->infos.newBlock();

	if (!m_selectedMarkers.isEmpty()) {
		// skipped data marked by selected markers
		emit progressChanged(tr("Skipping data marked by selected markers..."));
		AwMarkerList all_markers = pdi.input.markers;
		// remove selected markers from all markers if the names match
		QStringList labels = AwMarker::getAllLabels(m_selectedMarkers);
		foreach(AwMarker *m, all_markers) {
			if (labels.contains(m->label()))
				all_markers.removeAll(m);
		}
		AwMarkerList markers = AwMarker::cutAroundMarkers(all_markers, m_selectedMarkers);
		AwMarkerList merged = AwMarker::merge(m_selectedMarkers);

		float cut_duration = 0., sr = m_channels.first()->samplingRate();
		typedef QPair<qint64, qint64> sample_marker;
		QList<sample_marker> samples_cut;	// convert markers in samples
		// compute the number of samples that will be removed
		foreach(AwMarker *m, merged) {
			cut_duration += m->duration();
			// samples cut contains start and end converted to samples for each markers
			samples_cut << QPair<qint64, qint64>((qint64)floor(m->start() * sr), (qint64)floor(m->end() * sr));
		}
		qint64 removed_samples = (qint64)floor(sr * cut_duration);
		// compute the new size of a channel in samples
		qint64 samples = m_channels.first()->dataSize() - removed_samples;
		float *buf = new float[samples];
		foreach (AwChannel *c, m_channels) {
			float *source = c->data();
			qint64 source_pos = 0, buf_pos = 0;
			foreach(sample_marker pair, samples_cut) {
				// copy data which are before the marker.
				while (source_pos < pair.first)
					buf[buf_pos++] = source[source_pos++];

				// skip data marked by marker
				source_pos = pair.second + 1;
			}
			float *dest = c->newData(samples);
			memcpy(dest, buf, samples * sizeof(float));
		}
		emit progressChanged(tr("Done."));
		delete[] buf;
		while (!merged.isEmpty())
			delete merged.takeFirst();
		block->setSamples(samples);
		block->setDuration(pdi.input.reader()->infos.totalDuration() - cut_duration);
		block->setMarkers(markers);
		if (writer->createFile(m_path) != AwFileIO::NoError) {
			emit progressChanged(tr("Error creating output file."));
			foreach (AwChannel *c, m_channels)
				c->clearData();
			m_plugin->deleteInstance(writer);
			while (!markers.isEmpty())
				delete markers.takeFirst();
			while (!m_selectedMarkers.isEmpty()) 
				delete m_selectedMarkers.takeFirst();
			return;
		}
	}
	else {
		block->setDuration(pdi.input.reader()->infos.totalDuration());
		block->setSamples(m_channels.first()->dataSize());
		block->setMarkers(pdi.input.markers);
		if (writer->createFile(m_path) != AwFileIO::NoError) {
			emit progressChanged(tr("Error creating output file."));
			foreach (AwChannel *c, m_channels)
				c->clearData();
			m_plugin->deleteInstance(writer);
			return;
		}
	}
	emit progressChanged(tr("Writing data..."));
	writer->writeData(&m_channels);
	foreach (AwChannel *c, m_channels)
		c->clearData();
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