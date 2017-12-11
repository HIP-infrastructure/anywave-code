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
#include "sigpack.h"
#ifdef MKL
#include "fftw/fftw3.h"
#else
#include <fftw3.h>
#endif

namespace AwMath {
	// Implementation based on MATLAB
	void spectrogram(fvec& X, vec& window, int noverlap, mat& s)
	{
		vec x = conv_to<vec>::from(X);
		spectrogram(x, window, noverlap, s);
	}

	void spectrogram(vec& X, vec& window, int noverlap, mat& s)
	{
		// convert X to colvec
		colvec x = conv_to<colvec>::from(X);
		uword nx = x.n_elem;
		uword nwind = window.n_elem;
		uword ncol = floor((nx - noverlap) / (nwind - noverlap));
		mat xin = zeros(nwind, ncol);

		// build the matrix with windows of x in columns
		uword overlap = 0;
		for (uword i = 0; i < ncol; i++) {
			xin.col(i) = x.subvec(overlap, overlap + nwind - 1);
			// apply window to column
			xin.col(i) = xin.col(i) % window;
			overlap +=  nwind - overlap;
		}

		// compute FFT for all the columns
		uword half_n = floor(nwind / 2) + 1;
		fftw_complex *out = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * nwind);
		double *in = xin.colptr(0);
		fftw_plan plan = fftw_plan_dft_r2c_1d(nwind, in, out, FFTW_ESTIMATE);

		s = mat(half_n, ncol);
		cx_vec output = cx_vec(half_n);

		for (uword i = 0; i < ncol; i++) {
			in = xin.colptr(i);
			fftw_execute_dft_r2c(plan, in, out);
			memcpy(output.memptr(), out, half_n * sizeof(fftw_complex));
			output /= nwind;
			// compute modulus ^2
			output.for_each([](cx_vec::elem_type &val) { val = val * conj(val); });
			vec col = conv_to<vec>::from(output);
			// multiply all values except the first by 2.
			col *= 2.;
			col(0) = col(0) / 2.;
			// fill corresponding column in matrix s
			s.col(i) = col;
		}
		fftw_destroy_plan(plan);
		fftw_free(out);
	}
}