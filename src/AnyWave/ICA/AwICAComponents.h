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
#ifndef AWICACOMPONENTS_H
#define AWICACOMPONENTS_H

#include <QObject>
#include <hdf5/AwHDF5.h>
#include <matlab/AwMATLAB.h>
#include <AwChannel.h>
#include "AwICAChannel.h"
#include <QMutex>
#include <filter/AwFilterSettings.h>


#include <aw_armadillo.h>

class AwPanelItem;
class AwICAPanel;
class AwSEEGMap;
class AwICAComponents : public QObject
{
	Q_OBJECT

public:
	AwICAComponents(int type, QObject *parent = 0);
	~AwICAComponents();

	int loadComponents(AwHDF5& file);
	int loadComponents(AwMATLABFile& file);
	AwChannelList& sources() { return m_sources; }
	AwICAChannelList& components() { return m_icaChannels; }
	void computeComponents(AwICAChannelList& channels);
	void rejectComponent(int index);
	void addComponent(int index);
	inline bool isRejectionNeeded() { return !m_rejectedComponents.isEmpty(); }
	void buildChannels(const AwChannelList& channels);
	inline float lpFilter() { return m_lpFilter; }
	inline float hpFilter() { return m_hpFilter; }
	inline float notchFilter() { return m_notchFilter; }
	inline int numberOfComponents() { return m_icaChannels.size(); }
	QList<AwPanelItem *> createPanelItems();
	inline AwICAPanel *getPanelWidget() { return m_panel; }
	inline int type() { return m_type; }
	inline QStringList& labels() { return m_labels; }
	inline const fmat& mixing() { return m_mixing; }
	inline const fmat& unmixing() { return m_unmixing; }
	void showSEEGMap();
public slots:
	void showPanel();
	void setNewFilters(const AwFilterSettings& settings);
signals:
	/** Sent when the user reject a component **/
	void componentRejected(int index);
	/** Sent when the user undo the rejection of a component **/
	void componentAdded(int index);
	void filteringChecked(bool checked);
protected slots:
	void switchFilteringOn() { emit filteringChecked(true); }
	void updateComponentState(int index, bool rejected);
private:
	int m_type;
	fmat m_unmixing;
	fmat m_mixing;
	QStringList m_labels;
	AwChannelList m_sources;
	AwICAChannelList m_icaChannels;
	QHash<QString, int> m_labelToIndex;
	// filtering applied when computing ICA 
	float m_hpFilter, m_lpFilter, m_notchFilter;
	QList<int> m_rejectedComponents;	// indexes of all rejected ICA components

	void updateRejectedComponents();
	// Thread safe specific
	QMutex m_mutex;
	AwICAPanel *m_panel;
	AwSEEGMap* m_seegMap;
};

#endif // AWICACOMPONENTS_H
