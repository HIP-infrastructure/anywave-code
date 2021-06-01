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
#include "AwBIDSManager.h"
#include <QFile>
#include <QTextStream>

int AwBIDSManager::createEventsTsv(const QString& filePath, const AwMarkerList& markers)
{
	m_errorString.clear();
	if (markers.isEmpty())
		return -1;
	QFile file(filePath);
	QTextStream stream(&file);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		m_errorString = QString("Could not open %1 for writing.").arg(filePath);
		return -1;
	}
	// columns : onset duration trial_type
	stream << bids::tsv_event_onset << "\t" << bids::tsv_event_duration << "\t" << bids::tsv_event_trial_type << endl;
	for (auto m : markers) 
		stream << QString("%1").arg(m->start()) << "\t" << QString("%1").arg(m->duration()) << "\t" << m->label() << endl;
	file.close();
	return 0;
}