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
#include <epoch/AwEpoch.h>
#include <epoch/AwEpochTree.h>

AwEpoch::AwEpoch(AwEpochTree *condition, AwMarker *marker)
{
	m_posAndDuration = marker;
	m_rejected = m_loaded = false;
	m_channels = AwChannel::duplicateChannels(condition->channels());
	m_condition = condition;
	m_maxAmplitude = 0.;
	m_zeroSample = (int)floor(condition->preLatency() * m_channels.first()->samplingRate());
}

AwEpoch::~AwEpoch()
{
	if (m_posAndDuration)
		delete m_posAndDuration;
	while (!m_channels.isEmpty())
		delete m_channels.takeFirst();
}

void AwEpoch::computeMaxAmplitude()
{
	// compute only if data loaded.
	if (m_channels.isEmpty())
		return;
	if (m_channels.first()->dataSize() == 0)
		return;

	// compute the auto gain level
	float min = 0., max = 0.;
	for (auto c : m_channels)
		for (auto i = 0; i < c->dataSize(); i++) {
			if (c->data()[i] < min)
				min = c->data()[i];
			if (c->data()[i] > max)
				max = c->data()[i];
		}
	m_maxAmplitude = std::max(std::abs(min), std::abs(max));
}