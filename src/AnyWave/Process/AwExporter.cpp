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

namespace Exporter {
	constexpr auto output_writer = "output_writer";
	constexpr auto decimate_factor = "decimate_factor";
};


AwExporterPlugin::AwExporterPlugin()
{
	name = QString("File Exporter");
	description = QString(tr("Export data to a file"));
	category = "Process:File Operation:Export To File";
	version = "1.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine);
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
	auto outputFile = args.value(keys::output_file).toString();
	// check for the outputs
	auto writerName = args.value(Exporter::output_writer).toString();
	int decimateFactor = 1;
	if (args.contains(Exporter::decimate_factor))
		decimateFactor = args.value(Exporter::decimate_factor).toInt();


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

	float endTimePos = args.value(keys::file_duration).toFloat();
	auto markers = pdi.input.markers();


	AwBlock* block = writer->infos.newBlock();
	AwMarker global("global", 0., endTimePos);
	if (modifiersFlags() & Aw::ProcessIO::modifiers::UseOrSkipMarkersApplied) {
		block->setMarkers(pdi.input.modifiedMarkers());
		m_inputMarkers = pdi.input.markers();
	}
	else {
		block->setMarkers(pdi.input.markers());
		m_inputMarkers << &global;  // the trick is to avoid memory leak as we don't destroy m_inputMarkers list.
	}

	if (decimateFactor > 1) {
		sendMessage("Loading data...");
		//	AwChannel::clearFilters(m_channels);
	   requestData(&pdi.input.channels(), &m_inputMarkers, true);
	   sendMessage("Done.");
	   sendMessage("Downsampling...");
	   AwFiltering::downSample(pdi.input.channels(), decimateFactor);
		// apply filters set in the UI
		AwFiltering::filter(pdi.input.channels());
		sendMessage("Done.");
	}
	else {
		sendMessage("Loading data...");
		requestData(&pdi.input.channels(), &m_inputMarkers);
		sendMessage("Done.");
	}

	// set channels to the writer object AFTER loading and/or not downsampling
	// duplicate input channels before set them to writer as writer object takes ownership of channels and will destroy them 
	writer->infos.setChannels(AwChannel::duplicateChannels(pdi.input.channels()));

	block->setSamples(pdi.input.channels().first()->dataSize());
	block->setDuration((float)pdi.input.channels().first()->dataSize() / pdi.input.channels().first()->samplingRate());
	writer->infos.setDate(args.value(keys::date).toString());
	writer->infos.setTime(args.value(keys::time).toString());
	writer->infos.setISODate(args.value(keys::iso_date).toString());

	if (writer->createFile(outputPath) != AwFileIO::NoError) {
		sendMessage(QString("Error creating %1.").arg(outputPath));
		writer->cleanUpAndClose();
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
	runFromCommandLine();
}

bool AwExporter::showUi()
{
	AwExporterSettings ui(this);
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
		pdi.input.settings[keys::output_file] = ui.filePath;
		if (ui.useCurrentMontage)
			m_channels = ui.channels;
		else
			m_channels = ui.selectedChannels;

		pdi.input.settings[Exporter::output_writer] = ui.writer;
		
		if (QFile::exists(ui.filePath)) {
			if (AwMessageBox::information(0, tr("File"), tr("the file already exists. Overwrite?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
				return false;
		}
		//m_path = ui.filePath;
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

		pdi.input.setNewChannels(AwChannel::duplicateChannels(m_channels));

		if (!ui.skippedMarkers.isEmpty())
			pdi.input.settings[keys::skip_markers] = ui.skippedMarkers;
		if (!ui.usedMarkers.isEmpty())
			pdi.input.settings[keys::use_markers] = ui.usedMarkers;

		if (ui.decimateFactor > 1)
			pdi.input.settings[Exporter::decimate_factor] = ui.decimateFactor;

		return true;
	}
	return false;
}