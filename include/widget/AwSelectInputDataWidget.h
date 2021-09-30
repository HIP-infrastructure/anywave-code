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
#pragma once

#include <QWidget>
#include <AwMarker.h>

namespace Ui { class AwSelectInputDataWidgetUi; };

class AW_WIDGETS_EXPORT AwSelectInputDataWidget : public QWidget
{
	Q_OBJECT

public:
	AwSelectInputDataWidget(QWidget *parent = Q_NULLPTR);
	~AwSelectInputDataWidget();
	enum Filters { AllMarkers, WithDuration, UniqueLabels, WithDurationAndUniqueLabels };

	void setMarkers(const AwMarkerList& markers, int filter = AwSelectInputDataWidget::WithDurationAndUniqueLabels);
	QStringList skippedMarkers();
	QStringList usedMarkers();
protected slots:
	void addSkipLabel();
	void addUseLabel();
	void clearUse();
	void clearSkip();
	void changeSkipStatus(bool flag);
	void changeUseStatus(bool flag);
private:
	QStringList m_skipLabels, m_useLabels;
	Ui::AwSelectInputDataWidgetUi *m_ui;
};
