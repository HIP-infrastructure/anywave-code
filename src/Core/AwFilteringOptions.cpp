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

#include <AwFilteringOptions.h>
////////////////////////////////////////////////////////////////////////////////////////////////////

AwFilteringOptions::AwFilteringOptions()
{
	eegLP = eegHP = megLP = megHP = emgLP = emgHP = 0.; 
	eegNotch = megNotch = emgNotch = 0.;
}

AwFilteringOptions& AwFilteringOptions::operator=(const AwFilteringOptions& other)
{
	if (this == &other)
		return *this;

	eegHP = other.eegHP;
	eegLP = other.eegLP;
	megHP = other.megHP;
	megLP = other.megLP;
	emgHP = other.emgHP;
	emgLP = other.emgLP;
	eegNotch = other.eegNotch;
	megNotch = other.megNotch;
	emgNotch = other.emgNotch;

	return *this;
}


void AwFilteringOptions::clear()
{
	eegLP = eegHP = megLP = megHP = emgLP = emgHP = 0.;
	eegNotch = megNotch = emgNotch = 0.;
}

void AwFilteringOptions::setFilters(AwChannel *channel)
{
	switch (channel->type()) {
	case AwChannel::EEG:
	case AwChannel::SEEG:
		channel->setHighFilter(eegHP);
		channel->setLowFilter(eegLP);
		channel->setNotch(eegNotch);
		break;
	case AwChannel::MEG:
	case AwChannel::GRAD:
		channel->setHighFilter(megHP);
		channel->setLowFilter(megLP);
		channel->setNotch(megNotch);
		break;
	case AwChannel::EMG:
	case AwChannel::ECG:
		channel->setHighFilter(emgHP);
		channel->setLowFilter(emgLP);
		channel->setNotch(emgNotch);
		break;
	}
}

void AwFilteringOptions::setFilters(const AwChannelList& channels)
{
	for (auto c : channels) 
		setFilters(c);
}