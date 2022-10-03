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
#include <QVariantMap>
#include <QSharedPointer>

class AW_CORE_EXPORT AwEvent
{
public:
	AwEvent();
	enum EventType { StartProcessDetached, StartProcess, HighlightTimeSelection, ShowChannels, LoadICAMatFile, AddNewView };

	inline QVariantMap& data() { return m_data; }
	inline int id() { return m_id; }
	void setId(int id) { m_id = id; }
	void addValue(const QString& key, const QVariant& value) { m_data[key] = value; }
	void setData(const QVariantMap& map) { m_data = map; }
protected:
	int m_id;
	QVariantMap m_data;
};
Q_DECLARE_METATYPE(AwEvent);
Q_DECLARE_METATYPE(QSharedPointer<AwEvent>);

