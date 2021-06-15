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
#include <aw_armadillo.h>
#include <AwChannel.h>

/// <summary>
/// AwGainLevel stores the scale of values for gain levels for a type of channel.
/// Scales are stored as vec (armadillo)
/// The current gain value for that type of channel is stored as value.
/// </summary>
class AW_WIDGETS_EXPORT AwGainLevel : public QObject
{
	Q_OBJECT
public:
	AwGainLevel(QObject *parent = nullptr) : QObject(parent) { type = 0; m_value = 0; }
	AwGainLevel(AwGainLevel* copy, QObject *parent = nullptr) : QObject(parent) { type = copy->type; m_value = copy->m_value; m_values = copy->m_values; }

	/** get index in scale for the value, -1 is value is not in the scale.**/
	int getIndexOfValue(float value);
	/** add a new value to the scale, returns the index of the value in the scale **/
	int insertNewValue(float value);
	int type;

	void setValues(const vec& values) { m_values = values; }
	inline vec& values() { return m_values; }
	inline float value() { return m_value; }
	void setValue(float v) { m_value = v; emit valueChanged(type, v); }
	inline QString& unit() { return m_unit; }
	void setUnit(const QString& u) { m_unit = u; }
signals:
	void valueChanged(int channelType, float value);
protected:
	
	vec m_values;   // scale of values
	float m_value;  // current value
	QString m_unit;	// units
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

	void applyTo(const AwChannelList& channels);
	inline QList<AwGainLevel*> gainLevels() { return m_levels.values(); }
	AwGainLevel* getGainLevelFor(int type);
	  
	void clear();


signals:
	void changed();
protected:
	AwGainLevel* createDefaultGainLevel(int type);
	QMap<int, AwGainLevel*> m_levels;
};