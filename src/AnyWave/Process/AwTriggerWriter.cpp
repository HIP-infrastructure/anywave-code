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
#include "AwTriggerWriter.h"
#include "Widgets/AwSelectTriggerChannelDialog.h"
#include "Plugin/AwPluginManager.h"
#include <AwKeys.h>
#include <cmath>

AwTriggerWriterPlugin::AwTriggerWriterPlugin()
{
	name = QString("Trigger_Writer");
	description = QString("Writes marker values to a trigger channel");
	version = "1.0.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::PluginIsHidden | Aw::ProcessFlags::ProcessHasInputUi | Aw::ProcessFlags::CanRunFromCommandLine | Aw::ProcessFlags::ProcessDoesntRequireData);
	m_settings[keys::json_batch] = QString("{ \"parameters\": [ \"meg_file\", \"duration\"]}");
}

AwTriggerWriter::AwTriggerWriter() : AwProcess()
{
	
}

bool AwTriggerWriter::showUi()
{
	AwSelectTriggerChannelDialog dlg(pdi.input.reader()->triggerChannels());
	if (dlg.exec() == QDialog::Accepted) {
		m_triggerChannel = dlg.selectedChannel();
		return true;
	}
	return false;
}

void AwTriggerWriter::run()
{
	sendMessage("Writing values...");
	if (pdi.input.reader() == nullptr) {
		sendMessage("No data file opened. Aborted.");
		return;
	}
	int count = pdi.input.reader()->writeTriggerChannel(m_triggerChannel, pdi.input.markers());
	sendMessage(QString("%1 values written.").arg(count));
}

void AwTriggerWriter::runFromCommandLine()
{
	auto& args  = pdi.input.settings;
	if (!args.contains("meg_file")) {
		sendMessage("Error: --meg_file option missing.");
		return;
	}
	QString megFile = args.value("meg_file").toString();
	auto readerInstance = AwPluginManager::getInstance()->getReaderToOpenFile(megFile);
	if (readerInstance == nullptr) {
		sendMessage("Error: could not open meg file. No plugin found.");
		return;
	}
	// check if markers are present
	if (pdi.input.markers().isEmpty()) {
		sendMessage("Error: no markers loaded, pleasy specify --marker_file option");
		return;
	}

	QSharedPointer<AwFileIO> reader = QSharedPointer<AwFileIO>(readerInstance);
	if (reader->openFile(megFile) != AwFileIO::NoError) {
		sendMessage(QString("Error: %1").arg(reader->errorMessage()));
		return;
	}
	QFile file(megFile);
	if (!file.open(QIODevice::ReadWrite)) {
		sendMessage("Error: could not open meg_file for writting");
		return;
	}
	// get data format
	QString format = reader->settings().value("data_format").toString();
	if (format.isEmpty()) {
		sendMessage("Error: data format information is missing.");
		reader->cleanUpAndClose();
		file.close();
		return;
	}
	// get index of channel trigger
	int indexTrigger = reader->infos.indexOfChannel("TRIGGER");
	auto nChannels = reader->infos.channelsCount();
	if (indexTrigger == -1) {
		sendMessage("Error: could not find channel TRIGGER in data");
		file.close();
		reader->cleanUpAndClose();
		return;
	}
	sendMessage("Erasing trigger channel...");
	reader->clearTriggerChannels({ "TRIGGER" });
	sendMessage("OK");
	qint64 headerPos = (qint64)std::floor(reader->settings().value("header_position").toDouble());
	// now get the markers
	auto markers = pdi.input.markers();
	std::sort(markers.begin(), markers.end());
	float sr = reader->settings().value("sampling_rate").toFloat();
	int dataSize;
	if (format == "float")
		dataSize = sizeof(float);
	else if (format == "double")
		dataSize = sizeof(double);
	else if (format == "short")
		dataSize = sizeof(qint16);
	else dataSize = sizeof(long);

	QDataStream stream(&file);
	stream.setVersion(QDataStream::Qt_4_4);
	qint64 duration = 20; // default to 20ms length
	if (args.contains("duration"))
		duration = args.value("duration").toInt();
	// convert duration to samples
	using namespace std;
	duration = floor((double)duration / 1000. * sr);   // duration is ms
	auto writeSamples = [=]<typename T>(T v, QFile& file, QDataStream& stream) {
		for (auto m : markers) {
			qint64 position = std::floor(m->start() * sr);
			T value = T(m->value());
			file.seek(nChannels * position * dataSize + indexTrigger * dataSize);
			for (auto i = 0; i < duration && position < headerPos;  i++) {
				position = file.pos();
				stream << value;
				position += nChannels * dataSize;
				file.seek(position);
			}
			sendMessage(QString("value:%1 written at:%2").arg(value).arg(m->start()));
		}
	};
	if (format == "float")
		writeSamples((float)1.0, file, stream);
	else if (format == "double")
		writeSamples((double)1.0, file, stream);
	else if (format == "short")
		writeSamples((qint16)1, file, stream);
	else
		writeSamples((int)1, file, stream);

	file.close();
	reader->cleanUpAndClose();
	sendMessage("All done.");
}