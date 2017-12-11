/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AW_LAYOUT_H
#define AW_LAYOUT_H
#include <AwGlobal.h>
#include <QString>
#include <QStringList>
#include <QHash>
#include <AwSensor.h>

class AW_LAYOUT_EXPORT AwLayout
{
public:
	AwLayout(const QString& name, int type, const QString& filePath);
	enum LayoutTypes { L2D = 1, L3D = 2, EEG = 4, MEG = 8 };
	enum Status { Valid, FileError };
	inline QString name() { return m_name; }
	inline int type() { return m_flags; }
	int load();
	inline AwSensorList& sensors() { return m_sensors; }
	inline QStringList& labels() { return m_labels; }
	AwSensor *getSensorByName(const QString& label) { return m_sensorsByName.value(label); }
	inline bool is2D() { return m_flags & AwLayout::L2D; }
	inline bool isMEG() { return m_flags & AwLayout::MEG; }
	inline bool is3D() { return m_flags & AwLayout::L3D; }
	inline bool isEEG() { return m_flags & AwLayout::EEG; }
	inline int numberOfElectrodes() { return m_labels.size(); }
	/** Check if a mesh file has been saved for the layout. If yes, return the path to the mesh file. **/
	QString isMeshFileExists();
protected:
	QString m_name;
	QString m_file;
	int m_flags;	// flags will be used to combine type of layout and type of signals.
	int m_status;
	QStringList m_labels;
	AwSensorList m_sensors;
	QHash<QString, AwSensor *> m_sensorsByName;
};
#endif