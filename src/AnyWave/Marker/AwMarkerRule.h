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
#ifndef AWMARKERRULE_H
#define AWMARKERRULE_H

#include <QObject>
#include <AwMarker.h>
class AwMarkerRuleElement;


class AwMarkerRule : public QObject
{
	Q_OBJECT

public:
	AwMarkerRule(QObject *parent = 0);
	~AwMarkerRule();
	
	/** Apply the rule on the marker's list passed as parameter **/
	AwSharedMarkerList applyRule(const AwSharedMarkerList& list);
	inline void setName(const QString& name) { m_name = name; }
	inline QList<AwMarkerRuleElement *>& elements() { return m_elements; }
	inline QString& name() { return m_name; }
	bool isValid() { checkIfValid(); return m_isValid; }

	bool load();
	bool save();
	bool exists();
	void removeElement(AwMarkerRuleElement *elem);
	static void remove(const QString& name);
public slots:
	void addElement(AwMarkerRuleElement *elem);
protected:
	void clean();
	void checkIfValid();

	bool m_isValid;
	QString m_name;
	QList<AwMarkerRuleElement *> m_elements;
};

typedef QList<AwMarkerRule *> AwMarkerRulesList;

#endif // AWMARKERRULE_H
