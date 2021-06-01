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

class AW_FILTER_EXPORT AwFilterBounds
{
public:
	explicit AwFilterBounds(int t = 0, float hp = 0., float lp = 0.);
	AwFilterBounds& operator=(const AwFilterBounds& other);

	int type;
	float bounds[2];
};

class AW_FILTER_EXPORT AwFilterSettings : public QObject
{
	Q_OBJECT
public:
	AwFilterSettings();
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
	void setBounds(int type, const QVector<float>& values);
	void setBounds(int type, float hp, float lp);
	void setFilterBounds(int type, const AwFilterBounds& bounds);
	QHash<QString, AwFilterBounds> filterBounds() const { return m_filterBounds; }
	/** check for bounds **/
	QList<int> checkForBounds();
	/** apply the current settings to a channel **/
	void apply(AwChannel *channel) const;
	/** apply the current settings to a whole list of channels **/
	void apply(const AwChannelList& channels) const;
	/** init from previously saved json file for a specified data file **/
	bool initWithFile(const QString& filePath);
	/** init from a list of channels **/
	void initWithChannels(const AwChannelList& channels);
	/** reset all filters settings to zero **/
	void zero();
	/** save settings to json file **/
	void save(const QString& file);
	void load(const QString& file);
	inline int count() const { return m_hash.count(); }
	inline bool isEmpty() const { return m_hash.count() == 0; }
	QList<int> currentTypes() const { return m_hash.keys(); }
	QVector<float> filters(int type) const { return m_hash[type]; }

	QWidget *ui();
	void setUiDocked() { m_uiDocked = true; }
signals:
	void settingsChanged(const AwFilterSettings& settings);
	/** DebugLog complience **/
	void log(const QString& message);
public slots:
	void updateGUI();
protected slots:
	void setNewSettings(const AwFilterSettings& settings);
protected:
	QHash<int, QVector<float>> m_hash;
	QHash<int, QVector<float>> m_bounds;	// bounds holds the frequency boundaries for virtual channels like ICA/Source.
	
	QHash<QString, AwFilterBounds> m_filterBounds;
	// AwFilterBounds contains the target channel type which should be constrained.
	// This container is only used to display pertinent infomation in the GUI. 
	// However, we must use the setFilterBounds method instead of setBounds. setFilterBounds will call setBounds so the checking of frequency bands will operate
	// as previously.
	AwFilterGUI *m_ui;
	bool m_uiDocked;
};

