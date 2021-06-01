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
#include "qr.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Declarations
static double rt_hypotd_snf(double u0, double u1);

// Function Definitions
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = std::abs(u0);
  y = std::abs(u1);
  if (a < y) {
    a /= y;
    y *= std::sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * std::sqrt(y * y + 1.0);
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

namespace coder
{
  void qr(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U> &Q, ::
          coder::array<double, 2U> &R)
  {
    CBLAS_TRANSPOSE t;
    array<double, 2U> b_A;
    array<double, 1U> tau;
    array<double, 1U> work;
    double atmp;
    double xnorm;
    int coltop;
    int exitg1;
    int i;
    int ia;
    int iaii;
    int ii;
    int lastc;
    int lastv;
    int lda;
    int m;
    int minmana;
    int minmn;
    int n;
    boolean_T exitg2;
    b_A.set_size(A.size(0), A.size(1));
    lastc = A.size(0) * A.size(1);
    for (iaii = 0; iaii < lastc; iaii++) {
      b_A[iaii] = A[iaii];
    }

    m = b_A.size(0) - 1;
    n = b_A.size(1);
    lastc = b_A.size(0);
    minmana = b_A.size(1);
    if (lastc < minmana) {
      minmana = lastc;
    }

    lastc = b_A.size(0);
    minmn = b_A.size(1);
    if (lastc < minmn) {
      minmn = lastc;
    }

    tau.set_size(minmana);
    for (iaii = 0; iaii < minmana; iaii++) {
      tau[iaii] = 0.0;
    }

    if ((b_A.size(0) != 0) && (b_A.size(1) != 0) && (minmn >= 1)) {
      lda = b_A.size(0);
      work.set_size(b_A.size(1));
      lastc = b_A.size(1);
      for (iaii = 0; iaii < lastc; iaii++) {
        work[iaii] = 0.0;
      }

      for (i = 0; i < minmn; i++) {
        ii = i * lda + i;
        lastc = m - i;
        if (i + 1 < m + 1) {
          atmp = b_A[ii];
          tau[i] = 0.0;
          if (lastc + 1 > 0) {
            if (lastc < 1) {
              xnorm = 0.0;
            } else {
              xnorm = cblas_dnrm2((MKL_INT)lastc, &b_A[ii + 1], (MKL_INT)1);
            }

            if (xnorm != 0.0) {
              xnorm = rt_hypotd_snf(b_A[ii], xnorm);
              if (b_A[ii] >= 0.0) {
                xnorm = -xnorm;
              }

              if (std::abs(xnorm) < 1.0020841800044864E-292) {
                minmana = -1;
                do {
                  minmana++;
                  if (lastc >= 1) {
                    cblas_dscal((MKL_INT)lastc, 9.9792015476736E+291, &b_A[ii +
                                1], (MKL_INT)1);
                  }

                  xnorm *= 9.9792015476736E+291;
                  atmp *= 9.9792015476736E+291;
                } while (!(std::abs(xnorm) >= 1.0020841800044864E-292));

                if (lastc < 1) {
                  xnorm = 0.0;
                } else {
                  xnorm = cblas_dnrm2((MKL_INT)lastc, &b_A[ii + 1], (MKL_INT)1);
                }

                xnorm = rt_hypotd_snf(atmp, xnorm);
                if (atmp >= 0.0) {
                  xnorm = -xnorm;
                }

                tau[i] = (xnorm - atmp) / xnorm;
                if (lastc >= 1) {
                  cblas_dscal((MKL_INT)lastc, 1.0 / (atmp - xnorm), &b_A[ii + 1],
                              (MKL_INT)1);
                }

                for (iaii = 0; iaii <= minmana; iaii++) {
                  xnorm *= 1.0020841800044864E-292;
                }

                atmp = xnorm;
              } else {
                tau[i] = (xnorm - b_A[ii]) / xnorm;
                if (lastc >= 1) {
                  cblas_dscal((MKL_INT)lastc, 1.0 / (b_A[ii] - xnorm), &b_A[ii +
                              1], (MKL_INT)1);
                }

                atmp = xnorm;
              }
            }
          }

          b_A[ii] = atmp;
        } else {
          tau[i] = 0.0;
        }

        if (i + 1 < n) {
          atmp = b_A[ii];
          b_A[ii] = 1.0;
          minmana = ii + lda;
          if (tau[i] != 0.0) {
            lastv = lastc + 1;
            lastc += ii;
            while ((lastv > 0) && (b_A[lastc] == 0.0)) {
              lastv--;
              lastc--;
            }

            lastc = (n - i) - 1;
            exitg2 = false;
            while ((!exitg2) && (lastc > 0)) {
              coltop = minmana + (lastc - 1) * lda;
              ia = coltop + 1;
              do {
                exitg1 = 0;
                if (ia <= coltop + lastv) {
                  if (b_A[ia - 1] != 0.0) {
                    exitg1 = 1;
                  } else {
                    ia++;
                  }
                } else {
                  lastc--;
                  exitg1 = 2;
                }
              } while (exitg1 == 0);

              if (exitg1 == 1) {
                exitg2 = true;
              }
            }
          } else {
            lastv = 0;
            lastc = 0;
          }

          if ((lastv > 0) && (lastc >= 1)) {
            t = CblasConjTrans;
            cblas_dgemv(CblasColMajor, t, (MKL_INT)lastv, (MKL_INT)lastc, 1.0,
                        &b_A[minmana], (MKL_INT)lda, &b_A[ii], (MKL_INT)1, 0.0,
                        &(work.data())[0], (MKL_INT)1);
            cblas_dger(CblasColMajor, (MKL_INT)lastv, (MKL_INT)lastc, -tau[i],
                       &b_A[ii], (MKL_INT)1, &(work.data())[0], (MKL_INT)1,
                       &b_A[minmana], (MKL_INT)lda);
          }

          b_A[ii] = atmp;
        }
      }
    }

    m = b_A.size(0);
    n = b_A.size(1);
    lastc = b_A.size(0);
    lda = b_A.size(1);
    if (lastc < lda) {
      lda = lastc;
    }

    R.set_size(lda, b_A.size(1));
    for (minmana = 0; minmana < lda; minmana++) {
      for (i = 0; i <= minmana; i++) {
        R[i + R.size(0) * minmana] = b_A[i + b_A.size(0) * minmana];
      }

      iaii = minmana + 2;
      for (i = iaii; i <= lda; i++) {
        R[(i + R.size(0) * minmana) - 1] = 0.0;
      }
    }

    iaii = b_A.size(0) + 1;
    for (minmana = iaii; minmana <= n; minmana++) {
      for (i = 0; i < lda; i++) {
        R[i + R.size(0) * (minmana - 1)] = b_A[i + b_A.size(0) * (minmana - 1)];
      }
    }

    if (lda >= 1) {
      iaii = lda - 1;
      for (minmana = lda; minmana <= iaii; minmana++) {
        ia = minmana * m;
        lastc = m - 1;
        for (i = 0; i <= lastc; i++) {
          b_A[ia + i] = 0.0;
        }

        b_A[ia + minmana] = 1.0;
      }

      ii = lda - 1;
      work.set_size(b_A.size(1));
      lastc = b_A.size(1);
      for (iaii = 0; iaii < lastc; iaii++) {
        work[iaii] = 0.0;
      }

      for (i = lda; i >= 1; i--) {
        iaii = (i + (i - 1) * m) - 1;
        if (i < lda) {
          b_A[iaii] = 1.0;
          minmana = iaii + m;
          if (tau[ii] != 0.0) {
            lastv = (m - i) + 1;
            lastc = minmana - i;
            while ((lastv > 0) && (b_A[lastc] == 0.0)) {
              lastv--;
              lastc--;
            }

            lastc = lda - i;
            exitg2 = false;
            while ((!exitg2) && (lastc > 0)) {
              coltop = minmana + (lastc - 1) * m;
              ia = coltop + 1;
              do {
                exitg1 = 0;
                if (ia <= coltop + lastv) {
                  if (b_A[ia - 1] != 0.0) {
                    exitg1 = 1;
                  } else {
                    ia++;
                  }
                } else {
                  lastc--;
                  exitg1 = 2;
                }
              } while (exitg1 == 0);

              if (exitg1 == 1) {
                exitg2 = true;
              }
            }
          } else {
            lastv = 0;
            lastc = 0;
          }

          if ((lastv > 0) && (lastc >= 1)) {
            t = CblasConjTrans;
            cblas_dgemv(CblasColMajor, t, (MKL_INT)lastv, (MKL_INT)lastc, 1.0,
                        &b_A[minmana], (MKL_INT)m, &b_A[iaii], (MKL_INT)1, 0.0,
                        &(work.data())[0], (MKL_INT)1);
            cblas_dger(CblasColMajor, (MKL_INT)lastv, (MKL_INT)lastc, -tau[ii],
                       &b_A[iaii], (MKL_INT)1, &(work.data())[0], (MKL_INT)1,
                       &b_A[minmana], (MKL_INT)m);
          }
        }

        if (i < m) {
          n = m - i;
          if (n >= 1) {
            cblas_dscal((MKL_INT)n, -tau[ii], &b_A[iaii + 1], (MKL_INT)1);
          }
        }

        b_A[iaii] = 1.0 - tau[ii];
        for (minmana = 0; minmana <= i - 2; minmana++) {
          b_A[(iaii - minmana) - 1] = 0.0;
        }

        ii--;
      }
    }

    Q.set_size(m, lda);
    for (minmana = 0; minmana < lda; minmana++) {
      for (i = 0; i < m; i++) {
        Q[i + Q.size(0) * minmana] = b_A[i + b_A.size(0) * minmana];
      }
    }
  }
}

// End of code generation (qr.cpp)
