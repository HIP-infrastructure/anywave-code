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
#ifndef AWMARKER_H
#define AWMARKER_H
#include <AwGlobal.h>
#include <QStringList>
#include <QMetaType>
/*!
 * \brief
 * This class defines the AwMarker object.
 * 
 * Markers can mark a specified position in a file or a time selection.
 * They can also target some channels.
 * By default a marker is defined to be global. Addings channels as targets will make it channel specific.
 * 
 *
 * \remarks
 * Some attributes are optional, like: value, targetedChannels and duration.
 * 
 */

class AW_CORE_EXPORT AwMarker
{	
public:
	// Markers types.
	enum MarkerType {Single = 2, Selection = 4};

	/** Default constructor that defines a Single marker. **/
	AwMarker();
	/** Copy constructor. **/
	AwMarker(AwMarker *marker);
	/** fancy constructor **/
	AwMarker(const QString& label, float position = 0., float duration = 0.);
	virtual ~AwMarker() {}

    static int stringToType(const QString& s);
	static QString typeToString(qint32 t);
	/** Gets all the marker types available as QString in a list. **/
	static QStringList markersTypeList();
	/** Gets a unique instance of marker labels from a list **/
	static QStringList getUniqueLabels(const QList<AwMarker *>& markers);
	/** Count the number of markers with a specified label **/
	static QList<QPair<QString, int> > count(const QList<AwMarker *>& markers);
	/** Gets all markers labels **/
	static QStringList getAllLabels(const QList<AwMarker *>& markers);
	/* Get alls markers with a label, markers are not duplicated. */
	static QList<AwMarker *> getMarkersWithLabel(const QList<AwMarker *>& markers, const QString& label);
	/* Get alls markers with specified labels, markers are not duplicated. */
	static QList<AwMarker *> getMarkersWithLabels(const QList<AwMarker *>& markers, const QStringList& labels); 
	/** Rename all markers in a list. **/
	static QList<AwMarker *>& rename(QList<AwMarker *>& markers, const QString& newLabel);
	/** Load a marker file and returns the markers */ 
	static QList<AwMarker *> load(const QString& file);
	/** Save markers to a file **/
	static void save(const QString& file, const QList<AwMarker *>& markers);
	/** duplicate markers **/
	static QList<AwMarker *> duplicate(const QList<AwMarker *>& markers);
	/** Sort markers, chronologically. Don't duplicate.**/
	static QList<AwMarker *>& sort(QList<AwMarker *>& markers);
	/** Shift all markers like if the data where cut around cut markers. Duplicates markers. **/
	static QList<AwMarker *> cutAroundMarkers(QList<AwMarker *>& markers, QList<AwMarker *>& cutMarkers);
	/** Apply logical AND between markers. Returns duplicated makers. **/
	static QList<AwMarker *> applyANDOperation(QList<AwMarker *>& m1, QList<AwMarker *>& m2);
	/** Merge markers whichs overlaps. Return the resulting merged markers **/
	static QList<AwMarker *> merge(QList<AwMarker *>& markers);
	/** Get markers from a list which intersect a time selection **/
	static QList<AwMarker *> intersect(const QList<AwMarker *>& markers, float start, float end);
	/** Create an inverter marker list that skip the markers set as input **/
	static QList<AwMarker *> invertMarkerSelection(const QList<AwMarker *>& markers, const QString& label, float end);
	/** Filters markers: markers can either be specified to be removed or used. **/
	static QList<AwMarker *> applySelectionFilter(const QList<AwMarker *>& markers, const QStringList& skip, const QStringList& used, float totalDuration);
	/** Returns the marker's label. **/
	inline QString& label() { return m_label; }
	/** Returns the marker's type. AwMarker::Single or AwMarker::Selection. **/
	inline int type() { return m_duration ? AwMarker::Selection : AwMarker::Single; }
	/** Returns the marker's value or -1 if no value is set. **/
	inline float value() { return m_code; }

	/** Returns the position in seconds from the beginning of data block. **/
	inline float start() { return m_start; }
	/** Returns the duration in seconds. Single marker has a duration of 0 **/
	inline float duration() { return m_duration; }
	/** Returns the ending position of a marker from the beginning of data block. **/
	inline float end() { return m_start + m_duration; }
	/** Returns a string list containing all the channels targeted by the marker.
	If the list is empty then the marker is not targetting channels but it is a global marker. **/
	inline QStringList& targetChannels() { return m_targetChannels; }

	/** Returns true if 'marker' is included in the marker. **/
	bool contains(AwMarker *marker);
	inline void setLabel(const QString& label) { m_label = label; }
	inline void setValue(float value) { m_code = value; }
	inline void setStart(float start) { m_start = start; }
	inline void setDuration(float duration) { m_duration = duration; if (duration > 0) m_type = Selection;  }
	inline void setTargetChannels(const QStringList& targetChannels) { m_targetChannels = targetChannels; }
	inline void addTargetChannel(const QString& channel) { m_targetChannels.append(channel); }
	inline void setColor(const QString& color) { m_color = color; }
	inline QString& color() { return m_color; }
	void setEnd(float end);
	void reshape(float start, float end);
protected:
	QString m_label;		
	int m_type;				
	float m_start, m_duration, m_code;		
	QStringList m_targetChannels;	
	QString m_color;
};

//
// AwMarkerLessThan()
// Compare function to be used in qSort
// This function is used to sort markers chronologically.
bool AW_CORE_EXPORT AwMarkerLessThan(AwMarker *m1, AwMarker *m2);

typedef QList<AwMarker *> AwMarkerList; ///< A type defining a list of AwMarker *
Q_DECLARE_METATYPE(AwMarker)
Q_DECLARE_METATYPE(AwMarkerList)


#endif