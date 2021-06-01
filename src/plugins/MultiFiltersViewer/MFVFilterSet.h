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

// class designed to represent a filtering set 
// contains HP,LP,Notch filtering settings and associated color

#include <QString>

class MFVFilterSet
{
public:
	explicit MFVFilterSet() { m_hp = m_lp = m_notch = 0.; }

	inline QString& color() { return m_color; }
	inline float hp() { return m_hp; }
	inline float lp() { return m_lp; }
	inline float notch() { return m_notch; }
	void setHP(float hp) { m_hp = hp; }
	void setLP(float lp) { m_lp = lp; }
	void setNotch(float notch) { m_notch = notch; }
	void setColor(const QString& color) { m_color = color; }
protected:
	float m_lp, m_hp, m_notch;
	QString m_color;
};

using MFVFilterSets = QList<MFVFilterSet *>;