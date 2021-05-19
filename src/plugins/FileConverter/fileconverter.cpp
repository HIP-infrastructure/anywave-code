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
#include "fileconverter.h"
#include "settings.h"
#include <filter/AwFiltering.h>
#include <AwKeys.h>

FileConverterPlugin::FileConverterPlugin()
{
	type = AwProcessPlugin::Background;
	name = "File Converter";
	description = tr("Converts files to another format.");
	category = "Process:File Operation:Convert Files";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData | Aw::ProcessFlags::ProcessHasInputUi);
	m_helpUrl = "File Exporter::File Operations::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_file_exporter";
}

FileConverter::FileConverter()
{
	setInputFlags(Aw::ProcessIO::GetReaderPlugins|Aw::ProcessIO::GetWriterPlugins);
}

FileConverter::~FileConverter()
{
	delete m_ui;
}

bool FileConverter::showUi()
{
	m_ui = new settings(pdi.input.readers, pdi.input.writers);

	// check for previous settings
	QString settingsFile = pdi.input.settings.value(keys::working_dir).toString() + "/paths.txt";
	if (QFile::exists(settingsFile)) {
		QFile file(settingsFile);
		if (file.open(QIODevice::ReadOnly|QIODevice::Text)) {
			QTextStream stream(&file);
			stream >> m_ui->outputDir;
			file.close();
		}
	}
	if (m_ui->exec() == QDialog::Accepted) {
		QFile file(settingsFile);
		if (file.open(QIODevice::ReadWrite|QIODevice::Text)) {
			QTextStream stream(&file);
			stream << m_ui->outputDir << endl;
			file.close();
		}
		return true;
	}
	return false;
}

void FileConverter::run()
{
	AwFileIO *writer = m_ui->selectedWriter->newInstance();
	writer->setPlugin(m_ui->selectedWriter);

	int count = 1;
	foreach (input in, m_ui->inputs) {
		emit progressChanged((count * 100) / m_ui->inputs.size());
		emit progressChanged(tr("Converting file ") + in.second + "...");
		AwFileIO *fr = in.first->newInstance();
		fr->setPlugin(in.first);

		if (fr->openFile(in.second) != AwFileIO::NoError) {
			emit progressChanged(tr("Failed to open the file."));
			continue;
		}

		AwChannelList sourceChannels = AwChannel::duplicateChannels(fr->infos.channels());
		writer->infos.setChannels(sourceChannels);
		AwBlock *block = writer->infos.newBlock();
		block->setDuration(fr->infos.totalDuration());
		block->setSamples(fr->infos.totalSamples());
		writer->infos.setDate(fr->infos.recordingDate());
		writer->infos.setTime(fr->infos.recordingTime());
		writer->infos.setISODate(fr->infos.isoDate());

		// if a .mrk is attached to the file use it, otherwise use markers which come from the data file

		// extract filename from source file
		QFileInfo fi(in.second);
		QString outputFilename = m_ui->outputDir + "/" + fi.baseName();
		QString mrkFilename = in.second + ".mrk";
		if (QFile::exists(mrkFilename)) {
			AwMarkerList markers = AwMarker::load(mrkFilename);
			block->setMarkers(markers);
		}
		else
			block->setMarkers(fr->infos.blocks().first()->markers());
		
		writer->createFile(outputFilename);

		emit progressChanged(tr("Reading data..."));
		fr->readDataFromChannels(0, fr->infos.totalDuration(), sourceChannels);
		emit progressChanged(tr("Done."));

		// apply filters (note: this can be an empty operation if filters are all set to zeros).
		sendMessage(tr("Filtering (optional)..."));
		m_ui->filterSettings().apply(sourceChannels);
		AwFiltering::filter(&sourceChannels);
		sendMessage(tr("Done."));
	
		emit progressChanged(tr("Writing data..."));
		writer->writeData(&sourceChannels);
		emit progressChanged(tr("Done."));
		in.first->deleteInstance(fr);
		count++;
		writer->cleanUpAndClose();

		// destroy source channels
		while (!sourceChannels.isEmpty())
			delete sourceChannels.takeFirst();
	}
	m_ui->selectedWriter->deleteInstance(writer);
}

