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
#include <math/AwFFT.h>

#ifdef MKL
#include "fftw/fftw3.h"
#include "fftw/fftw3_mkl.h"
#else
#include <fftw3.h>
#endif

cx_vec AwFFT::fft(float *data, quint64 length)
{
	// optimise length for fft
	auto fftLength = (quint64)pow(2.0, ceil(log((double)length) / log(2.0)));
	// padding with zero before and after the signal


	double *in = (double *)fftw_malloc(sizeof(double) * fftLength);
	fftw_complex *fftx = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * fftLength);
	m_mutex.lock();
	fftw_plan plan = fftw_plan_dft_r2c_1d(fftLength, in, fftx, FFTW_ESTIMATE);
	m_mutex.unlock();
	memset(in, 0, fftLength * sizeof(double));
	for (auto i = 0; i < length; i++)
		in[i] = (double)data[i];
	fftw_execute_dft_r2c(plan, in, fftx);
	m_mutex.lock();
	fftw_destroy_plan(plan);
	m_mutex.unlock();
	fftw_free(in);
	cx_vec res(length);
	for (auto i = 0; i < length; i++) {
		res.at(i) = std::complex<double>(fftx[i][0], fftx[i][1]);
	}
	fftw_free(fftx);
	return res;
}
