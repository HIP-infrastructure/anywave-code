/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
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

#include <AwMarker.h>
#include <AwChannel.h>
#include <aw_armadillo.h>


class AwEpochTree;
class AW_EPOCH_EXPORT AwEpoch
{
public:
	AwEpoch(AwEpochTree *condition, AwMarker *marker);
	~AwEpoch();

	enum Tags { Average = 1 };

	inline AwEpochTree *condition() { return m_condition; }
	inline bool isRejected() { return m_rejected; }
	inline void setRejected(bool flag = true) { m_rejected = flag; }
	inline bool isLoaded() { return m_loaded; }
	inline void setLoaded(bool flag = true) { m_loaded = flag; }
	AwChannelList& channels() { return m_channels; }
	inline AwMarker *posAndDuration() { return m_posAndDuration; }
protected:
	AwMarker *m_posAndDuration;
	bool m_rejected, m_loaded;
	int m_tags;
	AwEpochTree *m_condition;
	AwChannelList m_channels;	// The epoch holds a copy of channels set in EpochTree.
};

typedef QList<AwEpoch *> AwEpochList;