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
#ifndef FFT_H
#define FFT_H
#ifdef MKL
#include <fftw/fftw3.h>
#else
#include "fftw3.h"
#endif
#include <qmutex.h>
#include <vector>
using std::vector;
#include <complex>
using std::complex;
#include <AwGlobal.h>
//
//namespace AwMath {
//	class AW_MATH_EXPORT FFT {
//	private:
//		fftw_plan m_plan;
//		QMutex m_mutex;
//		unsigned int m_dataLength;
//		vector<double> m_space;
//
//		void init();
//		void exec_r2c(double *in, fftw_complex *out);
//	public:
//		FFT();
//		FFT(unsigned int length);
//		void setDataLength(int length);
//		void exec(double *in, complex<double> *out);
//		void exec(unsigned int in_samples, double *in, complex<double> *out);
//	};
//}
#endif