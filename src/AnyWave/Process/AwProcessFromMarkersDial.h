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

#ifndef AWPROCESSFROMMARKERSDIAL_H
#define AWPROCESSFROMMARKERSDIAL_H

#include <QDialog>
#include "ui_AwProcessFromMarkersDial.h"
#include <AwMarker.h>


class AwProcessFromMarkersDial : public QDialog
{
	Q_OBJECT

public:
	AwProcessFromMarkersDial(const AwMarkerList& markers, const AwMarkerList& selectedMarkers, QWidget *parent = 0);
	~AwProcessFromMarkersDial();

	AwMarkerList markersForProcess() { return m_processMarkers; }
protected:
	void changeEvent(QEvent *);
public slots:
	void accept();
	int exec();	
private:
	Ui::AwProcessFromMarkersDialClass m_ui;
	QStringList m_plugins;
	QStringList m_types;
	QStringList m_names;
	QStringList m_codes;
	AwMarkerList m_markers;
	AwMarkerList m_selectedMarkers;
	AwMarkerList m_processMarkers;	// final list built and returned to launch process
};


#endif // AWPROCESSFROMMARKERSDIAL_H
