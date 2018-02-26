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
#ifndef AWVIRTUALCHANNEL_H
#define AWVIRTUALCHANNEL_H
#include <AwChannel.h>

class AW_CORE_EXPORT AwVirtualChannel : public AwChannel
{
public:
	AwVirtualChannel() : AwChannel() { m_sourceType = AwChannel::Virtual; m_className = "AwVirtual"; }
	/** Copy constructor **/
	AwVirtualChannel(AwChannel *chan) : AwChannel(chan) { m_sourceType = AwChannel::Virtual; m_className = "AwVirtual"; }
	AwVirtualChannel(AwVirtualChannel *chan);
	virtual ~AwVirtualChannel();
	AwChannelList& connectedChannels() { return m_connectedChannels; }
	void connectChannels(const AwChannelList& channels);
	void connectChannel(AwChannel *channel);
	AwChannel *getConnectedChannel(const QString& name);

	// overide setDataReady from AwChannel
	void setDataReady(bool f = true) { m_dataReady = f; update(); }
	
	virtual void compute() {}	// override this method to compute data for the channel. Default implementation does nothing.
	virtual void update() {}	// override this method to update the data content for the channel. Called each time data are set to the channel.
	virtual void update(float position, float duration) {}	// override this method to compute data for the virtual channel starting at pos position
															// for a duration of duration seconds.
	virtual AwVirtualChannel *duplicate();
protected:
	void clearConnectedChannels();

	AwChannelList m_connectedChannels;	// may be empty
	QHash<QString, AwChannel *> m_channelsFromName; // to quickly retreive a connected channel by its name
};
#endif