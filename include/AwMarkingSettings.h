/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_MARKINGSETTINGS_H
#define AW_MARKINGSETTINGS_H
#include <AwGlobal.h>
#include <AwMarker.h>
#include <QObject>
#include <QVector>

class AW_WIDGETS_EXPORT AwMarkingSettings : public QObject
{
	Q_OBJECT
public:	
	AwMarkingSettings(QObject *parent = 0) : QObject(parent) 
	{
		type = AwMarker::Single;
		label = "New Marker";
		index = 1;
		usingSpacebar = false;
		isAutoInc = false;
		isTargettingChannels = false;
		value = -1.0;
		autoTargetChannel = false;
		m_spaceBarIndex = 0;
	}
	void setPredefinedMarkers(const AwMarkerList& markers);
	AwMarkerList getSelectedPredefinedMarkers();

	int type;						// current marker type
	bool isUsingList;				// true if we are using a predefined list of markers
	QString label;					// marker label
	float value;					// associated value 
	bool isAutoInc;					// true if auto incrementing label is on
	bool isTargettingChannels;		// true if marking is targeting predefined channels
	QStringList targets;			// labels of predefined targets
	int index;						// used while auto incrementing
	AwMarkerList predefinedMarkers;	// The predefined list of markers
	QVector<int> selectedPredefinedMarkers;
	bool autoTargetChannel;			// true if the channel under the mouse will be used as target
	bool usingSpacebar;				// true if using spacebar binding
	QString color;					// color to set for the marker
//	AwMarker *spaceBarNext();
//	void setBoundMarkers(AwMarkerList& markers);
protected:
	int m_spaceBarIndex;				// current index in boundMarkers 
	AwMarkerList m_boundMarkers;		// List of markers used with spacebar 
};

#endif