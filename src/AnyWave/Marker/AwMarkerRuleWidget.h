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
#ifndef AWMARKERRULEWIDGET_H
#define AWMARKERRULEWIDGET_H

#include <QWidget>
#include "ui_AwMarkerRuleWidget.h"
#include "AwMarkerRuleElement.h"
#include <AwMarker.h>

class AwMarkerRuleWidget : public QWidget, public Ui::AwMarkerRuleWidgetUI
{
	Q_OBJECT

public:
	AwMarkerRuleWidget(AwMarkerRuleElement *element = NULL, QWidget *parent = 0);
	~AwMarkerRuleWidget();
	inline AwMarkerRuleElement *ruleElement() { return m_element; }

	void lock();
	void unlock();
public slots:
	void setValue();
signals:
	void addNewRuleClicked();
	void ruleRemoved(AwMarkerRuleElement *element);
protected slots:
	void updateOnNewType(int index);
	void updateNewCondition(int index);
	void updateValue(int index);
	void removeRule();
protected:
	QStringList labelsFromMarkers();
	QStringList codesFromMarkers();
	QVariantList parseRangeInt(const QString& value);
	QVariantList parseRangeFloat(const QString& value);

	AwMarkerRuleElement *m_element;
	AwSharedMarkerList m_markers;	// list of all markers
	QLineEdit *m_editValue;
};

#endif // AWMARKERRULEWIDGET_H
