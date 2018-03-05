/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "fileconverter.h"
#include "settings.h"
#include <AwFiltering.h>

FileConverterPlugin::FileConverterPlugin()
{
	type = AwProcessPlugin::Background;
	name = "File Convert";
	description = tr("Converts files to another format.");
	category = "Process:File Operation:Convert Files";
	setFlags(Aw::ProcessFlags::ProcessDontRequireData);
}

FileConverter::FileConverter()
{
	setFlags(Aw::ProcessFlags::ProcessHasInputUi|Aw::ProcessFlags::ProcessIsScriptable);
	pdi.addInputParameter(Aw::ProcessInput::GetReaderPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetWriterPlugins, "1-n");
}

FileConverter::~FileConverter()
{
	delete m_ui;
}

bool FileConverter::showUi()
{
	m_ui = new settings(pdi.input.readers, pdi.input.writers);

	// check for previous settings
	QString settingsFile = pdi.input.workingDirPath + "/paths.txt";
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
		// check for filtering
		if (m_ui->filters) {
			foreach (AwChannel *c, sourceChannels) {
				switch (c->type()) {
				case AwChannel::EEG:
				case AwChannel::SEEG:
					c->setLowFilter(m_ui->eegLP);
					c->setHighFilter(m_ui->eegHP);
					c->setNotch(m_ui->eegNotch);
					break;
				case AwChannel::MEG:
					c->setLowFilter(m_ui->megLP);
					c->setHighFilter(m_ui->megHP);
					c->setNotch(m_ui->megNotch);
					break;
				case AwChannel::ECG:
				case AwChannel::EMG:
					c->setLowFilter(m_ui->emgLP);
					c->setHighFilter(m_ui->emgHP);
					c->setNotch(m_ui->emgNotch);
					break;
				}
			}
			emit progressChanged(tr("Filtering data..."));
			AwFiltering::filter(&sourceChannels);
			AwFiltering::notch(&sourceChannels);
			emit progressChanged(tr("Done..."));
		}

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

