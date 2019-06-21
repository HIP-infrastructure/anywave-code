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
#include <math/AwMath.h>
#include <QtMath>
#include <cmath>
#include <iostream>
#include <AwChannel.h>

namespace AwMath {
	fmat channelsToFMat(const AwChannelList& channels)
	{
		qint64 dataLength = channels.first()->dataSize();
		fmat res(dataLength, channels.size());
		for (int i = 0; i < channels.size(); i++) 
			memcpy(res.colptr(i), channels.at(i)->data(), dataLength * sizeof(float));
		return res.t();
	}


	mat channelsToMat(const AwChannelList& channels) 
	{
		uword cols = channels.first()->dataSize();
		uword rows = channels.size();
		mat res(rows, cols);
		for (qint64 r = 0; r < res.n_rows; r++) {
			AwChannel *channel = channels.at(r);
			for (qint64 c = 0; c < res.n_cols; c++)
				res(r, c) = channel->data()[c];
		}
		return res;
	}
	fvec channelToFVec(AwChannel *channel)
	{
		fvec vec(channel->dataSize());
		memcpy(vec.memptr(), channel->data(), channel->dataSize() * sizeof(float));
		return vec;
	}
	vec channelToVec(AwChannel *channel)
	{
		vec res(channel->dataSize());
		//memcpy(vec.memptr(), channel->data(), channel->dataSize() * sizeof(float));
		for (qint64 i = 0; i < channel->dataSize(); i++)
			res(i) = (double)channel->data()[i];
		return res;
	}
	QVector<double> vecToQVector(vec& x)
	{
		QVector<double> res(x.n_elem);
		for (auto i = 0; i < x.n_elem; i++)
			res[i] = x(i);
		return res;
	}

	bool isNanInChannels(const AwChannelList& channels)
	{
		foreach(AwChannel *c, channels) {
			qint64 count = 0;
			while (count < c->dataSize()) {
				if (std::isnan(c->data()[count++]))
					return true;
			}
		}
		return false;
	}
}

