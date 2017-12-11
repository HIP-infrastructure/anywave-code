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
#pragma once

#include <QDialog>
namespace Ui { class AwInputMarkerWidgetUi; };
#include <AwMarker.h>
#include <AwGlobal.h>

class AW_WIDGETS_EXPORT AwInputMarkerWidget : public QDialog
{
	Q_OBJECT

public:
	AwInputMarkerWidget(QWidget *parent = Q_NULLPTR);
	~AwInputMarkerWidget();
	enum Flags {
		MultiSelection = 1, ShowLabel = 2, ShowDuration = 4, ShowPosition = 8,
		ShowColor = 16, ShowTargets = 32, ShowValue = 64, ShowAllProperties = 126, UniqueMarkerLabel = 128, 
		OnlySingleMarkers = 256, OnlySelectionMarkers = 512
	};

	/** Use setFlags before to define how the markers will be filtered. **/
	void setMarkers(const AwMarkerList& markers);
	/** Defines the flags to filter markers and also what properties of marker to show **/
	void setFlags(int flags);

	inline AwMarkerList& markers() { return m_selectedMarkers; }
	inline float preLatency() { return m_pre; }
	inline float postLatency() { return m_post;  }
public slots:
	void accept();

private:
	Ui::AwInputMarkerWidgetUi *m_ui;
	int m_flags;
	AwMarkerList m_allMarkers, m_displayedMarkers, m_selectedMarkers;
	float m_pre, m_post;
};
