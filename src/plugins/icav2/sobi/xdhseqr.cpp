//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xdhseqr.cpp
//
// Code generation for function 'xdhseqr'
//

// Include files
#include "xdhseqr.h"
#include "rt_nonfinite.h"
#include "xdlanv2.h"
#include "xzlarfg.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include <cmath>

// Function Definitions
namespace coder {
namespace internal {
namespace reflapack {
int eml_dlahqr(::coder::array<double, 2U> &h, ::coder::array<double, 2U> &z)
{
  double v[3];
  double aa;
  double ab;
  double ba;
  double bb;
  double d;
  double rt1r;
  double rt2r;
  double s;
  double tst;
  int info;
  int ldh;
  int ldz;
  int n;
  n = h.size(0);
  ldh = h.size(0);
  ldz = z.size(0);
  info = 0;
  if ((n != 0) && (1 != n)) {
    double SMLNUM;
    double itmax;
    int i;
    int j;
    boolean_T exitg1;
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
    for (j = 0; j <= n - 4; j++) {
      h[(j + h.size(0) * j) + 2] = 0.0;
      h[(j + h.size(0) * j) + 3] = 0.0;
    }
    if (1 <= n - 2) {
      h[(n + h.size(0) * (n - 3)) - 1] = 0.0;
    }
    itmax = 30.0 * std::fmax(10.0, static_cast<double>(n));
    SMLNUM = 2.2250738585072014E-308 *
             (static_cast<double>(n) / 2.2204460492503131E-16);
    i = n - 1;
    exitg1 = false;
    while ((!exitg1) && (i + 1 >= 1)) {
      int L;
      int its;
      int nr;
      boolean_T exitg2;
      boolean_T goto150;
      L = 1;
      goto150 = false;
      its = 0;
      exitg2 = false;
      while ((!exitg2) && (its <= static_cast<int>(itmax))) {
        int k;
        boolean_T exitg3;
        k = i;
        exitg3 = false;
        while ((!exitg3) && (k + 1 > L)) {
          ba = std::abs(h[k + h.size(0) * (k - 1)]);
          if (ba <= SMLNUM) {
            exitg3 = true;
          } else {
            bb = std::abs(h[k + h.size(0) * k]);
            tst = std::abs(h[(k + h.size(0) * (k - 1)) - 1]) + bb;
            if (tst == 0.0) {
              if (k - 1 >= 1) {
                tst = std::abs(h[(k + h.size(0) * (k - 2)) - 1]);
              }
              if (k + 2 <= n) {
                tst += std::abs(h[(k + h.size(0) * k) + 1]);
              }
            }
            if (ba <= 2.2204460492503131E-16 * tst) {
              tst = std::abs(h[(k + h.size(0) * k) - 1]);
              if (ba > tst) {
                ab = ba;
                ba = tst;
              } else {
                ab = tst;
              }
              tst = std::abs(h[(k + h.size(0) * (k - 1)) - 1] -
                             h[k + h.size(0) * k]);
              if (bb > tst) {
                aa = bb;
                bb = tst;
              } else {
                aa = tst;
              }
              s = aa + ab;
              if (ba * (ab / s) <=
                  std::fmax(SMLNUM, 2.2204460492503131E-16 * (bb * (aa / s)))) {
                exitg3 = true;
              } else {
                k--;
              }
            } else {
              k--;
            }
          }
        }
        L = k + 1;
        if (k + 1 > 1) {
          h[k + h.size(0) * (k - 1)] = 0.0;
        }
        if (k + 1 >= i) {
          goto150 = true;
          exitg2 = true;
        } else {
          int m;
          if (its == 10) {
            s = std::abs(h[(k + h.size(0) * k) + 1]) +
                std::abs(h[(k + h.size(0) * (k + 1)) + 2]);
            tst = 0.75 * s + h[k + h.size(0) * k];
            aa = -0.4375 * s;
            ab = s;
            bb = tst;
          } else if (its == 20) {
            s = std::abs(h[i + h.size(0) * (i - 1)]) +
                std::abs(h[(i + h.size(0) * (i - 2)) - 1]);
            tst = 0.75 * s + h[i + h.size(0) * i];
            aa = -0.4375 * s;
            ab = s;
            bb = tst;
          } else {
            tst = h[(i + h.size(0) * (i - 1)) - 1];
            ab = h[i + h.size(0) * (i - 1)];
            aa = h[(i + h.size(0) * i) - 1];
            bb = h[i + h.size(0) * i];
          }
          s = ((std::abs(tst) + std::abs(aa)) + std::abs(ab)) + std::abs(bb);
          if (s == 0.0) {
            rt1r = 0.0;
            ab = 0.0;
            rt2r = 0.0;
            aa = 0.0;
          } else {
            tst /= s;
            ab /= s;
            aa /= s;
            bb /= s;
            ba = (tst + bb) / 2.0;
            tst = (tst - ba) * (bb - ba) - aa * ab;
            ab = std::sqrt(std::abs(tst));
            if (tst >= 0.0) {
              rt1r = ba * s;
              rt2r = rt1r;
              ab *= s;
              aa = -ab;
            } else {
              rt1r = ba + ab;
              rt2r = ba - ab;
              if (std::abs(rt1r - bb) <= std::abs(rt2r - bb)) {
                rt1r *= s;
                rt2r = rt1r;
              } else {
                rt2r *= s;
                rt1r = rt2r;
              }
              ab = 0.0;
              aa = 0.0;
            }
          }
          m = i - 1;
          exitg3 = false;
          while ((!exitg3) && (m >= k + 1)) {
            tst = h[m + h.size(0) * (m - 1)];
            ba = h[(m + h.size(0) * (m - 1)) - 1];
            bb = ba - rt2r;
            s = (std::abs(bb) + std::abs(aa)) + std::abs(tst);
            tst /= s;
            v[0] = (tst * h[(m + h.size(0) * m) - 1] + (ba - rt1r) * (bb / s)) -
                   ab * (aa / s);
            v[1] = tst * (((ba + h[m + h.size(0) * m]) - rt1r) - rt2r);
            v[2] = tst * h[(m + h.size(0) * m) + 1];
            s = (std::abs(v[0]) + std::abs(v[1])) + std::abs(v[2]);
            v[0] /= s;
            v[1] /= s;
            v[2] /= s;
            if ((m == k + 1) ||
                (std::abs(h[(m + h.size(0) * (m - 2)) - 1]) *
                     (std::abs(v[1]) + std::abs(v[2])) <=
                 2.2204460492503131E-16 * std::abs(v[0]) *
                     ((std::abs(h[(m + h.size(0) * (m - 2)) - 2]) +
                       std::abs(ba)) +
                      std::abs(h[m + h.size(0) * m])))) {
              exitg3 = true;
            } else {
              m--;
            }
          }
          for (int b_k{m}; b_k <= i; b_k++) {
            nr = (i - b_k) + 2;
            if (3 < nr) {
              nr = 3;
            }
            if (b_k > m) {
              int hoffset;
              hoffset = (b_k + ldh * (b_k - 2)) - 1;
              for (j = 0; j < nr; j++) {
                v[j] = h[j + hoffset];
              }
            }
            tst = v[0];
            bb = xzlarfg(nr, &tst, v);
            v[0] = tst;
            if (b_k > m) {
              h[(b_k + h.size(0) * (b_k - 2)) - 1] = tst;
              h[b_k + h.size(0) * (b_k - 2)] = 0.0;
              if (b_k < i) {
                h[(b_k + h.size(0) * (b_k - 2)) + 1] = 0.0;
              }
            } else if (m > k + 1) {
              h[(b_k + h.size(0) * (b_k - 2)) - 1] =
                  h[(b_k + h.size(0) * (b_k - 2)) - 1] * (1.0 - bb);
            }
            s = v[1];
            tst = bb * v[1];
            if (nr == 3) {
              d = v[2];
              ba = bb * v[2];
              for (j = b_k; j <= n; j++) {
                aa = (h[(b_k + h.size(0) * (j - 1)) - 1] +
                      s * h[b_k + h.size(0) * (j - 1)]) +
                     d * h[(b_k + h.size(0) * (j - 1)) + 1];
                h[(b_k + h.size(0) * (j - 1)) - 1] =
                    h[(b_k + h.size(0) * (j - 1)) - 1] - aa * bb;
                h[b_k + h.size(0) * (j - 1)] =
                    h[b_k + h.size(0) * (j - 1)] - aa * tst;
                h[(b_k + h.size(0) * (j - 1)) + 1] =
                    h[(b_k + h.size(0) * (j - 1)) + 1] - aa * ba;
              }
              if (b_k + 3 < i + 1) {
                nr = b_k + 2;
              } else {
                nr = i;
              }
              for (j = 0; j <= nr; j++) {
                aa = (h[j + h.size(0) * (b_k - 1)] +
                      s * h[j + h.size(0) * b_k]) +
                     d * h[j + h.size(0) * (b_k + 1)];
                h[j + h.size(0) * (b_k - 1)] =
                    h[j + h.size(0) * (b_k - 1)] - aa * bb;
                h[j + h.size(0) * b_k] = h[j + h.size(0) * b_k] - aa * tst;
                h[j + h.size(0) * (b_k + 1)] =
                    h[j + h.size(0) * (b_k + 1)] - aa * ba;
              }
              for (j = 0; j < n; j++) {
                ab = z[j + z.size(0) * (b_k - 1)];
                aa = (ab + s * z[j + z.size(0) * b_k]) +
                     d * z[j + z.size(0) * (b_k + 1)];
                z[j + z.size(0) * (b_k - 1)] = ab - aa * bb;
                z[j + z.size(0) * b_k] = z[j + z.size(0) * b_k] - aa * tst;
                z[j + z.size(0) * (b_k + 1)] =
                    z[j + z.size(0) * (b_k + 1)] - aa * ba;
              }
            } else if (nr == 2) {
              for (j = b_k; j <= n; j++) {
                ab = h[(b_k + h.size(0) * (j - 1)) - 1];
                aa = ab + s * h[b_k + h.size(0) * (j - 1)];
                h[(b_k + h.size(0) * (j - 1)) - 1] = ab - aa * bb;
                h[b_k + h.size(0) * (j - 1)] =
                    h[b_k + h.size(0) * (j - 1)] - aa * tst;
              }
              for (j = 0; j <= i; j++) {
                aa = h[j + h.size(0) * (b_k - 1)] + s * h[j + h.size(0) * b_k];
                h[j + h.size(0) * (b_k - 1)] =
                    h[j + h.size(0) * (b_k - 1)] - aa * bb;
                h[j + h.size(0) * b_k] = h[j + h.size(0) * b_k] - aa * tst;
              }
              for (j = 0; j < n; j++) {
                ab = z[j + z.size(0) * (b_k - 1)];
                aa = ab + s * z[j + z.size(0) * b_k];
                z[j + z.size(0) * (b_k - 1)] = ab - aa * bb;
                z[j + z.size(0) * b_k] = z[j + z.size(0) * b_k] - aa * tst;
              }
            }
          }
          its++;
        }
      }
      if (!goto150) {
        info = i + 1;
        exitg1 = true;
      } else {
        if ((L != i + 1) && (L == i)) {
          s = h[(i + h.size(0) * i) - 1];
          d = h[i + h.size(0) * (i - 1)];
          tst = h[i + h.size(0) * i];
          xdlanv2(&h[(i + h.size(0) * (i - 1)) - 1], &s, &d, &tst, &ab, &aa,
                  &ba, &bb, &rt2r, &rt1r);
          h[(i + h.size(0) * i) - 1] = s;
          h[i + h.size(0) * (i - 1)] = d;
          h[i + h.size(0) * i] = tst;
          if (n > i + 1) {
            nr = (n - i) - 1;
            if (nr >= 1) {
              cblas_drot((MKL_INT)nr, &h[(i + (i + 1) * ldh) - 1], (MKL_INT)ldh,
                         &h[i + (i + 1) * ldh], (MKL_INT)ldh, rt2r, rt1r);
            }
          }
          if (i - 1 >= 1) {
            cblas_drot((MKL_INT)(i - 1), &h[(i - 1) * ldh], (MKL_INT)1,
                       &h[i * ldh], (MKL_INT)1, rt2r, rt1r);
          }
          if (n >= 1) {
            cblas_drot((MKL_INT)n, &z[(i - 1) * ldz], (MKL_INT)1, &z[i * ldz],
                       (MKL_INT)1, rt2r, rt1r);
          }
        }
        i = L - 2;
      }
    }
  }
  return info;
}

} // namespace reflapack
} // namespace internal
} // namespace coder

// End of code generation (xdhseqr.cpp)
