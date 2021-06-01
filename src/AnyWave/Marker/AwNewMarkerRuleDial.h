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
#ifndef AWNEWMARKERRULEDIAL_H
#define AWNEWMARKERRULEDIAL_H

#include <QDialog>
#include "ui_AwNewMarkerRuleDial.h"
#include "AwMarkerRule.h"
#include "AwMarkerRuleWidget.h"

class AwNewMarkerRuleDial : public QDialog, public Ui::AwNewMarkerRuleDialUI
{
	Q_OBJECT

public:
	AwNewMarkerRuleDial(const QString& ruleName = QString(), QWidget *parent = 0);
	~AwNewMarkerRuleDial();

	inline QString& ruleName() { return m_ruleName; }
	inline QString& newRuleName() { return m_newRuleName; }
public slots:
	void accept();
	void reject();
protected slots:
	void removeRule(AwMarkerRuleElement *element);
	void addNewRule();
protected:
	QString m_ruleName;
	QString m_newRuleName;
	AwMarkerRule *m_rule;
	bool m_newRule;	// flag set to true if creating a new rule. False if modifyng an existing rule.
	QList<AwMarkerRuleWidget *> m_widgets;
	QList<AwMarkerRuleElement *> m_savedElements;
};

#endif // AWNEWMARKERRULEDIAL_H
