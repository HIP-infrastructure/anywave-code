#include "NI4DExporter.h"
#include <AwKeys.h>
#include <QDataStream>
#include "MEGstructs.h"
#include <QFileDialog>
#include <filter/AwFiltering.h>
#include <utils/endian.h>
#include <QMessageBox>
#include <QDir>
#include "Aw4DNIExportDialog.h"
#include <widget/AwWaitWidget.h>
#include <utils/bids.h>

Aw4DNIExporterPlugin::Aw4DNIExporterPlugin()
{
	name = QString("4DNIExporter");
	description = QString("Write 4DNI meg file content to a new file");
	category = "File:(4DNI MEG File Format) Write to a new file";
	version = "1.1.1";
	type = AwProcessPlugin::Background;
	// no input gui for now
	setFlags(Aw::ProcessFlags::ProcessHasInputUi);
}

Aw4DNIExporter::Aw4DNIExporter()
{
	setInputFlags(Aw::ProcessIO::GetReaderPlugins);
	setInputModifiers(Aw::ProcessIO::modifiers::IgnoreChannelSelection);
}


bool Aw4DNIExporter::showUi()
{
	// detect bids
	QString filePath = pdi.input.settings.value(keys::bids_file_path).toString();
	bool isBids = !filePath.isEmpty();

	if (!isBids) {
		m_outputFileName = QFileDialog::getSaveFileName(nullptr, "Select output file",
			pdi.input.settings.value(keys::data_path).toString() + "_new");
		if (m_outputFileName.size()) {
			m_outputFileName = QDir::toNativeSeparators(m_outputFileName);
			auto currentFile = QDir::toNativeSeparators(pdi.input.settings.value(keys::data_path).toString());

			if (m_outputFileName.toLower() == currentFile.toLower()) {
				QMessageBox::critical(nullptr, "Invalid file", "Do not overwrite the current open meg file.");
				return false;
			}
			return true;
		}
		return false;
	}
	else {
		Aw4DNIExportDialog dlg;
		if (dlg.exec() == QDialog::Accepted) {
			QFileInfo fi(filePath);
			auto bidsDir = fi.absolutePath();
			bidsDir = bidsDir.remove("_meg");
			auto map = AwUtilities::bids::getKeysValue(bidsDir);
			if (map.contains("proc")) {
				bidsDir = bidsDir.remove(QString("_proc-%1").arg(map.value("proc")));

			}
			bidsDir += QString("_proc-%1_meg").arg(dlg.selectedModification.remove(' '));
			// create the new dir
			QDir dir(bidsDir);
			dir.mkpath(bidsDir);  // bidsDir is the destination folder
			// moving files
			AwWaitWidget wait("Copying files...");
			connect(this, &Aw4DNIExporter::finished, &wait, &AwWaitWidget::accept);

			auto copyLambda = [this](const QString& source, const QString& dest, const QString& proc) {
				QDir sDir(source);
				QString srcBaseJsonFileName = source;
				srcBaseJsonFileName = srcBaseJsonFileName.remove("_meg");
				QString destBaseJsonFileName = dest;
				destBaseJsonFileName = destBaseJsonFileName.remove("_meg");
				// copy sidecar files
				QFile::copy(QString("%1_channels.tsv").arg(srcBaseJsonFileName), QString("%1_channels.tsv").arg(destBaseJsonFileName));
				QFile::copy(QString("%1_coordsystem.json").arg(srcBaseJsonFileName), QString("%1_coordsystem.json").arg(destBaseJsonFileName));
				QFile::copy(QString("%1_meg.json").arg(srcBaseJsonFileName), QString("%1_meg.json").arg(destBaseJsonFileName));
				QFile::copy(QString("%1_events.tsv").arg(srcBaseJsonFileName), QString("%1_events.tsv").arg(destBaseJsonFileName));
				QFile::copy(QString("%1_headshape.pos").arg(srcBaseJsonFileName), QString("%1_headshape.pos").arg(destBaseJsonFileName));

				// copy .mtg, .bad, .mrk !!!
				//QString  mrkFilePath = pdi.input.settings.value(keys::marker_file).toString();  // we use marker_file to get the deratives path for sidecar files.
				QString bidsDerivativesSource = pdi.input.settings.value(keys::bids_user_derivatives_folder).toString();
				auto map = AwUtilities::bids::getKeysValue(bidsDerivativesSource);

				QString bidsDerivativesDest = bidsDerivativesSource;
				bidsDerivativesDest = bidsDerivativesDest.remove("_meg");
				if (map.contains("proc")) {
					bidsDerivativesDest = bidsDerivativesDest.remove(QString("_proc-%1").arg(map.value("proc")));
				}

				bidsDerivativesDest = bidsDerivativesDest + proc;
				QDir destDerivativesDir(bidsDerivativesDest);
				destDerivativesDir.mkpath(bidsDerivativesDest);  // new derivatives path
				QDir sourceDerivativesDir(bidsDerivativesSource);
				for (const auto& file : sourceDerivativesDir.entryList(QDir::Files)) {
					QString srcFile = QDir::toNativeSeparators(QString("%1/%2").arg(bidsDerivativesSource).arg(file));
					QString destFile = QDir::toNativeSeparators(QString("%1/%2").arg(bidsDerivativesDest).arg(file));
					QFile::copy(srcFile, destFile);
				}
				// end copying .bad .mtg .mrk

				auto files = sDir.entryList(QDir::Files);
				for (const auto& file : files) {
					QString srcFile = QString("%1/%2").arg(source).arg(file);
					QString destFile = QString("%1/%2").arg(dest).arg(file);
					QFile::copy(srcFile, destFile);
				}
				emit finished();
			};
			wait.run(copyLambda, fi.absolutePath(), bidsDir, QString("_proc-%1_meg").arg(dlg.selectedModification));
			m_outputFileName = QString("%1/%2").arg(bidsDir).arg(fi.fileName());
			return true;
		}
	}
	return false;
}


void Aw4DNIExporter::run()
{
	AwFileIO* reader = pdi.input.reader();
	// make sure the current file open is 4DNI MEG file
	if (reader->plugin()->name != "4DNI Reader") {
		sendMessage("ERROR: the current file is not a 4DNI MEG file.");
		return;
	}

	auto args = pdi.input.settings;
	QString megFile = pdi.input.settings.value(keys::data_path).toString();

	// test if can create the file
	QFile outputFile(m_outputFileName);
	QDataStream streamOutput(&outputFile);
	streamOutput.setVersion(QDataStream::Qt_4_4);
	if (outputFile.open(QIODevice::WriteOnly)) {
		outputFile.close();
		QFile::remove(m_outputFileName);
	}
	else {
		sendMessage("ERROR: Could not write to the folder.");
		return;
	}

	QFile inputFile(megFile);
	QDataStream streamInput(&inputFile);
	streamInput.setVersion(QDataStream::Qt_4_4);
	if (!inputFile.open(QIODevice::ReadOnly)) {
		sendMessage("ERROR: could not open again the current meg file for reading.");
		return;
	}

	qint64 headerPos;
	inputFile.seek(inputFile.size() - 8);
	streamInput >> headerPos;
	qint64 headerSize = (inputFile.size() - 8) - headerPos;
	inputFile.seek(headerPos);
	alignFilePointer(inputFile);
	dftk_header_data hdr;
	dftk_channel_ref_data channel_ref;
	quint32 nChannels;
	streamInput >> hdr.version;
	streamInput.readRawData((char*)&hdr.file_type, 5);
	streamInput.skipRawData(1);
	streamInput >> hdr.data_format;
	streamInput >> hdr.acq_mode;
	streamInput >> hdr.total_epochs;
	streamInput >> hdr.input_epochs;
	streamInput >> hdr.total_events;
	streamInput >> hdr.total_fixed_events;
	streamInput >> hdr.sample_period;
	streamInput.readRawData((char*)&hdr.xaxis_label, 16);
	streamInput >> hdr.total_processes;
	streamInput >> hdr.total_chans;
	nChannels = hdr.total_chans;
	// ready to create output file
	if (!outputFile.open(QIODevice::WriteOnly)) {
		sendMessage(QString("ERROR: could not create file %1").arg(m_outputFileName));
		inputFile.close();
		return;
	}

	sendMessage("Reading all data...");
	auto channels = reader->infos.channels();
	AwChannelList chans = AwChannel::duplicateChannels(channels);

	requestData(&chans, 0., -1., true);
	sendMessage("Data read.");
	sendMessage("Filtering...");
	pdi.input.filterSettings.apply(chans);
	AwFiltering::filter(chans);
	sendMessage("Filtering done.");
	const qint64 chunkDuration = 300000; // 300 000 samples buffer
	qint64 totalSamples = chans.first()->dataSize();
	// create a multiplexed data buffer of 30s to write out the filtered data
	float* buffer = new float[chunkDuration * nChannels];
	qint64 position = 0;
	while (totalSamples) {
		qint64 duration = std::min(chunkDuration, totalSamples);
		for (auto i = 0; i < duration; i++) {
			for (auto j = 0; j < nChannels; j++) {
				auto chan = chans.at(j);
				float value = chan->data()[i + position];
				if (chan->isMEG() || chan->isReference())
					value *= 1e-12;
				else if (chan->isEEG() || chan->isECG() || chan->isEMG() || chan->isOther())
					value *= 1e-6;
				buffer[j + i * nChannels] = value;
			}
		}
		position += duration;
#if defined(_OPENMP)
#pragma omp parallel for
#endif
		for (auto i = 0; i < duration * nChannels; i++) {
			float& tmp = buffer[i];
			AwUtilities::endianness::swapEndian<float>(tmp);
		}
		sendMessage("Writing data...");
		streamOutput.writeRawData((char*)buffer, duration * nChannels * sizeof(float));
		sendMessage("Writing done.");
		totalSamples -= duration;
	}
	delete[] buffer;
	for (auto chan : chans)
		chan->clearData();
	// copy old header
	qint64 bytes = offsetFilePointer(outputFile);
	if (bytes) {
		char* dummy = new char[bytes];
		memset(dummy, 0, bytes);
		streamOutput.writeRawData(dummy, bytes);
		delete[] dummy;
	}
	qint64 newHeaderPos = outputFile.pos();
	char* header = new char[headerSize];
	inputFile.seek(headerPos);
	alignFilePointer(inputFile);
	streamInput.readRawData(header, headerSize);
	streamOutput.writeRawData(header, headerSize);
	streamOutput << newHeaderPos;
	delete[] header;
	outputFile.seek(newHeaderPos);
	alignFilePointer(outputFile);
	outputFile.seek(outputFile.pos() + 8);
	streamOutput << (qint16)3;
	outputFile.flush();
	outputFile.close();
	inputFile.close();

	// copy .bad file if exists
	auto badFile = pdi.input.settings.value(keys::bad_file).toString();
	if (QFile::exists(badFile)) {
		QString destBadFile = m_outputFileName + ".bad";
		QFile::copy(badFile, destBadFile);
	}
	// copy mtg file
	auto mtgFile = pdi.input.settings.value(keys::montage_file).toString();
	if (QFile::exists(mtgFile)) {
		QString destMtgFile = m_outputFileName + ".mtg";
		QFile::copy(mtgFile, destMtgFile);
	}
	// copy display file
	auto dispFile = pdi.input.settings.value(keys::disp_file).toString();
	if (QFile::exists(dispFile)) {
		QString destDispFile = m_outputFileName + ".display";
		QFile::copy(dispFile, destDispFile);
	}
	qDeleteAll(chans);
	sendMessage("All done.");
}


// 
// base on current file position, returns the number of bytes to add to get an aligned position to 8 bytes.
void Aw4DNIExporter::alignFilePointer(QFile& file)
{
	qint64 current = file.pos();
	qint64 offset = 0;
	if (current % 8 != 0) {
		offset = 8 - current % 8;
		file.seek(current + offset);
	}
}


// 
// base on current file position, returns the number of bytes to add to get an aligned position to 8 bytes.
qint64 Aw4DNIExporter::offsetFilePointer(const QFile& file)
{
	qint64 current = file.pos();
	qint64 offset = 0;
	if (current % 8 != 0) {
		offset = 8 - current % 8;

	}
	return offset;
}


