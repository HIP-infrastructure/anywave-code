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
#ifndef AWAVGCHANNEL_H
#define AWAVGCHANNEL_H

#include <AwGlobal.h>
#include <AwVirtualChannel.h>

class AW_MONTAGE_EXPORT AwAVGChannel : public AwVirtualChannel
{
public:
	AwAVGChannel(int type);
	AwAVGChannel(AwAVGChannel *c);
	AwAVGChannel(AwChannel *c) : AwVirtualChannel(c) {}
	virtual ~AwAVGChannel();

	AwAVGChannel *duplicate();
	void compute();
private:
	QHash<QString, AwChannel *> m_asRecordedChannels;  // to quickly retreive a connected channel from its name.
};

#endif // AWAVGCHANNEL_H
