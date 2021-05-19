// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef AWFILTERING_H
#define AWFILTERING_H
#include <AwGlobal.h>
#include <AwChannel.h>
#include <filter/AwFilterSettings.h>

class AW_FILTER_EXPORT AwFiltering
{
public:
    static void filter(const AwChannelList& channels);
	static void filter(AwChannelList* channels);
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
