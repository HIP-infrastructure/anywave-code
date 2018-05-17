/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWFILTERINGOPTIONS_H
#define AWFILTERINGOPTIONS_H
#include <AwChannel.h>

/*!
 * \brief
 * This class defines the filtering options for the different types of channels that AnyWave can filter.
 * 
 * Note that only MEG, EEG/SEEG and EMG/ECG channels are managed by this class.
 * EEG filtering options are also applied to SEEG channels.
 * EMG filtering options are also applied to ECG channels.
 *
 * \remarks
 * This object is used when a process plug-in is requesting data to AnyWave.
 */
class AW_CORE_EXPORT AwFilteringOptions
{
public:
	AwFilteringOptions(); 
	AwFilteringOptions& operator=(const AwFilteringOptions& other);

	void setFilters(const AwChannelList& channels);
	void setFilters(AwChannel *channel);
	void clear();

	float eegLP, eegHP, megLP, megHP, emgLP, emgHP;
	float eegNotch, megNotch, emgNotch;
};
#endif