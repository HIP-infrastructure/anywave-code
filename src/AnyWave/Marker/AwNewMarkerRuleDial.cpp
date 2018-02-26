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
#include "AwNewMarkerRuleDial.h"
#include <widget/AwMessageBox.h>

AwNewMarkerRuleDial::AwNewMarkerRuleDial(const QString& ruleName, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	AwMarkerRuleWidget *widget;
	AwMarkerRuleElement *element;
	m_rule = new AwMarkerRule();
	m_newRuleName = ruleName;
	m_newRule = false;

	if (ruleName.isEmpty())	// Empty rule name means this is a new rule being edited!
	{
		element = new AwMarkerRuleElement();
		m_rule->addElement(element);
		widget = new AwMarkerRuleWidget(element);
		layoutRules->addWidget(widget);
		connect(widget, SIGNAL(addNewRuleClicked()), this, SLOT(addNewRule()));
		connect(widget, SIGNAL(ruleRemoved(AwMarkerRuleElement *)), this, SLOT(removeRule(AwMarkerRuleElement *)));
		m_widgets << widget;
		m_newRule = true;
	}
	else
	{
		m_ruleName = ruleName;
		m_rule->setName(ruleName);
		leName->setText(ruleName);

		if (m_rule->load())
		{
			QList<AwMarkerRuleElement *> elements = m_rule->elements();
			if (elements.isEmpty()) // SHOULD NOT BE THE CASE!!!!!!!!!!
			{
				element = new AwMarkerRuleElement();
				m_rule->addElement(element);
				widget = new AwMarkerRuleWidget(element);
				layoutRules->addWidget(widget);
				connect(widget, SIGNAL(addNewRuleClicked()), this, SLOT(addNewRule()));
				connect(widget, SIGNAL(ruleRemoved(AwMarkerRuleElement *)), this, SLOT(removeRule(AwMarkerRuleElement *)));
				m_widgets << widget;
				leName->setText("");
			}
			else
			{
				foreach (AwMarkerRuleElement *e, elements)
				{
					AwMarkerRuleElement *newElement = new AwMarkerRuleElement(e);
					m_savedElements << newElement;
					widget = new AwMarkerRuleWidget(e);
					layoutRules->addWidget(widget);
					connect(widget, SIGNAL(addNewRuleClicked()), this, SLOT(addNewRule()));
					connect(widget, SIGNAL(ruleRemoved(AwMarkerRuleElement *)), this, SLOT(removeRule(AwMarkerRuleElement *)));
					m_widgets << widget;
				}
			}
		}
	}
}

AwNewMarkerRuleDial::~AwNewMarkerRuleDial()
{


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS

void AwNewMarkerRuleDial::accept()
{
	foreach (AwMarkerRuleWidget *widget, m_widgets)
		widget->setValue();

	m_rule->setName(leName->text());
	if (!m_rule->isValid())
	{
		QMessageBox box;
		box.setText(tr("Parameter incorrect."));
		box.setInformativeText(tr("rule's name should not be empty or contain invalid characters. Check test condition also."));
		box.setStandardButtons(QMessageBox::Ok);
		box.setDefaultButton(QMessageBox::Ok);
		box.exec();
		return;
	}

	m_rule->setName(leName->text());
	m_newRuleName = leName->text();

	// check for new rule or existing modified rule
	if (m_newRule)
	{
		// we were adding a new rule, so check if the name is not already used
		if (m_rule->exists())
		{
			AwMessageBox::information(this, tr("Invalid Name"), tr("A rule with that name already exists!"), QMessageBox::Ok);
			return;
		}
	}

	// delete saved elements
	while (!m_savedElements.isEmpty())
		delete m_savedElements.takeFirst();

	// delete widgets
	while (!m_widgets.isEmpty())
		delete m_widgets.takeFirst();

	// save rule
	if (!m_rule->save())
		QMessageBox::critical(this, tr("Save Error"), tr("Rule cannot be saved.\nCheck access to your home directory."));

	QDialog::accept();
}

void AwNewMarkerRuleDial::reject()
{
	// reject means delete current rule elements
	foreach (AwMarkerRuleElement *elem, m_rule->elements())
	{
		m_rule->removeElement(elem);
		delete elem;
	}

	// set saved elements back
	foreach (AwMarkerRuleElement *elem, m_savedElements)
	{
		m_rule->addElement(elem);
	}

	// delete widgets
	while (!m_widgets.isEmpty())
		delete m_widgets.takeFirst();

	QDialog::reject();
}

void AwNewMarkerRuleDial::removeRule(AwMarkerRuleElement *element)
{
	AwMarkerRuleWidget *widget = qobject_cast<AwMarkerRuleWidget *>(sender());

	if (!widget)
		return;

	if (m_widgets.size() < 2)	// only one or zero widgets => do nothing
		return;

	// ask rule to remove element. Does nothing if rule does not contain element
	m_rule->removeElement(element);
	delete element;
	layoutRules->removeWidget(widget);
	m_widgets.removeOne(widget);
	delete widget;

	if (m_widgets.size() == 1)
		m_widgets.at(0)->lock();
}

void AwNewMarkerRuleDial::addNewRule()
{
	AwMarkerRuleElement *elem = new AwMarkerRuleElement();
	AwMarkerRuleWidget *widget = new AwMarkerRuleWidget(elem);
	m_rule->addElement(elem);
	layoutRules->addWidget(widget);
	m_widgets << widget;
	connect(widget, SIGNAL(addNewRuleClicked()), this, SLOT(addNewRule()));
	connect(widget, SIGNAL(ruleRemoved(AwMarkerRuleElement *)), this, SLOT(removeRule(AwMarkerRuleElement *)));

	foreach (AwMarkerRuleWidget *w, m_widgets)
		w->unlock();
}