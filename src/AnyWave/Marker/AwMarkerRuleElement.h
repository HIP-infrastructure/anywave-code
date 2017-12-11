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

#ifndef AWMARKERRULEELEMENT_H
#define AWMARKERRULEELEMENT_H

#include <QObject>
#include <QVariant>


class AwMarkerRuleElement : public QObject
{
	Q_OBJECT

public:
	AwMarkerRuleElement(QObject *parent = 0);
	AwMarkerRuleElement(AwMarkerRuleElement *element);
	~AwMarkerRuleElement();

	enum type { RuleElementLabel, RuleElementCode, RuleElementDuration, RuleElementTargetChannel };
	enum condition { RuleConditionLesserThan = 1, RuleConditionDifferent = 2, RuleConditionGreaterThan = 4,
		RuleConditionEquals = 8, RuleConditionInRange = 16, RuleConditionContains = 32 };

	inline void setType(int type) { m_type = type; } // Element is of ONE type only
	inline void setCondition(int condition) { m_condition = condition; }
	void setValue(const QVariant& value) { m_value = value; }

	bool isValid() { checkIfValid(); return m_isValid; }
	inline int type() { return m_type; }
	inline int condition() { return m_condition; }
	inline bool isEqual() { return m_condition == RuleConditionEquals; }
	inline bool isDifferent() { return m_condition == RuleConditionDifferent; }
	inline bool isGreaterThan() { return m_condition == RuleConditionGreaterThan; }
	inline bool isLesserThan() { return m_condition == RuleConditionLesserThan; }
	inline bool isInRange() { return m_condition == RuleConditionInRange; }
	inline bool contains() { return m_condition == RuleConditionContains; }
//	inline bool isTargetingChannels() { return m_condition == RuleElementTargetChannel; }

	inline QVariant value() { return m_value; }

protected:
	void checkIfValid();

	int m_type;
	int m_condition;
	QVariant m_value;
	bool m_isValid;
};

#endif // AWMARKERRULEELEMENT_H
