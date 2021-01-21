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
#include "appendfiles.h"
#include <QtCore/qfileinfo.h>
#include <math.h>

AppendFilesPlugin::AppendFilesPlugin()
{
	type = AwProcessPlugin::Background;
	name = "Append Files";
	description = tr("Append compatible files into one.");
	category = "Process:File Operation:Append Files";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData| Aw::ProcessFlags::ProcessHasInputUi);
}

AppendFiles::AppendFiles()
{
	setInputFlags(Aw::ProcessIO::GetReaderPlugins|Aw::ProcessIO::GetWriterPlugins|Aw::ProcessIO::GetAllMarkers);
}

AppendFiles::~AppendFiles()
{
	delete m_ui;
	for (auto reader : m_readers.values()) {
		reader->cleanUpAndClose();
		delete reader;
	}
}



bool AppendFiles::showUi()
{
	for (auto writer : pdi.input.writers)
		m_writers[writer->name] = writer;
	for (auto plugin : pdi.input.readers)
		m_readers[plugin->name] = plugin->newInstance();
	
	m_ui = new settings(m_readers, m_writers.keys());

	if (m_ui->exec() == QDialog::Accepted)
		return true;
	return false;
}

void AppendFiles::run()
{
	int nChannels = 0;
	float sr = 0.;
	float duration = 0.;
	QStringList labels;

	m_writer = m_writers.value(m_ui->m_selectedWriter)->newInstance();
	
	QList<InputFile> inputs;
	auto cleanInputsFunction = [&inputs]() {
		auto tmp = inputs.takeFirst();
		tmp.second->cleanUpAndClose();
		delete tmp.second;
	};

	for (auto input : m_ui->m_files) {
		auto reader = input.second->plugin()->newInstance();
		if (reader->openFile(input.first) == AwFileIO::NoError)
			inputs.append(InputFile(input.first, reader));
		else 
			sendMessage(QString("WARNING: file %1 could not be opend.").arg(input.first));
	}

	if (inputs.size() < 2) {
		sendMessage("ERROR: at least 2 files are required as input.");
		cleanInputsFunction();
		return;
	}

	AwFileIO* reader = inputs.first().second;
	nChannels = reader->infos.channelsCount();
	sr = reader->infos.channels().first()->samplingRate();
	labels = AwChannel::getLabels(reader->infos.channels());
	duration = reader->infos.totalDuration();
	QList<InputFile> removedItems;
	// check consistency of n channels, sr, labels amongst all files
	for (int i = 1; i < inputs.size(); i++) {
		auto tmpReader = inputs.at(i).second;
		int n = tmpReader->infos.channelsCount();
		float s = tmpReader->infos.channels().first()->samplingRate();
		QStringList l = AwChannel::getLabels(tmpReader->infos.channels());
		l += labels;
		int removed = l.removeDuplicates();
		bool ok = (n == nChannels && s == sr && removed == labels.size());
		if (!ok) {
			sendMessage("WARNING: file %1 is not compatible (#channels, sampling rate or channel labels differ.");
			removedItems << inputs.at(i);
		}
		else 
			duration += tmpReader->infos.totalDuration();
	}
	for (auto item : removedItems) {
		item.second->cleanUpAndClose();
		delete item.second;
		inputs.removeAll(item);
	}
	if (inputs.size() < 2) {
		sendMessage("ERROR: at least 2 files are required as input.");
		cleanInputsFunction();
		return;
	}

	//// gather all markers from the input files and reorder them for the expected appended one.
	AwMarkerList outputMarkers = AwMarker::duplicate(reader->infos.blocks().first()->markers());
	float time_offset = reader->infos.totalDuration();
	for (int i = 1; i < inputs.size(); i++) {
		reader = inputs.at(i).second;
		outputMarkers << new AwMarker("File Junction", time_offset);
		AwMarkerList temp = AwMarker::duplicate(reader->infos.blocks().first()->markers());
		for (auto m : temp)
			m->setStart(m->start() + time_offset);
		time_offset += reader->infos.totalDuration();
		outputMarkers += temp;
	}
	// prepare channels for the writer
	AwChannelList sourceChannels = AwChannel::duplicateChannels(inputs.first().second->infos.channels());
	m_writer->infos.setChannels(sourceChannels);
	AwBlock* block = m_writer->infos.newBlock();
	block->setDuration(duration);
	block->setSamples((qint64)floor((double)duration * (double)sr));
	block->setMarkers(outputMarkers);

	if (m_writer->createFile(m_ui->outputFile) != AwFileIO::NoError) {
		sendMessage(m_writer->errorMessage());
		cleanInputsFunction();
		return;
	}
	// read input files using chunks of 5 minutes 
	const float chunkDuration = 300.;
	for (auto inputFile : inputs) {
		reader = inputFile.second;
		QString filePath = inputFile.first;
		float left = reader->infos.totalDuration();
		float duration = std::min(reader->infos.totalDuration(), chunkDuration);
		
		float position = 0;
		while (left > 0) {
			sendMessage(QString("Reading data from file %1...").arg(filePath));
			reader->readDataFromChannels(position, duration, sourceChannels);
			sendMessage("Done.");
			left -= duration;
			// check for MEG data that must be reconverted to T
			for (auto c : sourceChannels) {
				if (c->isMEG() || c->isGRAD() || c->isReference()) {
					auto start = c->data();
					for (auto i = 0; i < c->dataSize(); i++)
						start[i] *= 1e-12;
				}
			}
			sendMessage(QString("Writing data to %1...").arg(m_ui->outputFile));
			m_writer->writeData(&sourceChannels);
			position += duration;
			sendMessage("Done.");
		}
	}

	m_writer->cleanUpAndClose();
	cleanInputsFunction();
	delete m_writer;

}
