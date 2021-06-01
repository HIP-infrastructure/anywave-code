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
#include "AwJSON.h"
#include <QFile>
using namespace aw::json;
#include <AwException.h>

QJsonDocument loadJSONFile(const QString& path)
{
	QFile loadFile(path);
	QString origin = "loadJSONFile()";
	if (loadFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson(loadFile.readAll(), &error);
		loadFile.close();
		if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
			throw AwException(QString("Json error: %1.").arg(error.errorString()), origin);
			return QJsonDocument();
		}
		return doc;
	}
	throw AwException(QString("Failed to open %1").arg(path), origin);
	return QJsonDocument();
}