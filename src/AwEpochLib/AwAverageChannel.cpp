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
#include <epoch/AwAverageChannel.h>

AwAverageChannel::AwAverageChannel() : AwChannel()
{
}

AwAverageChannel::AwAverageChannel(AwChannel *channel) : AwChannel(channel)
{

}

AwAverageChannel *AwAverageChannel::duplicate()
{
	AwAverageChannel *channel = new AwAverageChannel;
	channel->setType(m_type);
	channel->setID(m_ID);
	channel->setGain(m_gain);
	channel->setName(m_name);
	channel->setReferenceName(m_referenceName);
	channel->setColor(m_color);
	channel->setLowFilter(m_lowFilter);
	channel->setHighFilter(m_highFilter);
	channel->setNotch(m_notch);
	channel->setXYZ(m_x, m_y, m_z);
	channel->setOrientationXYZ(m_ox, m_oy, m_oz);

	if (!m_data.is_empty())
		channel->setData(m_data);
	if (!m_errorType.is_empty())
		channel->setErrorType(m_errorType);
	return channel;
}


void AwAverageChannel::setData(const fvec& data)
{
	m_data = data;
	m_mean = arma::mean(data);
	if (!m_errorType.is_empty())
		computeErrorCurves();
}

void AwAverageChannel::setErrorType(const fvec& error)
{
	m_errorType = error;
	if (!m_data.is_empty()) 
		computeErrorCurves();
}


void AwAverageChannel::computeErrorCurves()
{
	m_minus = m_errorType - m_mean;
	m_plus = m_errorType + m_mean;
}