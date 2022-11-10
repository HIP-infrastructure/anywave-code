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
#ifndef AWMARKAROUNDWIDGET_H
#define AWMARKAROUNDWIDGET_H
#include <AwGlobal.h>
#include <QWidget>
namespace Ui {class AwMarkAroundWidgetUi;};
#include <AwMarker.h>

class AW_WIDGETS_EXPORT AwMarkAroundWidget : public QWidget
{
	Q_OBJECT

public:
	AwMarkAroundWidget(QWidget *parent = 0);
	~AwMarkAroundWidget();

	void setMarkers(const AwSharedMarkerList& markers);
	void setTotalDuration(float duration) { m_totalDuration = duration; }
public slots:
	/** Generate markers before and after the selected marker and using the settings defined by the user **/
	void generatePrePost();	
	/** Generate marker around the selected marker and using the settings defined by the user **/
	void generateAround();	
signals:
	void newMarkersCreated(const AwSharedMarkerList& markers);
private:
	Ui::AwMarkAroundWidgetUi *m_ui;
	AwSharedMarkerList m_markers, m_newMarkers;	// complete list of all markers and current list of new markers
	float m_totalDuration;
};

#endif // AWMARKAROUNDWIDGET_H
