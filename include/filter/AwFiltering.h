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
#ifndef AWFILTERING_H
#define AWFILTERING_H
#include <AwGlobal.h>
#include <AwChannel.h>
#include <AwFilteringOptions.h>

#include <filter/AwFilterSettings.h>

class AW_FILTER_EXPORT AwFiltering
{
public:
    static void filter(const AwChannelList& channels);
	static void filter(AwChannelList* channels);
	static void filter(AwChannelList* channels, AwFilteringOptions *fo);
	/** down sample the data of a channel, to the new specified rate **/
	static void downSample(AwChannel *channel, float newSamplingRate);
	/** down sample the data of all channels, to the new specified rate **/
	static void downSample(const AwChannelList& channels, float newSamplingRate);
	/** down sample the data of all channels dividing the sampling rate by a factor **/
	static void downSample(const AwChannelList& channels, int factor);
	
	static void decimate(const AwChannelList& channels, int factor);
	static QVector<float> pad_left(AwChannel *c);
	static QVector<float> pad_right(AwChannel *c);
};
#endif
