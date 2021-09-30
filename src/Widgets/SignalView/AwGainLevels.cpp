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
	uvec indexes = find(values == v);
	if (indexes.is_empty())
		return -1;
	return indexes(0);
}

int AwGainLevel::insertNewValue(float v)
{
	int index = getIndexOfValue(v);
	if (index == -1) {
		auto size = values.size();
		values.resize(size + 1);
		values(size) = v;
		sort(values);
		index = getIndexOfValue(v);
	}
	return index;
}


AwGainLevels::AwGainLevels(QObject* parent) : QObject(parent)
{

}

AwGainLevels::AwGainLevels(AwGainLevels* copy, QObject* parent) : QObject(parent)
{
	auto keys = copy->m_levels.keys();
	for (auto const& k : keys) {
		AwGainLevel* gl = copy->m_levels.value(k);
		m_levels.insert(k, new AwGainLevel(gl));
	}
}

AwGainLevels::~AwGainLevels()
{
	auto values = m_levels.values();
	for (auto v : values)
		delete v;
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
	AwGainLevel* gl = new AwGainLevel;
	gl->type = type;
	switch (type)
	{
	case AwChannel::EEG:
	case AwChannel::SEEG:
	case AwChannel::EMG:
	case AwChannel::ECG:
	{  // eeg seeg emg ecg will share the same gain scales and units
		vec a = regspace(0.001, 0.1, 0.9);
		vec b = regspace(1, 2, 9);
		gl->values = join_cols(a, b);
		b = regspace(10, 10, 490);
		gl->values = join_cols(gl->values, b);
		b = regspace(500, 100, 5000);
		gl->values = join_cols(gl->values, b);
		gl->unit = QString::fromLatin1("µV/cm");
		if (type == AwChannel::EEG)
			gl->value = 150;
		else if (type == AwChannel::SEEG)
			gl->value = 300;
		else if (type == AwChannel::EMG)
			gl->value = 400;
		else if (type == AwChannel::ECG)
			gl->value = 450;
		else
			gl->value = 200;
	}
	break;
	case AwChannel::ICA:
	{
		vec a = regspace(1, 1, 9);
		vec b = regspace(10, 100, 10000);
		gl->values = join_cols(a, b);
		gl->unit = QString::fromLatin1("??/cm");
		gl->value = 350;
	}
		break;
	case AwChannel::MEG:
	case AwChannel::GRAD:
	case AwChannel::Reference:
	{
		vec a = regspace(0.01, 0.1, 0.9);
		vec b = regspace(1, 2, 500);
		gl->values = join_cols(a, b);
		if (type == AwChannel::MEG) {
			gl->value = 3;
			gl->unit = QString::fromLatin1("pT/cm");
		}
		else if (type == AwChannel::GRAD) {
			gl->value = 21;
			gl->unit = QString::fromLatin1("pT/cm/cm");
		}
		else {
			gl->unit = QString::fromLatin1("pT/cm");
			gl->value = 23;
		}
	}
	break;
	default:
	{
		vec a = regspace(0.0011, 0.1, 0.9);
		vec b = regspace(1, 2, 9);
		gl->values = join_cols(a, b);
		b = regspace(10, 10, 490);
		gl->values = join_cols(gl->values, b);
		b = regspace(500, 100, 5000);
		gl->values = join_cols(gl->values, b);
		gl->unit = QString::fromLatin1("??/cm");
		gl->value = 100;
	}
	}
	return gl;
}

void AwGainLevels::clear()
{
	m_map.clear();
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
			c->setGain(levels.at(index)->value);
	}
}

