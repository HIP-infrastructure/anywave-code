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
#include "AwMarkerRule.h"
#include "Prefs/AwSettings.h"
#include <QDomElement> // For xml input/output
#include "AwMarkerRuleElement.h"
#include <QFile>
#include <QTextStream>

AwMarkerRule::AwMarkerRule(QObject *parent)
	: QObject(parent)
{
	m_isValid = true;
}

AwMarkerRule::~AwMarkerRule()
{

}

void AwMarkerRule::removeElement(AwMarkerRuleElement *elem)
{
	int index = m_elements.indexOf(elem);

	if (index != -1)
		m_elements.removeAt(index);
}

/*!
 * \brief
 * apply the rule on the marker list.
 * 
 * \param list
 * List of markers to apply rule on.
 * 
 * \returns
 * List of markers filtered by the rule. May be empty.
 * 
 * 
 * Parse each element of the rule and filters markers accordingly.
 * 
 * 
 * \see
 * AwMarkerRuleElement
 */
AwMarkerList AwMarkerRule::applyRule(const AwMarkerList &list)
{
	AwMarkerList result;

	if (list.isEmpty())
		return result;

	foreach (AwMarker *m, list)	{
		foreach (AwMarkerRuleElement *e, m_elements) {
			switch (e->type()) {
			case AwMarkerRuleElement::RuleElementTargetChannel:
				// rule on targeted channels
				if (e->isEqual()) {
					QString value = e->value().toString();
					QStringList targets = m->targetChannels();
					if (!targets.isEmpty() && targets.contains(value))
						result << m;
					break;
				}
				if (e->isDifferent()) {
					QString value = e->value().toString();
					QStringList targets = m->targetChannels();
					if (!targets.isEmpty() && !targets.contains(value))
						result << m;
					break;
				}
				break;
			case AwMarkerRuleElement::RuleElementLabel:
				// rule on label => only two conditions here : equals or different
				if (e->isEqual()) {
					QString val = e->value().toString();
					if (m->label() == val)
						result << m;
					break;
				}
				if (e->isDifferent()) {
					QString val = e->value().toString();
					if (m->label() != val)
						result << m;
					break;
				}
				if (e->contains()) {
					if (m->label().contains(e->value().toString()))
						result << m;
					break;
				}
				break;
			case AwMarkerRuleElement::RuleElementCode:
				// rule on digital code
				if (e->isGreaterThan())	{
					int val = e->value().toInt();
					if (m->value() > val)
						result << m;
					break;
				}
				if (e->isLesserThan()) {
					int val = e->value().toInt();
					if (m->value() < val)
						result << m;
					break;
				}
				if (e->isEqual()) {
					int val = e->value().toInt();
					if (m->value() == val)
						result << m;
					break;
				}
				if (e->isDifferent()) {
					int val = e->value().toInt();
					if (m->value() != val)
						result << m;
					break;
				}
				if (e->isInRange()) {
					QList<QVariant> list = e->value().toList();

					int low = list.at(0).toInt();
					int high = list.at(1).toInt();

					if (m->value() >= low && m->value() <= high)
						result << m;
					break;
				}
				break;
			case AwMarkerRuleElement::RuleElementDuration:
				if (e->isGreaterThan()) {
					float val = e->value().toFloat();
					if (m->duration() > val)
						result << m;
					break;
				}
				if (e->isLesserThan()) {
					float val = e->value().toFloat();
					if (m->duration() < val)
						result << m;
					break;
				}
				if (e->isEqual()) {
					float val = e->value().toFloat();
					if (m->duration() == val)
						result << m;
					break;
				}
				if (e->isDifferent()) {
					int val = e->value().toInt();
					if (m->duration() != val)
						result << m;
					break;
				}
				if (e->isInRange()) {
					QList<QVariant> list = e->value().toList();

					float low = list.at(0).toFloat();
					float high = list.at(1).toFloat();

					if (m->duration() >= low && m->duration() <= high)
						result << m;
					break;
				}
				break;
			}
		}
	}
	return result;
}

void AwMarkerRule::clean()
{
	while (!m_elements.isEmpty())
		delete m_elements.takeFirst();
}

void AwMarkerRule::checkIfValid()
{
	bool isValid = true;
	// trim rule name
	m_name = m_name.trimmed();
	if (m_name.isEmpty())
	{
		m_isValid = false;
		return;
	}
	// check for invalid characters
	if (m_name.contains('/') || m_name.contains('\\') || m_name.contains('&') ||
		m_name.contains(':') || m_name.contains('*') || m_name.contains('~'))
	{
		m_isValid = false;
		return;
	}
	foreach (AwMarkerRuleElement *e, m_elements)
		if (!e->isValid())
			isValid = false;

	m_isValid = isValid;
}

bool AwMarkerRule::save()
{
	auto dir = AwSettings::getInstance()->value(aws::marker_rules_dir).toString();
	if (dir.isEmpty())
		return false;

	QString path = QString("%1%2.awr").arg(dir).arg(m_name);
	QFile file(path);

	if (!file.open(QIODevice::WriteOnly))
		return false;

	QTextStream out;
	out.setDevice(&file);

	QDomDocument doc("AnyWaveMarkerRule");
	QDomElement root = doc.createElement("MarkerRule");
	QDomElement element;
	QDomElement child;

	doc.appendChild(root);

	foreach (AwMarkerRuleElement *re, m_elements) {
		element = doc.createElement("ElementType");
		element.setAttribute("type", QString("%1").arg(re->type()));
		root.appendChild(element);
		
		child = doc.createElement("Condition");

		switch (re->type()) {
		case AwMarkerRuleElement::RuleElementLabel:
			if (re->isEqual())
				child.appendChild(doc.createTextNode("Is Equal"));
			else if (re->isDifferent())
				child.appendChild(doc.createTextNode("Is Different"));
			else if (re->contains())
				child.appendChild(doc.createTextNode("Contains"));

			child.setAttribute("value", re->value().toString());

			element.appendChild(child);
			break;
		case AwMarkerRuleElement::RuleElementCode:
			if (re->isEqual()) 	{
				child.appendChild(doc.createTextNode("Is Equal"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isDifferent()) {
				child.appendChild(doc.createTextNode("Is Different"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isGreaterThan()) {
				child.appendChild(doc.createTextNode("Is Greater Than"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isLesserThan()) {
				child.appendChild(doc.createTextNode("Is Lesser Than"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isInRange()) {
				child.appendChild(doc.createTextNode("Is In Range"));
				QList<int> range;
				QVariantList list = re->value().toList();
				foreach (QVariant v, list)
					range << v.toInt();
				child.setAttribute("min", QString("%1").arg(range.at(0)));
				child.setAttribute("max", QString("%1").arg(range.at(1)));
			}
			element.appendChild(child);
			break;
		case AwMarkerRuleElement::RuleElementDuration:
			if (re->isEqual())	{
				child.appendChild(doc.createTextNode("Is Equal"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isDifferent())	{
				child.appendChild(doc.createTextNode("Is Different"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isGreaterThan()) {
				child.appendChild(doc.createTextNode("Is Greater Than"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isLesserThan()) {
				child.appendChild(doc.createTextNode("Is Lesser Than"));
				child.setAttribute("value", QString("%1").arg(re->value().toInt()));
			}
			else if (re->isInRange()) {
				child.appendChild(doc.createTextNode("Is In Range"));
				QList<float> range;
				QVariantList list = re->value().toList();
				foreach (QVariant v, list)
					range << v.toFloat();
				child.setAttribute("min", QString("%1").arg(range.at(0)));
				child.setAttribute("max", QString("%1").arg(range.at(1)));
			}
			element.appendChild(child);
			break;
		case AwMarkerRuleElement::RuleElementTargetChannel:
			if (re->isEqual())
				child.appendChild(doc.createTextNode("Is Equal"));
			else if (re->isDifferent())
				child.appendChild(doc.createTextNode("Is Different"));
			child.setAttribute("value", re->value().toString());
			element.appendChild(child);
			break;
		}
	}

	doc.save(out, 3);
	file.close();
	return true;
}

bool AwMarkerRule::load()
{
	auto dir = AwSettings::getInstance()->value(aws::marker_rules_dir).toString();
	if (dir.isEmpty())
		return false;

	QString path = QString("%1%2.awr").arg(dir).arg(m_name);

	QFile file(path);

	if (!file.open(QIODevice::ReadOnly))
		return false;

	QDomDocument doc;
	QDomElement element;
	
	if (!doc.setContent(&file))
	{
		file.close();
		return false;
	}

	QDomElement root = doc.documentElement(); // get root tag
	if (root.tagName() != "MarkerRule")	{
		file.close();
		return false;
	}

	// clean current rule elements
	clean();

	QDomNode node = root.firstChild();
	while (!node.isNull())	{
		element = node.toElement();
		QDomNode child;

		if (element.tagName() == "ElementType")	{
			int type = element.attribute("type").toInt();
			AwMarkerRuleElement *re = new AwMarkerRuleElement();
			re->setType(type);

			switch (type)
			{
			case AwMarkerRuleElement::RuleElementTargetChannel:
			case AwMarkerRuleElement::RuleElementLabel:
				// parse child elements (conditions)
				child = element.firstChild();
				while (!child.isNull())	{
					QDomElement ee = child.toElement();
					if (ee.tagName() == "Condition") {
						QString condition = ee.text();

						if (condition == "Is Equal") {
							re->setCondition(AwMarkerRuleElement::RuleConditionEquals);
							re->setValue(ee.attribute("value"));
						}
						else if (condition == "Is Different") {
							re->setCondition(AwMarkerRuleElement::RuleConditionDifferent);
							re->setValue(ee.attribute("value"));
						}
						else if(condition == "Contains") {
							re->setCondition(AwMarkerRuleElement::RuleConditionContains);
							re->setValue(ee.attribute("value"));
						}
					}
					child = child.nextSibling();
				}
				m_elements << re;
				break;
			case AwMarkerRuleElement::RuleElementCode:
				// parse child elements
				child = element.firstChild();
				while (!child.isNull()) 	{
					QDomElement ee = child.toElement();
					if (ee.tagName() == "Condition") {
						QString condition = ee.text();

						if (condition == "Is Equal") {
							re->setCondition(AwMarkerRuleElement::RuleConditionEquals);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is Different") {
							re->setCondition(AwMarkerRuleElement::RuleConditionDifferent);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is Greater Than") {
							re->setCondition(AwMarkerRuleElement::RuleConditionGreaterThan);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is Lesser Than") 	{
							re->setCondition(AwMarkerRuleElement::RuleConditionLesserThan);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is In Range") {
							re->setCondition(AwMarkerRuleElement::RuleConditionInRange);
							QVariantList range;
							range << ee.attribute("min").toInt();
							range << ee.attribute("max").toInt();
							re->setValue(range);
						}
					}
					child = child.nextSibling();
				}
				m_elements << re;
				break;
			case AwMarkerRuleElement::RuleElementDuration:
				child = element.firstChild();
				while (!child.isNull()) 	{
					QDomElement ee = child.toElement();
					if (ee.tagName() == "Condition") {
						QString condition = ee.text();
						if (condition == "Is Equal") {
							re->setCondition(AwMarkerRuleElement::RuleConditionEquals);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is Different") {
							re->setCondition(AwMarkerRuleElement::RuleConditionDifferent);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is Greater Than") {
							re->setCondition(AwMarkerRuleElement::RuleConditionGreaterThan);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is Lesser Than") {
							re->setCondition(AwMarkerRuleElement::RuleConditionLesserThan);
							re->setValue(ee.attribute("value").toInt());
						}
						else if (condition == "Is In Range") {
							re->setCondition(AwMarkerRuleElement::RuleConditionInRange);
							QVariantList range;
							range << ee.attribute("min").toFloat();
							range << ee.attribute("max").toFloat();
							re->setValue(range);
						}
					}
					child = child.nextSibling();
				}
				m_elements << re;
				break;
			}
		}
		node = node.nextSibling();
	}
	file.close();
	return true;
}


void AwMarkerRule::remove(const QString& name)
{
	auto dir = AwSettings::getInstance()->value(aws::marker_rules_dir).toString();

	if (dir.isEmpty())
		return;

	QString path = QString("%1%2.awr").arg(dir).arg(name);
	QFile::remove(path);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS
/////////////////////////////////////////////////////////////////////////////////////////////////////////


void AwMarkerRule::addElement(AwMarkerRuleElement *elem)
{
	m_elements << elem;
}


bool AwMarkerRule::exists()
{
	auto dir = AwSettings::getInstance()->value(aws::marker_rules_dir).toString();
	if (dir.isEmpty())
		return false;
	QString path = QString("%1%2.awr").arg(dir).arg(m_name);
	return QFile::exists(path);
}


