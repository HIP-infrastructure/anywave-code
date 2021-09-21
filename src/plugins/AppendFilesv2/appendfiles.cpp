#include "appendfiles.h"
#include <QtCore/qfileinfo.h>
#include <math.h>

AppendFilesPlugin::AppendFilesPlugin()
{
	type = AwProcessPlugin::Background;
	name = "Append Files";
	description = tr("Concatenate files.");
	version = "1.0.0";
	category = "Process:File Operation:Append Files";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData| Aw::ProcessFlags::ProcessHasInputUi);
	m_helpUrl = "Append Files::File Operations::https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/plugin_append_files";
}

AppendFiles::AppendFiles()
{
	setInputFlags(Aw::ProcessIO::GetReaderPlugins|Aw::ProcessIO::GetWriterPlugins|Aw::ProcessIO::GetAllMarkers);
}

AppendFiles::~AppendFiles()
{
	delete m_ui;
}

bool AppendFiles::showUi()
{
	for (auto writer : pdi.input.writers)
		m_writers[writer->name] = writer;
	
	m_ui = new settings(pdi.input.readers, m_writers.keys());

	if (m_ui->exec() == QDialog::Accepted)
		return true;
	return false;
}

void AppendFiles::run()
{
	m_writer = QSharedPointer<AwFileIO>(m_writers.value(m_ui->m_selectedWriter)->newInstance());
	int channels = 0;
	float sr = 0.;
	QStringList labels;
	QStringList files;
	QList<QSharedPointer<AwFileIO>> readers;
	for (int i = 0; i < m_ui->files.size(); i++) {
		auto file = m_ui->files.at(i);
		auto reader = m_ui->readers.at(i);
		sendMessage(QString("Processing file %1").arg(file));
		bool channelsChecked = false, srChecked = false, labelsChecked = false;
		if (reader->openFile(file) == AwFileIO::NoError) {
				sendMessage("File can be open.");
				sendMessage(QString("Channels: %1").arg(reader->infos.channelsCount()));
				sendMessage(QString("Sampling rate:%1").arg(reader->infos.channels().first()->samplingRate()));
				if (channels == 0) {
					channels = reader->infos.channelsCount();
					channelsChecked = true;
				}
				else
					channelsChecked = channels == reader->infos.channelsCount();
				if (sr == 0.) {
					sr = reader->infos.channels().first()->samplingRate();
					srChecked = true;
				}
				else
					srChecked = sr == reader->infos.channels().first()->samplingRate();
				QStringList l = AwChannel::getLabels(reader->infos.channels());
				if (labels.isEmpty()) {
					labels = l;
					labelsChecked = true;
					labels.sort();
				}
				else {
					l.sort();
					labelsChecked = l.size() == labels.size();
					if (labelsChecked) {
						bool ok = true;
						for (int i = 0; i < l.size(); i++) {
							QString l1 = l.at(i);
							QString l2 = labels.at(i);
							if (l1 != l2) {
								ok = false;
								break;
							}
						}
						labelsChecked = ok == true;
					}
				}
				if (channelsChecked && srChecked && labelsChecked) {
					sendMessage("File is ok.");
					files << file;
					readers << reader;
				}
				else
					sendMessage("The file is not compatible with previous ones. Skipped");
				reader->cleanUpAndClose();
		}
		else {
			sendMessage("The file could not be open. skipped.");
		}
	}

	if (files.size() < 2) {
		sendMessage("ERROR: at least 2 files are required as input.");
		return;
	}

	AwMarkerList outputMarkers;
	float time_offset;
	float duration = 0;
	AwChannelList sourceChannels;
	for (int i = 0; i < files.size(); i++) {
		auto file = files.at(i);
		auto reader = readers.at(i);
		reader->openFile(file);
		auto tmp = AwMarker::duplicate(reader->infos.blocks().first()->markers());
		if (i == 0) {
			time_offset = reader->infos.totalDuration();
			outputMarkers = tmp;
			sourceChannels = AwChannel::duplicateChannels(reader->infos.channels());
			m_writer->infos.setChannels(sourceChannels);
			sr = reader->infos.channels().first()->samplingRate();
		}
		else {
			outputMarkers << new AwMarker("File Junction", time_offset);
			for (auto m : tmp)
				m->setStart(m->start() + time_offset);
			time_offset += reader->infos.totalDuration();
			outputMarkers += tmp;
		}
		duration += reader->infos.totalDuration();
		reader->cleanUpAndClose();
	}
	AwBlock* block = m_writer->infos.newBlock();
	block->setDuration(duration);
	block->setSamples((qint64)floor((double)duration* (double)sr));
	block->setMarkers(outputMarkers);
	if (m_writer->createFile(m_ui->outputFile) != AwFileIO::NoError) {
		sendMessage(m_writer->errorMessage());
		return;
	}
	// read input files using chunks of 5 minutes 
	const float chunkDuration = 300.;
	for (int i = 0; i < m_ui->files.size(); i++) {
		auto file = m_ui->files.at(i);
		auto reader = m_ui->readers.at(i);
		reader->openFile(file);
		float left = reader->infos.totalDuration();
		float duration = std::min(reader->infos.totalDuration(), chunkDuration);
		float position = 0;
		while (left > 0) {
			sendMessage(QString("Reading data from file %1...").arg(file));
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
}
