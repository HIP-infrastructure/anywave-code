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