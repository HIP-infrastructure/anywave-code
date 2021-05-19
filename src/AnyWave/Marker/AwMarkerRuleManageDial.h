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
#ifndef AWMARKERRULEMANAGEDIAL_H
#define AWMARKERRULEMANAGEDIAL_H

#include <QDialog>
#include "ui_AwMarkerRuleManageDial.h"

class AwMarkerRuleManageDial : public QDialog, public Ui::AwMarkerRuleManageDialUI
{
	Q_OBJECT

public:
	AwMarkerRuleManageDial(const QStringList& rules, QWidget *parent = 0);
	~AwMarkerRuleManageDial();
signals:
	void ruleRemoved(const QString& name);
	void ruleAdded(const QString& name);
	void ruleChanged(const QString& oldName, const QString& newName);
protected:
	QStringList m_loadedRules;
protected slots:
	void updateButtons();
	void removeRule();
	void newRule();
	void modifyRule();
};

#endif // AWMARKERRULEMANAGEDIAL_H
