#include "mergefiles.h"
#include <QMessageBox>

MergeFilePlugin::MergeFilePlugin()
{
	type = AwProcessPlugin::Background;
	name = "Merge Files to ADES";
	description = tr("Merge two files in one (ADES).");
	category = "Process:File Operation:Merge Files";
	setFlags(Aw::ProcessFlags::ProcessDoesntRequireData);
}

MergeFile::MergeFile()
{
	setFlags(Aw::ProcessFlags::ProcessHasInputUi|Aw::ProcessFlags::ProcessIsScriptable);
	pdi.addInputParameter(Aw::ProcessInput::GetReaderPlugins, "1-n");
	pdi.addInputParameter(Aw::ProcessInput::GetWriterPlugins, "1-n");
	m_adesPlugin = NULL;
}

MergeFile::~MergeFile()
{
	delete m_ui;
}


void MergeFile::destroyFileReader(AwFileIO *r)
{
	r->cleanUpAndClose();
	r->plugin()->deleteInstance(r);
}

void MergeFile::run()
{
	emit progressChanged(tr("Checking input files..."));
	AwFileIO *fr1 = m_ui->file1Plugin->newInstance();
	fr1->setPlugin(m_ui->file1Plugin);
	if (fr1->openFile(m_ui->file1) != AwFileIO::NoError) {
		emit progressChanged(tr("Failed to open ") + m_ui->file1);
		sendMessage(QString("error is %1").arg(fr1->errorMessage()));
		destroyFileReader(fr1);
		return;
	}
	AwFileIO *fr2 = m_ui->file2Plugin->newInstance();
	fr2->setPlugin(m_ui->file2Plugin);
	if (fr2->openFile(m_ui->file2) != AwFileIO::NoError) {
		emit progressChanged(tr("Failed to open ") + m_ui->file2);
		sendMessage(QString("error is %1").arg(fr2->errorMessage()));
		destroyFileReader(fr1);
		destroyFileReader(fr2);
		return;
	}
	emit progressChanged(tr("Ok."));
	emit progressChanged(tr("Checking the number of samples of both files..."));
	if (fr1->infos.totalSamples() != fr2->infos.totalSamples()) {
		emit progressChanged(tr("The files don't have the same number of samples."));
		destroyFileReader(fr1);
		destroyFileReader(fr2);
		return;
	}
	emit progressChanged(tr("Ok."));
	emit progressChanged(tr("Checking for the electrodes names..."));
	AwChannelList channels1 = AwChannel::duplicateChannels(fr1->infos.channels());
	AwChannelList channels2 = AwChannel::duplicateChannels(fr2->infos.channels());
	QStringList labels1 = AwChannel::getLabels(channels1);
	QStringList labels2 = AwChannel::getLabels(channels2);
	bool matchingLabel = false;
	foreach (QString s, labels2) {
		if (labels1.contains(s)) {
			matchingLabel = true;
			break;
		}
	}

	if (matchingLabel) {
		emit progressChanged(tr("One channel/electrode exists in both files. Stopping..."));
		destroyFileReader(fr1);
		destroyFileReader(fr2);
		return;
	}
	emit progressChanged(tr("Ok."));

	AwFileIO *writer = m_adesPlugin->newInstance();
    writer->setPlugin(m_adesPlugin);
#ifndef Q_OS_WIN
    AwChannelList mergedChannels = channels1 + channels2;
    writer->infos.setChannels(mergedChannels);
#else
	writer->infos.setChannels(channels1 + channels2);
#endif
    AwBlock *block = writer->infos.newBlock();
	block->setDuration(fr1->infos.totalDuration());
	block->setSamples(fr1->infos.totalSamples());
	block->setMarkers(fr1->infos.blocks().first()->markers() + fr2->infos.blocks().first()->markers());
	if (writer->createFile(m_ui->outputFile) != AwFileIO::NoError) {
		emit progressChanged(tr("Error creating output file!"));
		sendMessage(QString("error is %1").arg(writer->errorMessage()));
		destroyFileReader(fr1);
		destroyFileReader(fr2);
		return;
	}
	emit progressChanged(tr("Reading file1..."));
	fr1->readDataFromChannels(0, fr1->infos.totalDuration(), channels1);
	emit progressChanged(tr("Reading file2..."));
	fr2->readDataFromChannels(0, fr2->infos.totalDuration(), channels2);
	emit progressChanged(tr("Writing data..."));
	AwChannelList merged = channels1 + channels2;
	writer->writeData(&merged);
	emit progressChanged(tr("Ok."));
	writer->cleanUpAndClose();
	m_adesPlugin->deleteInstance(writer);
	destroyFileReader(fr1);
	destroyFileReader(fr2);
}


bool MergeFile::showUi()
{
	m_ui = new Settings(pdi.input.readers);

	if (m_ui->exec() == QDialog::Accepted) {
		// get ADES writer plugin
		foreach(AwFileIOPlugin *p, pdi.input.writers)  {
			if (p->name == "AnyWave ADES Format") {
				m_adesPlugin = p;
				break;
			}
		}
		if (m_adesPlugin == NULL) {
			QMessageBox::critical(0, tr("ADES plugin"), tr("ADES plugin is not present!"));
			return false;
		}
		return true;
	}
	return false;
}
