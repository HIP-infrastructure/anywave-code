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
#include <QObject>
#include <AwGlobal.h>
#include <QVariantMap>
#include <aw_armadillo.h>
#include <AwChannel.h>

/// <summary>
/// AwGainLevel stores the scale of values for gain levels for a type of channel.
/// Scales are stored as vec (armadillo)
/// The current gain value for that type of channel is stored as value.
/// </summary>
class AW_WIDGETS_EXPORT AwGainLevel
{
public:
	AwGainLevel() { type = 0; value = 0; }
	AwGainLevel(AwGainLevel* copy) { type = copy->type; value = copy->value; values = copy->values; }

	/** get index in scale for the value, -1 is value is not in the scale.**/
	int getIndexOfValue(float value);
	/** add a new value to the scale, returns the index of the value in the scale **/
	int insertNewValue(float value);

	int type;
	vec values;   // scale of values
	float value;  // current value
	QString unit;	// units
};

/// <summary>
/// AwGainLevels handles all the possible GainLevel for all types of channels
/// </summary>
class AW_WIDGETS_EXPORT AwGainLevels : public QObject
{
	Q_OBJECT
public:
	AwGainLevels(QObject* parent = nullptr);
	AwGainLevels(AwGainLevels *copy, QObject* parent = nullptr);
	~AwGainLevels();

//	void init(const QString& lvlFilePath);

	void applyTo(const AwChannelList& channels);
	QList<AwGainLevel*> gainLevels() { return m_levels.values(); }
	AwGainLevel* getGainLevelFor(int type);
	  

	void clear();
protected:
	//void load(const QString& path);
	//void save(const QString& path);
	
	AwGainLevel* createDefaultGainLevel(int type);

	QVariantMap m_map;
//	QString m_lvlPath;
	QMap<int, AwGainLevel*> m_levels;
};