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
#include <AwGlobal.h>
#include <AwChannel.h>

class AwFilterGUI;

class AW_FILTER_EXPORT AwFilterSettings : public QObject
{
	Q_OBJECT
public:
	AwFilterSettings(bool noInit = false);
	AwFilterSettings(const AwFilterSettings& copy);

	~AwFilterSettings();
	AwFilterSettings& operator=(const AwFilterSettings& other);

	/** clean up all filter settings and bounds **/
	void clear();
	/** set filters for a specified type of channel **/
	void set(int type, const QVector<float>& values);
	/** convenience method to use a string instead of the channel type **/
	void set(const QString& name, const QVector<float>& values);
	/** set filters for a specified type of channel **/
	void set(int type, float hp, float lp, float notch);
	/** set bounds for a specified type of channel **/
	void setLimits(int type, const QVector<float>& values);
	QVector<float> limits(int type) const { return m_limits.value(type); }
	/** apply the current settings to a channel **/
	void apply(AwChannel *channel) const;
	/** apply the current settings to a whole list of channels **/
	void apply(const AwChannelList& channels) const;
	/** reset all filters settings to zero **/
	void zero();
	/** save settings to json file **/
	void save(const QString& file);
	void load(const QString& file);
	inline int count() const { return m_filters.count(); }
	inline bool isEmpty() const { return m_filters.count() == 0; }
	QList<int> currentTypes() const { return m_filters.keys(); }
	QVector<float> filters(int type) const { return m_filters.value(type); }
	QWidget *ui();
	inline QList<int> guiVisibleItems() { return m_guiVisibleItems; }
	void setUiDocked() { m_uiDocked = true; }
signals:
	void settingsChanged(const AwFilterSettings& settings);
	/** DebugLog complience **/
	void log(const QString& message);
public slots:
	void updateGUI();
	void setGuiVisibleItems(const AwChannelList& channels);
protected slots:
	void setNewSettings(const AwFilterSettings& settings);
protected:
	QMap<int, QVector<float>> m_filters;
	QMap<QString, QVector<float>> m_hash; // key of hash is linked to registeredChannelTypes
	QMap<int, QVector<float>> m_limits; // key of map is linked to registeredChannelTypes
	QList<int> m_guiVisibleItems;	// types of channel to show in GUI
	AwFilterGUI *m_ui;
	bool m_uiDocked;
};

