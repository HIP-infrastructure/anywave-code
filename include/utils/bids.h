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
#pragma once
#include <AwGlobal.h>
#include <QVariant>

namespace AwUtilities // put utility functions inside a namespace
{
	namespace bids {
		QString AW_UTILITIES_EXPORT getSubjectID(const QString& data);
		QString AW_UTILITIES_EXPORT getSessionLabel(const QString& data);
		QStringList AW_UTILITIES_EXPORT getTsvColumns(const QString& tsvFile);
		QVariantHash AW_UTILITIES_EXPORT loadTsv(const QString& tsvFile);
		QMap<int, QString> AW_UTILITIES_EXPORT columnsFromLine(const QString& line);
		QString AW_UTILITIES_EXPORT removeBidsKey(const QString& key, const QString& name);
		QMap<QString, QString> AW_UTILITIES_EXPORT getKeysValue(const QString& line);
	}
}
