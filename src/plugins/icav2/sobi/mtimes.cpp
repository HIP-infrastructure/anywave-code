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

// Include files
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "mkl_cblas.h"

// Function Definitions
namespace coder
{
  namespace internal
  {
    namespace blas
    {
      void mtimes(const ::coder::array<double, 2U> &A, const ::coder::array<
                  double, 2U> &B, ::coder::array<double, 2U> &C)
      {
        if ((A.size(0) == 0) || (A.size(1) == 0) || (B.size(0) == 0) || (B.size
             (1) == 0)) {
          int loop_ub;
          C.set_size(A.size(0), B.size(1));
          loop_ub = A.size(0) * B.size(1);
          for (int i = 0; i < loop_ub; i++) {
            C[i] = 0.0;
          }
        } else {
          CBLAS_TRANSPOSE b_t;
          CBLAS_TRANSPOSE t;
          C.set_size(A.size(0), B.size(1));
          t = CblasNoTrans;
          b_t = CblasNoTrans;
          cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)A.size(0), (MKL_INT)B.size
                      (1), (MKL_INT)A.size(1), 1.0, &(((::coder::array<double,
            2U> *)&A)->data())[0], (MKL_INT)A.size(0), &(((::coder::array<double,
            2U> *)&B)->data())[0], (MKL_INT)B.size(0), 0.0, &(C.data())[0],
                      (MKL_INT)A.size(0));
        }
      }
    }
  }
}

// End of code generation (mtimes.cpp)
