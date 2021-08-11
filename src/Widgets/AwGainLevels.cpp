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
#include <AwGainLevels.h>
#include <QFile>
#include <utils/json.h>

int AwGainLevel::getIndexOfValue(float v)
{
	uvec indexes = find(m_values == v);
	if (indexes.is_empty())
		return -1;
	return indexes(0);
}

int AwGainLevel::insertNewValue(float v)
{
	int index = getIndexOfValue(v);
	if (index == -1) {
		vec tmp = { v };
		m_values = arma::join_cols(m_values, tmp);
		//auto size = m_values.size();
		//m_values.resize(size + 1);
		//m_values(size) = v;
		m_values = sort(m_values);
		
		index = getIndexOfValue(v);
		Q_ASSERT(index != -1);
	}
	m_value = v;
	m_index = index;
	emit valueChanged(type, m_value);
	return index;
}


int AwGainLevel::up()
{
	if (m_index == m_values.size() - 1)
		return m_index;
	m_index++;
	m_value = m_values(m_index);
	emit valueChanged(this->type, m_value);
	return m_index;
}

int AwGainLevel::down()
{
	if (m_index == 0)
		return m_index;
	m_index--;
	m_value = m_values(m_index);
	emit valueChanged(this->type, m_value);
	return m_index;
}

AwGainLevels::AwGainLevels(QObject* parent) : QObject(parent)
{

}

AwGainLevels::AwGainLevels(AwGainLevels* copy, QObject* parent) : QObject(parent)
{
	auto keys = copy->m_levels.keys();
	for (auto const& k : keys) {
		AwGainLevel* gl = copy->m_levels.value(k);
		m_levels.insert(k, new AwGainLevel(gl, this));
	}
}

AwGainLevels::~AwGainLevels()
{
}


AwGainLevel* AwGainLevels::getGainLevelFor(int type)
{
	if (m_levels.contains(type))
		return m_levels.value(type);
	// no gain level for that type => create a default one
	auto gl = createDefaultGainLevel(type);
	m_levels.insert(type, gl);
	return gl;
}

AwGainLevel* AwGainLevels::createDefaultGainLevel(int type)
{
	AwGainLevel* gl = new AwGainLevel(this);
	gl->type = type;
	vec values;
	float value;
	QString unit;
	switch (type)
	{
	case AwChannel::EEG:
	case AwChannel::SEEG:
	case AwChannel::EMG:
	case AwChannel::ECG:
	{  // eeg seeg emg ecg will share the same gain scales and units
		vec a = regspace(0.001, 0.1, 0.9);
		vec b = regspace(1, 2, 9);
		values = join_cols(a, b);
		b = regspace(10, 10, 490);
		values = join_cols(values, b);
		b = regspace(500, 100, 5000);
		values = join_cols(values, b);
		unit = QString::fromUtf8("µV/cm");
		if (type == AwChannel::EEG)
			value = 150;
		else if (type == AwChannel::SEEG)
			value = 300;
		else if (type == AwChannel::EMG)
			value = 400;
		else if (type == AwChannel::ECG)
			value = 450;
		else
			value = 200;
		gl->setValues(values);
		gl->setValue(value);
		gl->setUnit(unit);
	}
	break;
	case AwChannel::ICA:
	{
		vec a = regspace(1, 1, 9);
		vec b = regspace(10, 100, 10000);
		values = join_cols(a, b);
		unit = QString("??/cm");
		value = 310;
		gl->setValues(values);
		gl->setValue(value);
		gl->setUnit(unit);
	}
	break;
	case AwChannel::MEG:
	case AwChannel::GRAD:
	case AwChannel::Reference:
	{
		vec a = regspace(0.01, 0.1, 0.9);
		vec b = regspace(1, 2, 500);
		values = join_cols(a, b);
		if (type == AwChannel::MEG) {
			value = 3;
			unit = QString::fromUtf8("pT/cm");
		}
		else if (type == AwChannel::GRAD) {
			value = 21;
			unit = QString::fromUtf8("pT/cm/cm");
		}
		else {
			unit = QString::fromUtf8("pT/cm");
			value = 23;
		}
		gl->setValues(values);
		gl->setValue(value);
		gl->setUnit(unit);
	}
	break;
	default:
	{
		vec a = regspace(0.001, 0.1, 0.9);
		vec b = regspace(1, 2, 9);
		values = join_cols(a, b);
		b = regspace(10, 10, 490);
		values = join_cols(values, b);
		b = regspace(500, 100, 5000);
		values = join_cols(values, b);
		unit = QString::fromUtf8("??/cm");
		value = 100;
		gl->setValues(values);
		gl->setValue(value);
		gl->setUnit(unit);
	}
	}
	return gl;
}

void AwGainLevels::clear()
{
	for (auto values : m_levels.values()) 
		delete values;
	m_levels.clear();
}

void AwGainLevels::applyTo(const AwChannelList& channels)
{
	if (channels.isEmpty())
		return;
	auto types = AwChannel::getTypes(channels);
	QList<AwGainLevel*> levels;

	for (auto t : types) 
		levels << getGainLevelFor(t);
	for (auto c : channels) {
		int index = types.indexOf(c->type());
		if (index != -1) 
			c->setGain(levels.at(index)->value());
	}
}

