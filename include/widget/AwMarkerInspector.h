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
	AwMarkerInspector(const AwSharedMarkerList& markers = AwSharedMarkerList(), const QStringList& targets = QStringList(), QWidget *parent = 0);
	~AwMarkerInspector();

	void setSingleOnly(bool flag = true);
	inline AwMarkingSettings& settings() { return m_settings; }
	void setPredefinedMarkers(const AwSharedMarkerList& markers);
	AwSharedMarkerList& predefinedMarkers();
public slots:
	void setMarkers(const AwSharedMarkerList& markers);
	void setTargets(const QStringList& targets);
	void setTargets(const AwChannelList& channels);
protected:
	void bindOnLabel(const QString& label);
	void bindOnValue(float value);
	void closeEvent(QCloseEvent *event) { emit closed(); event->accept(); }
	void changeEvent(QEvent*);
signals:
	void settingsChanged(AwMarkingSettings *settings);
	void closed();
	void predefinedMarkersChanged(const AwSharedMarkerList& markers);
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
	void changeColor();

private:
	Ui::AwMarkerInspectorUI *m_ui;
	AwMarkingSettings m_settings;
	AwSharedMarkerList m_markers;
	QStringList m_targetedChannels;
	QStringList m_targets;
	QStringList m_markerLabels;
	QStringList m_markerValues;
	QString m_profilePath;		// path to predefined markers profiles. If empty, no handling of profiles.
	bool m_onlySingle;
};

#endif // AWADDMARKERINSPECTOR_H
