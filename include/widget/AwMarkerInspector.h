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
#ifndef AWDMARKERINSPECTOR_H
#define AWDMARKERINSPECTOR_H
#include <AwGlobal.h>
#include <QWidget>
namespace Ui {class AwMarkerInspectorUI; };
#include <AwChannel.h>
#include <AwMarker.h>
#include <AwMarkingSettings.h>
#include <QCloseEvent>
#include <memory>
class AW_WIDGETS_EXPORT AwMarkerInspector : public QWidget
{
	Q_OBJECT

public:
	AwMarkerInspector(const AwMarkerList& markers = AwMarkerList(), const QStringList& targets = QStringList(), QWidget *parent = 0);
	~AwMarkerInspector();

	void setSingleOnly(bool flag = true);
	inline AwMarkingSettings& settings() { return m_settings; }
	void setPredefinedMarkers(const AwMarkerList& markers);
	AwMarkerList& predefinedMarkers();
public slots:
	void setMarkers(const AwMarkerList& markers);
	void setTargets(const QStringList& targets);
	void setTargets(AwChannelList& channels);
protected:
	void bindOnLabel(const QString& label);
	void bindOnValue(float value);
	void closeEvent(QCloseEvent *event) { emit closed(); event->accept(); }
	void changeEvent(QEvent*);
signals:
	void settingsChanged(AwMarkingSettings *settings);
	void closed();
	void predefinedMarkersChanged(const AwMarkerList& markers);
protected slots:
	void changeMarkerType(bool flag);
	void changeLabel(const QString& text);
	void changeValue(double value);
	void changeAutoInc(bool flag);
	void changeUsePredefined(bool flag);
	void changeTargetChannels(bool flag);
	void changeUseShortcut(bool flag);
	void addPredefinedMarker();
	void clearPredefinedMarkers();
	void removeSelectedPredefinedMarkers();
	void addTargets();
	void clearTargets();
	void changeBinding(bool on);
	void changeKeyBinding();
	void changeComboBinding();
	void changeColor();

private:
	Ui::AwMarkerInspectorUI *m_ui;
	AwMarkingSettings m_settings;
	AwMarkerList m_markers;
	QStringList m_targetedChannels;
	QStringList m_targets;
	QStringList m_markerLabels;
	QStringList m_markerValues;
	QString m_profilePath;		// path to predefined markers profiles. If empty, no handling of profiles.
	bool m_onlySingle;
};

#endif // AWADDMARKERINSPECTOR_H
