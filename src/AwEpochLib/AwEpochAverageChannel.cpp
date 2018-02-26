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
#include <epoch/AwEpochAverageChannel.h>

AwEpochAverageChannel::AwEpochAverageChannel() : AwChannel()
{
	m_className = "AwEpochAVG";
	m_zeroPosition = 0.;
	m_condition = NULL;
}

AwEpochAverageChannel::AwEpochAverageChannel(AwChannel *channel) : AwChannel(channel)
{
	m_className = "AwEpochAVG";
	m_zeroPosition = 0.;
	m_condition = NULL;
}

AwEpochAverageChannel *AwEpochAverageChannel::duplicate()
{
	AwEpochAverageChannel *channel = new AwEpochAverageChannel(this);

	if (!m_data.is_empty())
		channel->setData(fvec(m_data));
	if (!m_errorType.is_empty())
		channel->setErrorType(fvec(m_errorType));
	channel->setZeroPosition(m_zeroPosition);
	channel->setCondition(m_condition);
	return channel;
}


void AwEpochAverageChannel::setData(const fvec& data)
{
	m_data = data;
}

void AwEpochAverageChannel::setErrorType(const fvec& error)
{
	m_errorType = error;
}