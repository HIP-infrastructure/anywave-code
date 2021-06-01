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
#include <matlab/AwMATLAB.h>
#include <math/AwMath.h>


int AwMATLAB::saveToMatlab(const QString& fileName, const AwChannelList& channels)
{
	AwMATLABFile file;
	int status = file.create(fileName);
	if (status != 0)
		return status;

	QStringList labels = AwChannel::getLabels(channels);
	status = file.writeStringCellArray(QString("electrodes"), labels);
	if (status != 0) {
		file.close();
		return status;
	}

	// data matrix
	size_t matrixDims[2] = { size_t(channels.size()), size_t(channels.first()->dataSize()) };
	arma::fmat matrix = AwMath::channelsToFMat(channels).t();
	status = file.writeMatrix(QString("data"), matrix);
	if (status != 0) {
		file.close(); 
		return status;
	}
	file.close();
	return 0;
}
