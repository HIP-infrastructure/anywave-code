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
#include "AwMarkerRuleManageDial.h"
#include "AwNewMarkerRuleDial.h"

AwMarkerRuleManageDial::AwMarkerRuleManageDial(const QStringList& rules, QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);

	if (!rules.isEmpty())	{
		m_loadedRules = rules;
		listRules->addItems(m_loadedRules);
	}

	connect(listRules, SIGNAL(itemSelectionChanged()), this, SLOT(updateButtons()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeRule()));
	connect(buttonNew, SIGNAL(clicked()), this, SLOT(newRule()));
	connect(buttonModify, SIGNAL(clicked()), this, SLOT(modifyRule()));
}

AwMarkerRuleManageDial::~AwMarkerRuleManageDial()
{

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS



void AwMarkerRuleManageDial::updateButtons()
{
	buttonModify->setEnabled(!listRules->selectedItems().isEmpty());
	buttonRemove->setEnabled(!listRules->selectedItems().isEmpty());
}

void AwMarkerRuleManageDial::removeRule()
{
	QList<QListWidgetItem *> items = listRules->selectedItems();

	if (items.isEmpty())
		return;

	emit ruleRemoved(items.at(0)->text());
	listRules->removeItemWidget(items.at(0));
}

void AwMarkerRuleManageDial::newRule()
{
	AwNewMarkerRuleDial dlg;

	if (dlg.exec() == QDialog::Accepted)
	{
		listRules->addItem(dlg.newRuleName());
		emit ruleAdded(dlg.newRuleName());
	}
}

void AwMarkerRuleManageDial::modifyRule()
{
	// get the current selected rule (if any)
	QList<QListWidgetItem *> items = listRules->selectedItems();

	if (items.isEmpty())
		return;

	AwNewMarkerRuleDial dlg(items.at(0)->text());
	if (dlg.exec() == QDialog::Accepted)
	{
		if (dlg.newRuleName() != dlg.ruleName())
			emit ruleChanged(dlg.ruleName(), dlg.newRuleName());
	}
}