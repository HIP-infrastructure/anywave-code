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
#include <math/AwHilbertTransform.h>
#include <math/AwFFT.h>




void AwHilbertTransform::FHT(vec& data, int direction)
{
	auto N = data.n_elem;

	// forward
	if (direction == AwHilbertTransform::Forward) {

	}
	//	//vec z(N, arma::fill::zeros);
	//	//cx_vec cdata(data, z);
	//	// perform fft
	//	AwMathMutex.lock();
	//	fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, fftx, FFTW_ESTIMATE);
	//	TFMutex.unlock();
	//	auto plan = fftw_plan_r2r()
	//	auto res = arma::fft(data);
	//	for (auto i = 1; i < (N / 2); i++)
	//		res(span(1, (N / 2)) *= 2.0;
	//}
}