//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  pinv.cpp
//
//  Code generation for function 'pinv'
//


// Include files
#include "pinv.h"
#include "rt_nonfinite.h"
#include "svd.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <math.h>

// Function Definitions
namespace coder
{
  void eml_pinv(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U>
                &X)
  {
    array<double, 2U> U;
    array<double, 2U> V;
    array<double, 1U> s;
    double absx;
    int exponent;
    int n;
    int nx;
    int r;
    n = A.size(1);
    X.set_size(A.size(1), A.size(0));
    nx = A.size(1) * A.size(0);
    for (r = 0; r < nx; r++) {
      X[r] = 0.0;
    }

    if ((A.size(0) != 0) && (A.size(1) != 0)) {
      int k;
      boolean_T p;
      nx = A.size(0) * A.size(1);
      p = true;
      for (k = 0; k < nx; k++) {
        if ((!p) || (rtIsInf(A[k]) || rtIsNaN(A[k]))) {
          p = false;
        }
      }

      if (!p) {
        X.set_size(A.size(1), A.size(0));
        nx = A.size(1) * A.size(0);
        for (r = 0; r < nx; r++) {
          X[r] = rtNaN;
        }
      } else {
        internal::b_svd(A, U, s, V);
        absx = std::abs(s[0]);
        if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
          if (absx <= 2.2250738585072014E-308) {
            absx = 4.94065645841247E-324;
          } else {
            frexp(absx, &exponent);
            absx = std::ldexp(1.0, exponent - 53);
          }
        } else {
          absx = rtNaN;
        }

        absx *= static_cast<double>(A.size(0));
        r = 0;
        k = 0;
        while ((k <= n - 1) && (s[k] > absx)) {
          r++;
          k++;
        }

        if (r > 0) {
          nx = 0;
          for (exponent = 0; exponent < r; exponent++) {
            if (n >= 1) {
              cblas_dscal((MKL_INT)n, 1.0 / s[exponent], &V[nx], (MKL_INT)1);
            }

            nx += n;
          }

          CBLAS_TRANSPOSE b_t;
          CBLAS_TRANSPOSE t;
          t = CblasNoTrans;
          b_t = CblasConjTrans;
          cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)A.size(1), (MKL_INT)A.size
                      (0), (MKL_INT)r, 1.0, &(V.data())[0], (MKL_INT)A.size(1),
                      &(U.data())[0], (MKL_INT)A.size(0), 0.0, &(X.data())[0],
                      (MKL_INT)A.size(1));
        }
      }
    }
  }
}

// End of code generation (pinv.cpp)
