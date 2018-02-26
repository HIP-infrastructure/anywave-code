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


#include "AwMarkerRuleElement.h"

AwMarkerRuleElement::AwMarkerRuleElement(QObject *parent)
	: QObject(parent)
{
	m_condition = AwMarkerRuleElement::RuleConditionEquals;
	m_type = AwMarkerRuleElement::RuleElementLabel;
	m_isValid = false;
}

AwMarkerRuleElement::AwMarkerRuleElement(AwMarkerRuleElement *element)
{
	m_condition = element->condition();
	m_type = element->type();
	m_value = element->value();
	m_isValid = element->isValid();
}

AwMarkerRuleElement::~AwMarkerRuleElement()
{

}


void AwMarkerRuleElement::checkIfValid()
{
	bool isValid = true;

	switch (m_type)
	{
	case AwMarkerRuleElement::RuleElementTargetChannel:
		{
			QString value = m_value.toString();
			if (value.isEmpty())
				isValid = false;
		}
		break;
	case AwMarkerRuleElement::RuleElementLabel:
		{
			QString value = m_value.toString();
			if (value.isEmpty())
				isValid = false;
		}
		break;
	case AwMarkerRuleElement::RuleElementCode:
		if (isInRange())
		{
			QVariantList list = m_value.toList();
			if (list.size() != 2)
				isValid = false;
			else
			{
				int low = list.at(0).toInt();
				int high = list.at(1).toInt();

				if (high <= low)
					isValid = false;
			}
		}
		break;
	case AwMarkerRuleElement::RuleElementDuration:
		if (isInRange())
		{
			QVariantList list = m_value.toList();
			if (list.size() != 2)
				isValid = false;
			else
			{
				int low = list.at(0).toFloat();
				int high = list.at(1).toFloat();

				if (high <= low)
					isValid = false;
			}
		}
		break;
	}
	m_isValid = isValid;
}