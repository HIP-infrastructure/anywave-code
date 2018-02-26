/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef AWAVGCHANNEL_H
#define AWAVGCHANNEL_H

#include <AwVirtualChannel.h>

class AwAVGChannel : public AwVirtualChannel
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
