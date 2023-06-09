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
#include "AwMarkerExporter.h"

AwMarkerExporterPlugin::AwMarkerExporterPlugin() : AwProcessPlugin()
{
	name = QString("Markers Exporter");
	description = QString(tr("Export data to a file"));
	version = "1.0";
	type = AwProcessPlugin::Background;
	setFlags(Aw::ProcessFlags::ProcessSkipInputCheck);
}

AwMarkerExporter::AwMarkerExporter() : AwProcess()
{
	// no input checking for Process Manager
	//m_flags |= Aw::ProcessFlags::ProcessSkipInputCheck;
	concatenate = false;
	m_plugin = new AwMarkerExporterPlugin;
}

AwMarkerExporter::~AwMarkerExporter()
{
	delete m_plugin;
}

void AwMarkerExporter::run()
{
	if (pdi.input.markers().isEmpty()) {
		sendMessage("No markers selected as input.");
		return;
	}
	fmat matrix;
	fvec positions;	// position in second from begining of original file of each epochs
	int status;
	// create the file, either MATLAB or ADES
	AwChannelList inputChannels = pdi.input.channels();
	if (matlabFormat) {
		if (m_matlabFile.create(outputFile) != 0) {
			sendMessage(m_matlabFile.error());
			return;
		}
		if (m_matlabFile.writeScalar("sr", pdi.input.channels().first()->samplingRate()) != 0) {
			sendMessage(m_matlabFile.error());
			return;
		}
		
		QStringList labels = AwChannel::getLabels(inputChannels);
		if (m_matlabFile.writeStringCellArray("electrodes", labels) != 0) {
			sendMessage(m_matlabFile.error());
			return;
		}
		positions = fvec(pdi.input.markers().size());
	}
	else { // ADES
		m_adesFile.setPlugin(&m_adesPlugin);
	}
	int count = 1;
	for (auto m : pdi.input.markers()) {
		sendMessage("Getting data...");
		requestData(&inputChannels, m);
		sendMessage("Data loaded.");
		matrix = AwMath::channelsToFMat(inputChannels);
		sendMessage("Saving to file...");
		if (matlabFormat) { // TRANSPOSE the matrix as MATLAB has a column major ordering
			positions(count - 1) = m->start();
			if (m_matlabFile.writeMatrix(QString("epoch%1").arg(count++), matrix) != 0) {
				sendMessage(m_matlabFile.error());
				return;
			}
		} 
		else {
			m_adesFile.infos.setChannels(inputChannels);
			AwBlock *block = m_adesFile.infos.newBlock();
			block->setDuration(m->duration());
			block->setSamples(pdi.input.channels().first()->dataSize());
			QString path = outputFile.remove(".ades");

			if (m_adesFile.createFile(QString("%1%2.ades").arg(path).arg(count++)) != AwFileIO::NoError) {
				sendMessage(m_adesFile.errorMessage());
				return;
			}
			m_adesFile.writeData(&inputChannels);
			m_adesFile.cleanUpAndClose();
		}
		sendMessage("Done.");
		if (isAborted()) {
			return;
		}
	}
	if (matlabFormat) {
		if (m_matlabFile.writeVec(QString("positions"), positions) != 0) {
			sendMessage(m_matlabFile.error());
		}
	}
}