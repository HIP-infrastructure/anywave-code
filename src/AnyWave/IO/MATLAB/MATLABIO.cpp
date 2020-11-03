/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "MATLABIO.h"
#include <QDir>
#include <math/AwMath.h>

MATLABIOPlugin::MATLABIOPlugin() : AwFileIOPlugin()
{
	name = QString("MATLAB Output Plugin");
	description = QString(tr("Export data to MATLAB file"));
	version = QString("1.0");
	fileExtension = ".mat";
	m_flags = FileIO::HasExtension| FileIO::CanWrite;
}

MATLABIO::MATLABIO(const QString& path) : AwFileIO(path)
{

}


qint64 MATLABIO::writeData(QList<AwChannel*> *channels)
{
	if (channels->isEmpty())
		return -1;

	fmat data = AwMath::channelsToFMat(*channels);
	m_file.writeMatrix("data", data);
	return 0;
}


AwFileIO::FileStatus MATLABIO::createFile(const QString &path, int flags)
{
	QString fullPath = path;
	if (!path.contains(plugin()->fileExtension))
		fullPath = path + plugin()->fileExtension;
	if (m_file.create(fullPath) != 0) {
		m_error = QString("Could no create the file");
		return FileStatus::FileAccess;
	}
	if (infos.channelsCount() == 0) {
		m_error = QString("No channels to write");
		return AwFileIO::WrongParameter;
	}
	float sampleRate = infos.channels().first()->samplingRate();

	// write header variables
	m_file.writeScalar("sr", sampleRate);
	QStringList labels = AwChannel::getLabels(infos.channels(), true);
	m_file.writeStringCellArray("channels", labels);
	m_file.writeScalar("duration", infos.totalDuration());
	m_file.writeScalar("samples", infos.totalSamples());
	// build cell array for channel types
	QStringList types;
	for (auto c : infos.channels())
		types << AwChannel::typeToString(c->type());

	m_file.writeStringCellArray("channel_types", types);

	// Write markers if any
	auto markers = infos.blocks().first()->markers();
	if (!markers.isEmpty()) { // we consider a continous file (only one block)
		QStringList labels = AwMarker::getAllLabels(markers);

		QVector<float> values(labels.size());
		QVector<float> position(labels.size());
		QVector<float> duration(labels.size());
		int index = 0;
		for (auto m : markers) {
			values[index] = m->value();
			position[index] = m->start();
			duration[index++] = m->duration();
		}
		m_file.writeStringCellArray("markers", labels);
		m_file.writeVec("marker_positions", position);
		m_file.writeVec("marker_durations", duration);
		m_file.writeVec("marker_values", values);
	}
	return AwFileIO::NoError;
}


void MATLABIO::cleanUpAndClose()
{
	AwFileIO::cleanUpAndClose();
	m_file.close();
}




