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
#pragma once

#include <QWidget>
#include <AwGlobal.h>
#include <QVariantMap>
namespace Ui { class AwMarkingToolClassUi; };
class AwMarker;

namespace keys {
	constexpr auto auto_increment = "auto increment";
	constexpr auto tag_while_marking = "tag_marking";
	constexpr auto confidence_level = "confidence";
	constexpr auto default_color = "default_color";
	constexpr auto auto_inc_index = "inc_index";
};

class AW_WIDGETS_EXPORT AwMarkingTool : public QWidget
{
	Q_OBJECT

public:
	AwMarkingTool(QWidget *parent = nullptr);
	~AwMarkingTool();
	enum MarkingMode { Default, Custom, HED };

	inline int mode() { return m_mode; }
	void setToDefault();
	void loadCustomList(const QString& path);
	QSharedPointer<AwMarker>& defaultMarker();
	QVariantMap& settings() { return m_settings; }
	QList<QSharedPointer<AwMarker>>& customList() { return m_customMarkers; }

	static AwMarkingTool* instance();
protected slots:
	void changeConfidenceLevel(bool);
	void changeAutoIncrement(bool);
	void changeTagWhileMarking(bool);
	void changeDefaultColor();
	void changeDefaultMarkerTag();
	void changeDefaultValue(double);
	void addMarkerToCustomList();
protected:
	static AwMarkingTool* m_instance;
	int m_mode;
	QList<QSharedPointer<AwMarker>> m_customMarkers;
	QList<QSharedPointer<AwMarker>> m_hedMarkers;
	QSharedPointer<AwMarker> m_defaultMarker;
	QVariantMap m_settings;
private:
	Ui::AwMarkingToolClassUi *ui;

};
