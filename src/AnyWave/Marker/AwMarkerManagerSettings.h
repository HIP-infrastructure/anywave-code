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
#ifndef AW_MARKERS_MANAGER_SETTINGS_H
#define AW_MARKERS_MANAGER_SETTINGS_H

#include <QWidget>
#include "ui_AwMarkerManagerSettings.h"
#include "AwMarkerListModel.h"
#include "AwMarkerRuleManageDial.h"
#include "AwMarkerRule.h"
#include <QSortFilterProxyModel>

using namespace Ui;
class AwStatsWidget;
class AwMarkerManager;

class AwMarkerManagerSettings : public QWidget, public MarkersManagerSettingsClass
{
	Q_OBJECT

public:
	AwMarkerManagerSettings(AwMarkerList& markers, QWidget *parent = 0);
	~AwMarkerManagerSettings();
	void applyRule(AwMarkerRule *rule);
	void cleanUp();
	/** Set a new list of markers for the current file **/
	void setMarkers(const AwMarkerList& markers);

public slots:
	void removeRule(const QString& name);
	void setRule(const QString& name);
	void addRule(const QString& name);
	void modifyRule(const QString& oldName, const QString& newName);
	void editCurrentItem();
	void setMarkerAddingMode(bool on);
	void highlightMarker(AwMarker *marker);
	void openStats();

signals:
	/** Sent whenever displayed markers changed, depending on filtering rules **/
	void markersChanged(const AwMarkerList &markers);
	void markersNeedToBeExported(const AwMarkerList& markers);
	void triggerMarkersToWrite(const AwMarkerList& markers);
	void markersRemoved(const AwMarkerList& markers);
	void moveRequest(float pos);
	void loadMarkersClicked();
	void saveMarkersClicked();
	void clearTriggerClicked();


protected slots:
	void renameAllMarkers();
	void renameSelectedMarkers();
	void changeValueAllMarkers();
	void changeValueSelectedMarkers();
	void changeColorSelectedMarkers();
	void changeColorAllMarkers();
	void launchProcess();
	void writeTrigger();
	void clearTrigger();
	void saveSelectedMarkers();
	void saveSelectedMarkersToMATLAB();
	void removeMarkers();
	void removeAllLabels();
	void selectAllLabels();
	void goToMarkerPos();
	void editRules();
	void goToMarkerAtRow(int row);
	void contextMenuRequested(const QPoint&);
	//void updateNamesStats(const QString&);
	//void updateValuesStats(const QString&);
	/** slot called when an item is modified in TableView **/
	void updateMarkerList();
	void prevName();
	void nextName();
	void goToName();
	void prevValue();
	void nextValue();
	void goToValue();
	void showColumn(bool flag);
	void removeDuplicates();

protected:
	//void updateStats();
	// EVENTS
	void changeEvent(QEvent*);

	AwMarkerList m_markers;
	AwMarkerList m_displayedMarkers;
	AwMarkerList m_statsNames;		// list of markers for which name is selected on stats
	AwMarkerList m_statsValues;		// list of markers for which value is selected on stats
	int m_nameIndex;
	int m_valueIndex;
	AwMarkerListModel *m_model;
	QMenu *m_contextMenu;
	QString m_markerDir;
	QStringList m_loadedRules;
	AwMarkerRuleManageDial *m_markerRuleDial;
	AwMarkerRule *m_currentRule;
	QString m_noRuleString;
	QMenu *m_menu;
	QAction *m_removeAllLabel, *m_selectAllLabel;	// keep pointer for specific action Remove all "MARKER" and Select all "MARKER"
	bool m_isAddingMarker;
	AwStatsWidget* m_statsWidget;
	AwMarkerManager* m_mm;  // global instance of Marker Manager
};

#endif // MARKERS_MANAGER_SETTINGS_H
