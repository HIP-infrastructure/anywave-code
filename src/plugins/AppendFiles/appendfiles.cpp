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
	setFlags(Aw::ProcessFlags::ProcessDontRequireData);
}

AppendFiles::AppendFiles()
{
	setFlags(Aw::ProcessFlags::ProcessHasInputUi|Aw::ProcessFlags::ProcessIsScriptable);
	pdi.addInputParameter(Aw::ProcessInput::GetReaderPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetWriterPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetAllMarkers, "0-n");
}

AppendFiles::~AppendFiles()
{
}


bool AppendFiles::showUi()
{
	m_ui = new settings(pdi.input.readers, pdi.input.writers);

	if (m_ui->exec() == QDialog::Accepted)
		return true;
	return false;
}

void AppendFiles::error(const QString& message)
{
	emit progressChanged(message);
	m_ui->selectedWriter->deleteInstance(m_writer);
	while (!m_readers.isEmpty()) {
		AwFileIO *r = m_readers.takeFirst();
		r->plugin()->deleteInstance(r);
	}
}


void AppendFiles::run()
{
	m_writer = m_ui->selectedWriter->newInstance();
	m_writer->setPlugin(m_ui->selectedWriter);

	// check all input file for consistency
	bool error = false;
	for (auto item : m_ui->inputs)	{
		AwFileIOPlugin *plugin = item.first;
		AwFileIO *fr = plugin->newInstance();
		fr->setPlugin(plugin);
		QString file = item.second;
		sendMessage(QString("Checking file %1").arg(file));
		if (fr->openFile(file) != AwFileIO::NoError) {
			error = true;
			fr->plugin()->deleteInstance(fr);
			break;
		}
		sendMessage(QString("Ok"));
		m_readers << fr;
	}
	if (error) {
		sendMessage("Some input files cannot be open for reading. Aborted.");
		while (!m_readers.isEmpty()) {
			AwFileIO *r = m_readers.takeFirst();
			r->plugin()->deleteInstance(r);
		}
		return;
	}

	// check for consistency through input files (sampling rate, channel names)
	float sr = m_readers.first()->infos.channels().first()->samplingRate();
	QStringList labels = AwChannel::getLabels(m_readers.first()->infos.channels());
	int count = m_readers.first()->infos.channels().size();

	for (auto r : m_readers) {
		float tmp = r->infos.channels().first()->samplingRate();
		// check number of channels
		if (r->infos.channels().size() != count) {
			error = true;
			sendMessage(QString("The number of channels is different between files."));
			break;
		}
 		if (sr != tmp) {
			error = true;
			sendMessage(QString("Sampling rates are not consistent between files"));
			break;
		}
		QStringList tmpLabels = AwChannel::getLabels(r->infos.channels());
		for (auto l : tmpLabels) {
			if (!labels.contains(l)) {
				error = true;
				sendMessage(QString("Electrode names do not match."));
				break;
			}
		}
	}

	if (error) {
		while (!m_readers.isEmpty()) {
			AwFileIO *r = m_readers.takeFirst();
			r->plugin()->deleteInstance(r);
		}
		return;
	}

	// compute the total duration
	float duration = 0.;
	for (auto r : m_readers) 
		duration += r->infos.totalDuration();

	// gather all markers from the input files and reorder them for the expected appended one.
	AwMarkerList outputMarkers = AwMarker::duplicate(m_readers.first()->infos.blocks().first()->markers());
	float time_offset = m_readers.first()->infos.totalDuration();
	
	for (int i = 1; i < m_readers.size(); i++) {
		outputMarkers << new AwMarker("File Junction", time_offset);
		AwFileIO *r = m_readers.at(i);
		AwMarkerList temp = AwMarker::duplicate(r->infos.blocks().first()->markers());
		for (auto m : temp) 
			m->setStart(m->start() + time_offset);
		time_offset += r->infos.totalDuration();
		outputMarkers += temp;
	}
	
	// prepare channels for the writer
	AwChannelList sourceChannels = AwChannel::duplicateChannels(m_readers.first()->infos.channels());
	m_writer->infos.setChannels(sourceChannels);
	AwBlock *block = m_writer->infos.newBlock();
	block->setDuration(duration);
	block->setSamples((qint64)floor(duration * sr));
	block->setMarkers(outputMarkers);

	if (m_writer->createFile(m_ui->outputFile) != AwFileIO::NoError) {
		sendMessage(m_writer->errorMessage());
		while (!m_readers.isEmpty()) {
			AwFileIO *r = m_readers.takeFirst();
			r->plugin()->deleteInstance(r);
		}
		return;
	}

	// read input files using chunks of 5 minutes 
	const float chunkDuration = 300.;
	count = 0;
	for (auto r : m_readers) {
		float left = r->infos.totalDuration();
		float duration = std::min(r->infos.totalDuration(), chunkDuration);
		QString file = m_ui->inputs.at(count++).second;
		while (left > 0) {
			sendMessage(QString("Reading data from file %1...").arg(file));
			r->readDataFromChannels(0, duration, sourceChannels);
			sendMessage("Done.");
			left -= duration;
			sendMessage(QString("Writing data to %1...").arg(m_ui->outputFile));
			m_writer->writeData(&sourceChannels);
			sendMessage("Done.");
		}
	}

	m_writer->cleanUpAndClose();

	while (!m_readers.isEmpty()) {
		AwFileIO *r = m_readers.takeFirst();
		r->plugin()->deleteInstance(r);
	}
	m_ui->selectedWriter->deleteInstance(m_writer);
}
