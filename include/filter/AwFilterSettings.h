/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Authors: Bruno Colombet, Antoine Ducorps – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//    cenir - antoine.ducorps@umpc.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <AwGlobal.h>
#include <AwChannel.h>
#pragma once
class AW_FILTER_EXPORT AwFilterSettings
{
public:
	explicit AwFilterSettings() {}
	explicit AwFilterSettings(const AwFilterSettings& copy) { m_hash = copy.m_hash; }
	/** set filters for a specified type of channel **/
	void set(int type, const QVector<float>& values);
	/** convenience method to use a string instead of the channel type **/
	void set(const QString& name, const QVector<float>& values);
	/** set filters for a specified type of channel **/
	void set(int type, float hp, float lp, float notch);
	/** apply the current settings to a channel **/
	void apply(AwChannel *channel);
	/** apply the current settings to a whole list of channels **/
	void apply(const AwChannelList& channels);
	/** save settings to json file **/
	void save(const QString& file);
	void load(const QString& file);
	inline int count() const { return m_hash.count(); }
	QList<int> currentTypes() const { return m_hash.keys(); }
	QVector<float> filters(int type) const { return m_hash[type]; }
protected:
	QHash<int, QVector<float>> m_hash;
};

