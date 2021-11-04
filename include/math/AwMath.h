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
#ifndef AW_MATH_H
#define AW_MATH_H

#include <aw_armadillo.h>
#include <AwGlobal.h>
#include <AwChannel.h>
#include <QMutex>
namespace AwMath {
	fmat AW_MATH_EXPORT channelsToFMat(const AwChannelList& channels);
	mat AW_MATH_EXPORT channelsToMat(const AwChannelList& channels);
	fvec AW_MATH_EXPORT channelToFVec(AwChannel *channel);
	vec AW_MATH_EXPORT channelToVec(AwChannel *channel);
	QVector<double> AW_MATH_EXPORT vecToQVector(vec& X);
	bool AW_MATH_EXPORT isNanInChannels(const AwChannelList& channels);
	vec AW_MATH_EXPORT psd(arma::colvec& signal, int windowSize, int overlap);
	static QMutex FFTMutex;
	namespace ica {
		mat AW_MATH_EXPORT pca(mat& data, int ncomp);
	}
}

#endif