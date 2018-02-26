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
#include "AwMarkerRuleWidget.h"
#include "AwMarkerManager.h"
#include "Montage/AwMontageManager.h"
#include <QMessageBox>
#include <QLineEdit>

AwMarkerRuleWidget::AwMarkerRuleWidget(AwMarkerRuleElement *element, QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	m_element = element;
	m_markers = AwMarkerManager::instance()->getMarkers();

	// fill comboTypes;
	comboType->addItem(tr("Label"), AwMarkerRuleElement::RuleElementLabel);
	comboType->addItem(tr("Value"), AwMarkerRuleElement::RuleElementCode);
	comboType->addItem(tr("Duration"), AwMarkerRuleElement::RuleElementDuration);
	comboType->addItem(tr("Targeted Channel"), AwMarkerRuleElement::RuleElementTargetChannel);

	if (m_element == NULL)	
		// if no element specified init an empty element 
		m_element = new AwMarkerRuleElement();

	switch (m_element->type())	{
	case AwMarkerRuleElement::RuleElementLabel:
		comboType->setCurrentIndex(0);
		updateOnNewType(0);
		break;
	case AwMarkerRuleElement::RuleElementCode:
		comboType->setCurrentIndex(1);
		updateOnNewType(1);
		break;
	case AwMarkerRuleElement::RuleElementDuration:
		comboType->setCurrentIndex(2);
		updateOnNewType(2);
		break;
	case AwMarkerRuleElement::RuleElementTargetChannel:
		comboType->setCurrentIndex(3);
		updateOnNewType(3);
	}

	m_editValue = new QLineEdit();
	comboConditionValue->setLineEdit(m_editValue);

	// connections
	connect(comboType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOnNewType(int)));
	connect(comboCondition, SIGNAL(currentIndexChanged(int)), this, SLOT(updateNewCondition(int)));
	connect(comboConditionValue, SIGNAL(currentIndexChanged(int)), this, SLOT(updateValue(int)));
	connect(m_editValue, SIGNAL(editingFinished()), this, SLOT(setValue()));
	connect(buttonAdd, SIGNAL(clicked()), this, SIGNAL(addNewRuleClicked()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeRule()));
}	

AwMarkerRuleWidget::~AwMarkerRuleWidget()
{
	
}

QStringList AwMarkerRuleWidget::labelsFromMarkers()
{
	QStringList result;

	foreach (AwMarker *m, m_markers)
		if (!result.contains(m->label()))
			result << m->label();
	return result;
}


QStringList AwMarkerRuleWidget::codesFromMarkers()
{
	QStringList result;

	foreach (AwMarker *m, m_markers) {
		QString code = QString("%1").arg(m->value());
		if (!result.contains(code))
			result << code;
	}

	return result;
}

QVariantList AwMarkerRuleWidget::parseRangeInt(const QString& value)
{
	QVariantList result;
	QRegExp rexp("(\\d+)");	// integer value match
	int pos = 0; 

	while ((pos = rexp.indexIn(value, pos)) != -1) {
		result << rexp.cap(1).toInt();
		pos += rexp.matchedLength();
	}

	return result;
}

QVariantList AwMarkerRuleWidget::parseRangeFloat(const QString& value)
{
	QVariantList result;
	QRegExp rexp("(\\d*\\.\\d+)");	// floating point matching
	int pos = 0;
	
	while ((pos = rexp.indexIn(value, pos)) != -1) {
		result << rexp.cap(1).toFloat();
		pos += rexp.matchedLength();
	}

	return result;
}

void AwMarkerRuleWidget::lock()
{
	buttonRemove->setEnabled(false);
}

void AwMarkerRuleWidget::unlock()
{
	buttonRemove->setEnabled(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS.

void AwMarkerRuleWidget::updateValue(int index)
{
	switch (m_element->type())
	{
	case AwMarkerRuleElement::RuleElementTargetChannel:
	case AwMarkerRuleElement::RuleElementLabel:
		m_element->setValue(comboConditionValue->itemText(index));
		break;
	case AwMarkerRuleElement::RuleElementCode:
		//m_element->setValue(comboConditionValue->itemText(index).toInt());
		m_element->setValue(comboConditionValue->itemText(index).toDouble());
	}
}

void AwMarkerRuleWidget::setValue()
{
	QString value = m_editValue->text();

	switch (m_element->type())
	{
	case AwMarkerRuleElement::RuleElementTargetChannel:
	case AwMarkerRuleElement::RuleElementLabel:
		m_element->setValue(value);
		break;
	case AwMarkerRuleElement::RuleElementCode:
		if (!m_element->isInRange())
			m_element->setValue(value.toInt());
		else
			//m_element->setValue(parseRangeInt(value));
			m_element->setValue(parseRangeFloat(value));
		break;
	case AwMarkerRuleElement::RuleElementDuration:
		if (!m_element->isInRange())
			m_element->setValue(value.toInt());
		else
			m_element->setValue(parseRangeFloat(value));
		break;
	}
}

void AwMarkerRuleWidget::updateNewCondition(int index)
{
	// get element type from item index in comboType
	int condition = comboCondition->itemData(index).toInt();

	m_element->setCondition(condition);
}


void AwMarkerRuleWidget::updateOnNewType(int index)
{
	// get element type from item index in comboType
	int type = comboType->itemData(index).toInt();
	QStringList labels;
	AwChannelList channels;

	m_element->setType(type);

	switch (type)
	{
	case AwMarkerRuleElement::RuleElementTargetChannel:
		// build comboCondition's items list
		comboCondition->clear();
		comboCondition->addItem(tr("Is Equal To"), AwMarkerRuleElement::RuleConditionEquals);
		comboCondition->addItem(tr("Is Not Equal To"), AwMarkerRuleElement::RuleConditionDifferent);
		// build comboValue
		comboConditionValue->clear();
		// get channels from current montage
		channels = AwMontageManager::instance()->channels();
		if (channels.isEmpty())
			break;
		foreach (AwChannel *c, channels)
			labels << c->name();
		comboConditionValue->addItems(labels);
		if (m_element)	{ // if element is not null than set combo box element to corresponding value in m_element
			if (m_element->isDifferent())
				comboCondition->setCurrentIndex(1);
			else 
				comboCondition->setCurrentIndex(0);

			QString value = m_element->value().toString();
			int index = comboConditionValue->findText(value);
			if (index != -1)	// if current value of element is in combo elements, than set to corresponding index
				comboConditionValue->setCurrentIndex(index);
			else { // value is not in combo items list, so add it and set current index of combo to newly added element
				comboConditionValue->addItem(value);
				comboConditionValue->setCurrentIndex(comboConditionValue->count() - 1);
			}
		}
		break;
	case AwMarkerRuleElement::RuleElementLabel:
		// build comboCondition's items list
		comboCondition->clear();
		comboCondition->addItem(tr("Is Equal To"), AwMarkerRuleElement::RuleConditionEquals);
		comboCondition->addItem(tr("Is Not Equal To"), AwMarkerRuleElement::RuleConditionDifferent);
		comboCondition->addItem(tr("Contains"), AwMarkerRuleElement::RuleConditionContains);
		
		// build comboValue
		comboConditionValue->clear();
		labels = labelsFromMarkers();
		if (!labels.isEmpty())
			comboConditionValue->addItems(labelsFromMarkers());

		if (m_element) {	// if element is not null than set combo box element to corresponding value in m_element
			if (m_element->isDifferent())
				comboCondition->setCurrentIndex(1);
			else if (m_element->isEqual())
				comboCondition->setCurrentIndex(0);
			else if (m_element->contains())
				comboCondition->setCurrentIndex(2);

			QString value = m_element->value().toString();
			int index = comboConditionValue->findText(value);
			if (index != -1)	// if current value of element is in combo elements, than set to corresponding index
				comboConditionValue->setCurrentIndex(index);
			else // value is not in combo items list, so add it and set current index of combo to newly added element
			{
				comboConditionValue->addItem(value);
				comboConditionValue->setCurrentIndex(comboConditionValue->count() - 1);
			}
		}
		break;
	case AwMarkerRuleElement::RuleElementCode:
		// build comboCondition's items list
		comboCondition->clear();
		comboCondition->addItem(tr("Is Equal To"), AwMarkerRuleElement::RuleConditionEquals);
		comboCondition->addItem(tr("Is Not Equal To"), AwMarkerRuleElement::RuleConditionDifferent);
		comboCondition->addItem(tr("Is Greater Than"), AwMarkerRuleElement::RuleConditionGreaterThan);
		comboCondition->addItem(tr("Is Lesser Than"), AwMarkerRuleElement::RuleConditionLesserThan);
		comboCondition->addItem(tr("Is Within Range"), AwMarkerRuleElement::RuleConditionInRange);

		// build comboValue
		comboConditionValue->clear();
		comboConditionValue->addItems(codesFromMarkers());

		if (m_element)  {  // if element is not null than set combo box element to corresponding value in m_element
			switch (m_element->condition())
			{
			case AwMarkerRuleElement::RuleConditionDifferent:
				comboCondition->setCurrentIndex(1);
				break;
			case AwMarkerRuleElement::RuleConditionEquals:
				comboCondition->setCurrentIndex(0);
				break;
			case AwMarkerRuleElement::RuleConditionGreaterThan:
				comboCondition->setCurrentIndex(2);
				break;	
			case AwMarkerRuleElement::RuleConditionLesserThan:
				comboCondition->setCurrentIndex(3);
				break;	
			case AwMarkerRuleElement::RuleConditionInRange:
				comboCondition->setCurrentIndex(4);
				break;	
			}

			QString value;

			if (m_element->isInRange()) {
				QVariantList list = m_element->value().toList();
				int low = list.at(0).toInt();
				int high = list.at(1).toInt();
				value = QString("%1;%2").arg(low).arg(high);
			}
			else
				value = QString("%1").arg(m_element->value().toInt());

			int index = comboConditionValue->findText(value);
			if (index != -1)	// if current value of element is in combo elements, than set to corresponding index
				comboConditionValue->setCurrentIndex(index);
			else { // value is not in combo items list, so add it and set current index of combo to newly added element
				comboConditionValue->addItem(value);
				comboConditionValue->setCurrentIndex(comboConditionValue->count() - 1);
			}
		}
		break;
	case AwMarkerRuleElement::RuleElementDuration:
		// build comboCondition's items list
		comboCondition->clear();
		comboCondition->addItem(tr("Is Equal To"), AwMarkerRuleElement::RuleConditionEquals);
		comboCondition->addItem(tr("Is Not Equal To"), AwMarkerRuleElement::RuleConditionDifferent);
		comboCondition->addItem(tr("Is Greater Than"), AwMarkerRuleElement::RuleConditionGreaterThan);
		comboCondition->addItem(tr("Is Lesser Than"), AwMarkerRuleElement::RuleConditionLesserThan);
		comboCondition->addItem(tr("Is Within Range"), AwMarkerRuleElement::RuleConditionInRange);
		
		// build comboValue
		comboConditionValue->clear();
		if (m_element)    // if element is not null than set combo box element to corresponding value in m_element
		{
			switch (m_element->condition())
			{
			case AwMarkerRuleElement::RuleConditionDifferent:
				comboCondition->setCurrentIndex(1);
				break;
			case AwMarkerRuleElement::RuleConditionEquals:
				comboCondition->setCurrentIndex(0);
				break;
			case AwMarkerRuleElement::RuleConditionGreaterThan:
				comboCondition->setCurrentIndex(2);
				break;	
			case AwMarkerRuleElement::RuleConditionLesserThan:
				comboCondition->setCurrentIndex(3);
				break;	
			case AwMarkerRuleElement::RuleConditionInRange:
				comboCondition->setCurrentIndex(4);
				break;	
			}

			QString value;

			if (m_element->isInRange()) {
				QVariantList list = m_element->value().toList();
				int low = list.at(0).toInt();
				int high = list.at(1).toInt();
				value = QString("%1;%2").arg(low).arg(high);
			}
			else
				value = QString("%1").arg(m_element->value().toFloat());

			comboConditionValue->addItem(value);
		}
		break;
	}
}


void AwMarkerRuleWidget::removeRule()
{
	emit ruleRemoved(m_element);
}



