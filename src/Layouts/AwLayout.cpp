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
#include <layout/AwLayout.h>
#include <layout/AwLayoutManager.h>
#include <AwSensor.h>
#include <QFile>
#include <QTextStream>

AwLayout::AwLayout(const QString& name, int type, const QString& path)
{
	m_name = name;
	m_file = path;
	m_flags = type;
	m_status = AwLayout::Valid;
}

QString AwLayout::isMeshFileExists()
{
	QString dir = AwLayoutManager::instance()->workingDir();
	if (dir.isEmpty())
		return QString();
	QString path = QString("%1%2.mesh").arg(dir).arg(m_name);
	if (QFile::exists(path))
		return path;
	return QString();
}


int AwLayout::load()
{
	// path must be a file located in :/layouts in the ressource file.
	QFile file(":/layouts/" + m_file);
	QTextStream stream(&file);
	int nTokens =  is3D() ? 5 : 4;
	
	if (file.open(QIODevice::ReadOnly|QIODevice::Text))	{
		while (!stream.atEnd()) {
			QString line = stream.readLine();

			QStringList tokens = line.split(QRegExp("\\s+"));
			if (tokens.size() != nTokens) {
				file.close();
				m_status = AwLayout::FileError;
				return -1;
			}
			AwSensor *s = new AwSensor();
			QString label;
			if (is2D()) {
				s->setLabel(tokens.at(3).trimmed());
				s->setCoordinates(tokens.at(1).toDouble(), tokens.at(2).toDouble(), 0.);
				label = tokens.at(3).trimmed();
			}
			else {
				s->setLabel(tokens.at(4).trimmed());
				s->setCoordinates(tokens.at(1).toDouble(), tokens.at(2).toDouble(), tokens.at(3).toDouble());
				label = tokens.at(4).trimmed();
			}
			m_sensors << s;
			m_labels << label;
			m_sensorsByName.insert(label, s);
		}
	}
	file.close();
	return 0;
}