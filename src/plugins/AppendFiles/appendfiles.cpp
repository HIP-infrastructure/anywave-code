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
	setFlags(Aw::ProcessDontRequireData);
}

AppendFiles::AppendFiles()
{
	setFlags(Aw::ProcessHasInputUi|Aw::ProcessIsScriptable);
	pdi.addInputParameter(Aw::GetReaderPlugins, "1-n");
	pdi.addInputParameter(Aw::GetWriterPlugins, "1-n");
	pdi.addInputParameter(Aw::GetAllMarkers, "0-n");
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

	int count = 1;
	QStringList labels;
	AwMarkerList markers;
	float sr;	// sampling rate
	AwFileIOPlugin *plugin = m_ui->inputs.first().first;
	AwFileIO *fr = plugin->newInstance();
	QString file = m_ui->inputs.first().second;
	fr->setPlugin(plugin);
	emit progressChanged("Getting information of file " + file);
	m_readers << fr;
	
	if (fr->openFile(file) != AwFileIO::NoError) {
		error(QString("Failed to open %1").arg(file));
		return;
	}

	sr = fr->infos.channels().first()->samplingRate();
	markers = AwMarker::duplicate(fr->infos.blocks().first()->markers());
	labels = AwChannel::getLabels(fr->infos.channels());

	for (int i = 1; i < m_ui->inputs.size(); i++) {
		plugin = m_ui->inputs.at(i).first;
		fr = plugin->newInstance();
		fr->setPlugin(plugin);
		file =  m_ui->inputs.at(i).second;
		m_readers << fr;
		if (fr->openFile(file) != AwFileIO::NoError) {
			error(QString("Failed to open %1").arg(file));
			return;
		}

		QStringList temp = AwChannel::getLabels(fr->infos.channels());
		if (temp.size() != labels.size()) { // files do not contain the same channels
			error(QString("File %1 is not compatible with the previous files: The number of channels differ.").arg(file));
			return;
		}
		else {
			foreach (QString s, temp) {
				if (!labels.contains(s)) {
					error(QString("File %1 is not compatible with the previous files: The channels' names differ.").arg(file));
					return;
				}
			}
		}
		if (sr != fr->infos.channels().first()->samplingRate()) {
			error(QString("File %1 is not compatible with the previous files: The sampling rate differ.").arg(file));
			return;
		}
	}

	// compute the total duration
	float duration = 0.;
	foreach(AwFileIO *r, m_readers) {
		duration += r->infos.totalDuration();
	}
	float time_offset = 0.;
	for (int i = 1; i < m_readers.size(); i++) {
		AwFileIO *r = m_readers.at(i);
		time_offset +=  m_readers.at(i - 1)->infos.totalDuration();
		AwMarkerList temp = AwMarker::duplicate(r->infos.blocks().first()->markers());
		// Insert a marker here at first position to mark the file jointure
		AwMarker *joint = new AwMarker();
		joint->setLabel("File junction");
		joint->setStart(time_offset);
		foreach (AwMarker *m, temp)  {
			m->setStart(m->start() + time_offset);
		}
		temp.insert(0, joint);
		markers += temp;
	}

	AwChannelList sourceChannels = AwChannel::duplicateChannels(m_readers.first()->infos.channels());
	m_writer->infos.setChannels(sourceChannels);
	AwMarker::sort(markers);
	AwBlock *block = m_writer->infos.newBlock();
	block->setDuration(duration);
	block->setSamples((qint64)floor(duration * sr));
	block->setMarkers(markers);
	emit progressChanged(tr("Creating output file..."));
	if (m_writer->createFile(m_ui->outputFile) != AwFileIO::NoError) {
		error("Could not create the output file!");
		return;
	}
	foreach (AwFileIO *r, m_readers) {
		emit progressChanged(QString("Appending data of file %1...").arg(r->infos.fileName()));
		r->readDataFromChannels(0, fr->infos.totalDuration(), sourceChannels);
		m_writer->writeData(&sourceChannels);
	}
	m_writer->cleanUpAndClose();
	// destroy source channels
	while (!sourceChannels.isEmpty())
		delete sourceChannels.takeFirst();
	while (!markers.isEmpty())
		delete markers.takeFirst();
	while (!m_readers.isEmpty()) {
		AwFileIO *r = m_readers.takeFirst();
		r->plugin()->deleteInstance(r);
	}
	m_ui->selectedWriter->deleteInstance(m_writer);
}
