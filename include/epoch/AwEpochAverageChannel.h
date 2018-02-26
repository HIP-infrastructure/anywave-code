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

#include <AwChannel.h>
#include <AwEpoching.h>
#include <aw_armadillo.h>

class AwEpochTree;

class AW_EPOCH_EXPORT AwEpochAverageChannel : public AwChannel
{
public:
	AwEpochAverageChannel();
	AwEpochAverageChannel(AwChannel *channel);

	AwEpochAverageChannel *duplicate();
	void setData(const fvec& data);
	void setErrorType(const fvec& error);
	fvec& dataVector() { return m_data; }
	float *data() override { return m_data.memptr(); }
	qint64 dataSize() override { return (qint64)m_data.n_elem; }
	fvec& error() { return m_errorType;  }
	void setZeroPosition(float pos) { m_zeroPosition = pos; }
	inline float zeroPosition() { return m_zeroPosition; }
	void setCondition(AwEpochTree *condition) { m_condition = condition; }
protected:
	fvec m_data;
	fvec m_errorType;
	float m_zeroPosition;
	AwEpochTree *m_condition;	// pointer to associated condition (can be NULL)
};

typedef  QList<AwEpochAverageChannel *> AwEpochChannelList;