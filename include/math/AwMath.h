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
#ifndef AW_MATH_H
#define AW_MATH_H

#include <aw_armadillo.h>
#include <AwGlobal.h>
#include <AwChannel.h>

namespace AwMath {
	fmat AW_MATH_EXPORT channelsToFMat(const AwChannelList& channels);
	mat AW_MATH_EXPORT channelsToMat(const AwChannelList& channels);
	fvec AW_MATH_EXPORT channelToVec(AwChannel *channel);
	bool AW_MATH_EXPORT isNanInChannels(const AwChannelList& channels);
	/** Spectogram MATLAB port. **/
	void AW_MATH_EXPORT spectrogram(fvec& X, vec& window, int noverlap, mat& s);
	void AW_MATH_EXPORT spectrogram(vec& X, vec& window, int noverlap, mat& s);


}

#endif