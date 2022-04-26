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
#include "../ica.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "sobi_coder.h"

// void ICA::run_sobi(int m, int n, int m_nComp)
// {
// 	coder::array<double, 2U> data;
// 	coder::array<double, 2U> mixing;
// 	coder::array<double, 2U> unmixing;

// 	data.set_size(m, n);
// 	// Loop over the array to initialize each element.
// 	for (coder::SizeType idx0 = 0; idx0 < data.size(0); idx0++) {
// 		for (coder::SizeType idx1 = 0; idx1 < data.size(1); idx1++) {
// 			data[idx0 + data.size(0) * idx1] = m_channels.at(idx0)->data()[idx1];
// 		}
// 	}
// 	// saving memory by clearing loaded data
// 	for (AwChannel* c : m_channels)
// 		c->clearData();
// 	sobi_coder(data, m_nComp,  mixing, unmixing);
// 	m_mixing = arma::mat(mixing.data(), mixing.size(0), mixing.size(1));
// 	m_unmixing = arma::mat(unmixing.data(), unmixing.size(0), unmixing.size(1));
// }