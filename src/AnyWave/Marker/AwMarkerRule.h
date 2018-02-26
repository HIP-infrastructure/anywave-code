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
	AwMarkerList applyRule(const AwMarkerList& list);
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
