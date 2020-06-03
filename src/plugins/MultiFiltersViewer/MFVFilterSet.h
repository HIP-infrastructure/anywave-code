/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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