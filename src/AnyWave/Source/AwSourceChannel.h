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
#ifndef AW_SOURCE_CHANNEL_H
#define AW_SOURCE_CHANNEL_H

#include <AwVirtualChannel.h>

class AwSourceChannel : public AwVirtualChannel
{
public:
	AwSourceChannel();
	AwSourceChannel(AwSourceChannel *c);
	AwSourceChannel(AwChannel *c) : AwVirtualChannel(c) {}
	~AwSourceChannel();
	inline int index() { return m_index; }
	inline void setIndex(int index) { m_index = index; }
	inline void setSubType(int type) { m_subType = type; }
	inline int subType() { return m_subType; }
	AwSourceChannel *duplicate(); 
//	void setAutoScale(bool flag) { m_autoYScale = flag; }
//	void update();
protected:
	int m_index;	// index in components matrix
	int m_subType;	// type of real channel used to compute this one.
//	bool m_autoYScale;
};

typedef QList<AwSourceChannel *> AwSourceChannelList;

#endif // AW_SOURCE_CHANNEL_H


