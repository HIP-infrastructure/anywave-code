//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// schur.cpp
//
// Code generation for function 'schur'
//

// Include files
#include "schur.h"
#include "rt_nonfinite.h"
#include "sobi_coder_rtwutil.h"
#include "xdhseqr.h"
#include "xzlarf.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include <cmath>

// Function Definitions
namespace coder {
void schur(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U> &V,
           ::coder::array<double, 2U> &T)
{
  array<double, 1U> tau;
  array<double, 1U> work;
  int nx;
  int rowleft;
  boolean_T p;
  nx = A.size(0) * A.size(1);
  p = true;
  for (rowleft = 0; rowleft < nx; rowleft++) {
    if ((!p) || (std::isinf(A[rowleft]) || std::isnan(A[rowleft]))) {
      p = false;
    }
  }
  if (!p) {
    int i;
    rowleft = A.size(0);
    V.set_size(A.size(0), A.size(1));
    nx = A.size(0) * A.size(1);
    for (i = 0; i < nx; i++) {
      V[i] = rtNaN;
    }
    if ((A.size(0) != 0) && (A.size(1) != 0) && (1 < A.size(0))) {
      int itau;
      int knt;
      itau = 2;
      if (A.size(0) - 2 < A.size(1) - 1) {
        knt = A.size(0) - 1;
      } else {
        knt = A.size(1);
      }
      for (int lastv{0}; lastv < knt; lastv++) {
        for (int b_i{itau}; b_i <= rowleft; b_i++) {
          V[(b_i + V.size(0) * lastv) - 1] = 0.0;
        }
        itau++;
      }
    }
    T.set_size(A.size(0), A.size(1));
    nx = A.size(0) * A.size(1);
    for (i = 0; i < nx; i++) {
      T[i] = rtNaN;
    }
  } else {
    int b_i;
    int b_n;
    int i;
    int itau;
    int knt;
    int lastv;
    int n;
    n = A.size(0);
    T.set_size(A.size(0), A.size(1));
    nx = A.size(0) * A.size(1);
    for (i = 0; i < nx; i++) {
      T[i] = A[i];
    }
    b_n = T.size(0);
    if (T.size(0) < 1) {
      i = 0;
    } else {
      i = T.size(0) - 1;
    }
    tau.set_size(i);
    work.set_size(T.size(0));
    nx = T.size(0);
    for (i = 0; i < nx; i++) {
      work[i] = 0.0;
    }
    i = T.size(0);
    for (b_i = 0; b_i <= i - 2; b_i++) {
      double alpha1;
      int im1n_tmp;
      int in;
      im1n_tmp = b_i * b_n;
      in = (b_i + 1) * b_n;
      alpha1 = T[(b_i + T.size(0) * b_i) + 1];
      if (b_i + 3 < b_n) {
        nx = b_i;
      } else {
        nx = b_n - 3;
      }
      nx = (nx + im1n_tmp) + 2;
      itau = (b_n - b_i) - 2;
      tau[b_i] = 0.0;
      if (itau + 1 > 0) {
        double xnorm;
        if (itau < 1) {
          xnorm = 0.0;
        } else {
          xnorm = cblas_dnrm2((MKL_INT)itau, &T[nx], (MKL_INT)1);
        }
        if (xnorm != 0.0) {
          xnorm = rt_hypotd_snf(alpha1, xnorm);
          if (alpha1 >= 0.0) {
            xnorm = -xnorm;
          }
          if (std::abs(xnorm) < 1.0020841800044864E-292) {
            knt = -1;
            do {
              knt++;
              if (itau >= 1) {
                cblas_dscal((MKL_INT)itau, 9.9792015476736E+291, &T[nx],
                            (MKL_INT)1);
              }
              xnorm *= 9.9792015476736E+291;
              alpha1 *= 9.9792015476736E+291;
            } while (!(std::abs(xnorm) >= 1.0020841800044864E-292));
            if (itau < 1) {
              xnorm = 0.0;
            } else {
              xnorm = cblas_dnrm2((MKL_INT)itau, &T[nx], (MKL_INT)1);
            }
            xnorm = rt_hypotd_snf(alpha1, xnorm);
            if (alpha1 >= 0.0) {
              xnorm = -xnorm;
            }
            tau[b_i] = (xnorm - alpha1) / xnorm;
            if (itau >= 1) {
              cblas_dscal((MKL_INT)itau, 1.0 / (alpha1 - xnorm), &T[nx],
                          (MKL_INT)1);
            }
            for (rowleft = 0; rowleft <= knt; rowleft++) {
              xnorm *= 1.0020841800044864E-292;
            }
            alpha1 = xnorm;
          } else {
            tau[b_i] = (xnorm - alpha1) / xnorm;
            if (itau >= 1) {
              cblas_dscal((MKL_INT)itau, 1.0 / (alpha1 - xnorm), &T[nx],
                          (MKL_INT)1);
            }
            alpha1 = xnorm;
          }
        }
      }
      T[(b_i + T.size(0) * b_i) + 1] = 1.0;
      knt = (b_i + im1n_tmp) + 1;
      if (tau[b_i] != 0.0) {
        boolean_T exitg2;
        lastv = itau + 1;
        nx = knt + itau;
        while ((lastv > 0) && (T[nx] == 0.0)) {
          lastv--;
          nx--;
        }
        nx = b_n;
        exitg2 = false;
        while ((!exitg2) && (nx > 0)) {
          int exitg1;
          rowleft = in + nx;
          itau = rowleft;
          do {
            exitg1 = 0;
            if ((b_n > 0) && (itau <= rowleft + (lastv - 1) * b_n)) {
              if (T[itau - 1] != 0.0) {
                exitg1 = 1;
              } else {
                itau += b_n;
              }
            } else {
              nx--;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      } else {
        lastv = 0;
        nx = 0;
      }
      if ((lastv > 0) && (nx >= 1)) {
        cblas_dgemv(CblasColMajor, CblasNoTrans, (MKL_INT)nx, (MKL_INT)lastv,
                    1.0, &T[in], (MKL_INT)b_n, &T[knt], (MKL_INT)1, 0.0,
                    &(work.data())[0], (MKL_INT)1);
        cblas_dger(CblasColMajor, (MKL_INT)nx, (MKL_INT)lastv, -tau[b_i],
                   &(work.data())[0], (MKL_INT)1, &T[knt], (MKL_INT)1, &T[in],
                   (MKL_INT)b_n);
      }
      internal::reflapack::xzlarf((b_n - b_i) - 1, (b_n - b_i) - 1,
                                  (b_i + im1n_tmp) + 2, tau[b_i], T,
                                  (b_i + in) + 2, b_n, work);
      T[(b_i + T.size(0) * b_i) + 1] = alpha1;
    }
    V.set_size(T.size(0), T.size(1));
    nx = T.size(0) * T.size(1);
    for (i = 0; i < nx; i++) {
      V[i] = T[i];
    }
    if (A.size(0) != 0) {
      knt = A.size(0) - 1;
      for (lastv = n; lastv >= 2; lastv--) {
        itau = (lastv - 1) * n - 1;
        for (b_i = 0; b_i <= lastv - 2; b_i++) {
          V[(itau + b_i) + 1] = 0.0;
        }
        nx = itau - n;
        i = lastv + 1;
        for (b_i = i; b_i <= n; b_i++) {
          V[itau + b_i] = V[nx + b_i];
        }
        i = n + 1;
        for (b_i = i; b_i <= n; b_i++) {
          V[itau + b_i] = 0.0;
        }
      }
      for (b_i = 0; b_i < n; b_i++) {
        V[b_i] = 0.0;
      }
      V[0] = 1.0;
      i = A.size(0) + 1;
      for (lastv = i; lastv <= n; lastv++) {
        itau = (lastv - 1) * n;
        for (b_i = 0; b_i < n; b_i++) {
          V[itau + b_i] = 0.0;
        }
        V[(itau + lastv) - 1] = 1.0;
      }
      if (A.size(0) - 1 >= 1) {
        i = A.size(0) - 2;
        for (lastv = knt; lastv <= i; lastv++) {
          itau = (n + lastv * n) + 1;
          nx = n - 2;
          for (b_i = 0; b_i <= nx; b_i++) {
            V[itau + b_i] = 0.0;
          }
          V[itau + lastv] = 1.0;
        }
        itau = A.size(0) - 2;
        work.set_size(V.size(1));
        nx = V.size(1);
        for (i = 0; i < nx; i++) {
          work[i] = 0.0;
        }
        for (b_i = A.size(0) - 1; b_i >= 1; b_i--) {
          nx = ((n + b_i) + (b_i - 1) * n) + 1;
          if (b_i < n - 1) {
            V[nx - 1] = 1.0;
            internal::reflapack::xzlarf(n - b_i, knt - b_i, nx, tau[itau], V,
                                        nx + n, n, work);
            b_n = (n - b_i) - 1;
            if (b_n >= 1) {
              cblas_dscal((MKL_INT)b_n, -tau[itau], &V[nx], (MKL_INT)1);
            }
          }
          V[nx - 1] = 1.0 - tau[itau];
          for (lastv = 0; lastv <= b_i - 2; lastv++) {
            V[(nx - lastv) - 2] = 0.0;
          }
          itau--;
        }
      }
    }
    internal::reflapack::eml_dlahqr(T, V);
    nx = T.size(0);
    if ((T.size(0) != 0) && (T.size(1) != 0) && (3 < T.size(0))) {
      itau = 4;
      if (T.size(0) - 4 < T.size(1) - 1) {
        knt = T.size(0) - 3;
      } else {
        knt = T.size(1);
      }
      for (lastv = 0; lastv < knt; lastv++) {
        for (b_i = itau; b_i <= nx; b_i++) {
          T[(b_i + T.size(0) * lastv) - 1] = 0.0;
        }
        itau++;
      }
    }
  }
}

} // namespace coder

// End of code generation (schur.cpp)
