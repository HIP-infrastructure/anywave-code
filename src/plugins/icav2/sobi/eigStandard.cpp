//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// eigStandard.cpp
//
// Code generation for function 'eigStandard'
//

// Include files
#include "eigStandard.h"
#include "rt_nonfinite.h"
#include "sobi_coder_data.h"
#include "sobi_coder_rtwutil.h"
#include "xzggbal.h"
#include "xzhgeqz.h"
#include "xzlartg.h"
#include "xztgevc.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include <cmath>

// Function Definitions
namespace coder {
void eigStandard(const ::coder::array<double, 2U> &A,
                 ::coder::array<creal_T, 2U> &V, ::coder::array<creal_T, 2U> &D)
{
  array<creal_T, 2U> At;
  array<creal_T, 1U> alpha1;
  array<creal_T, 1U> beta1;
  array<int, 1U> rscale;
  array<signed char, 2U> b_I;
  creal_T tmp;
  double a;
  double absxk;
  double anrm;
  double anrmto;
  double cto1;
  double ctoc;
  double stemp_im;
  int ihi;
  int ilo;
  int j;
  int jcol;
  int jcolp1;
  int jrow;
  int n;
  At.set_size(A.size(0), A.size(1));
  jcol = A.size(0) * A.size(1);
  for (jrow = 0; jrow < jcol; jrow++) {
    At[jrow].re = A[jrow];
    At[jrow].im = 0.0;
  }
  n = At.size(0) - 1;
  alpha1.set_size(At.size(0));
  jcol = At.size(0);
  for (jrow = 0; jrow < jcol; jrow++) {
    alpha1[jrow].re = 0.0;
    alpha1[jrow].im = 0.0;
  }
  beta1.set_size(At.size(0));
  jcol = At.size(0);
  for (jrow = 0; jrow < jcol; jrow++) {
    beta1[jrow].re = 0.0;
    beta1[jrow].im = 0.0;
  }
  V.set_size(At.size(0), At.size(0));
  jcol = At.size(0) * At.size(0);
  for (jrow = 0; jrow < jcol; jrow++) {
    V[jrow].re = 0.0;
    V[jrow].im = 0.0;
  }
  if ((At.size(0) != 0) && (At.size(1) != 0)) {
    boolean_T exitg1;
    anrm = 0.0;
    jcol = 0;
    exitg1 = false;
    while ((!exitg1) && (jcol <= At.size(0) * At.size(1) - 1)) {
      absxk = rt_hypotd_snf(At[jcol].re, At[jcol].im);
      if (std::isnan(absxk)) {
        anrm = rtNaN;
        exitg1 = true;
      } else {
        if (absxk > anrm) {
          anrm = absxk;
        }
        jcol++;
      }
    }
    if (std::isinf(anrm) || std::isnan(anrm)) {
      alpha1.set_size(At.size(0));
      jcol = At.size(0);
      for (jrow = 0; jrow < jcol; jrow++) {
        alpha1[jrow].re = rtNaN;
        alpha1[jrow].im = 0.0;
      }
      beta1.set_size(At.size(0));
      jcol = At.size(0);
      for (jrow = 0; jrow < jcol; jrow++) {
        beta1[jrow].re = rtNaN;
        beta1[jrow].im = 0.0;
      }
      V.set_size(At.size(0), At.size(0));
      jcol = At.size(0) * At.size(0);
      for (jrow = 0; jrow < jcol; jrow++) {
        V[jrow].re = rtNaN;
        V[jrow].im = 0.0;
      }
    } else {
      int b_n;
      int i;
      boolean_T guard1{false};
      boolean_T ilascl;
      boolean_T notdone;
      ilascl = false;
      anrmto = anrm;
      guard1 = false;
      if ((anrm > 0.0) && (anrm < 6.7178761075670888E-139)) {
        anrmto = 6.7178761075670888E-139;
        ilascl = true;
        guard1 = true;
      } else if (anrm > 1.4885657073574029E+138) {
        anrmto = 1.4885657073574029E+138;
        ilascl = true;
        guard1 = true;
      }
      if (guard1) {
        absxk = anrm;
        ctoc = anrmto;
        notdone = true;
        while (notdone) {
          stemp_im = absxk * 2.0041683600089728E-292;
          cto1 = ctoc / 4.9896007738368E+291;
          if ((stemp_im > ctoc) && (ctoc != 0.0)) {
            a = 2.0041683600089728E-292;
            absxk = stemp_im;
          } else if (cto1 > absxk) {
            a = 4.9896007738368E+291;
            ctoc = cto1;
          } else {
            a = ctoc / absxk;
            notdone = false;
          }
          jcol = At.size(0) * At.size(1);
          for (jrow = 0; jrow < jcol; jrow++) {
            At[jrow].re = a * At[jrow].re;
            At[jrow].im = a * At[jrow].im;
          }
        }
      }
      internal::reflapack::xzggbal(At, &ilo, &ihi, rscale);
      b_n = At.size(0);
      b_I.set_size(At.size(0), At.size(0));
      jcol = At.size(0) * At.size(0);
      for (jrow = 0; jrow < jcol; jrow++) {
        b_I[jrow] = 0;
      }
      if (At.size(0) > 0) {
        for (jcol = 0; jcol < b_n; jcol++) {
          b_I[jcol + b_I.size(0) * jcol] = 1;
        }
      }
      V.set_size(b_I.size(0), b_I.size(1));
      jcol = b_I.size(0) * b_I.size(1);
      for (jrow = 0; jrow < jcol; jrow++) {
        V[jrow].re = b_I[jrow];
        V[jrow].im = 0.0;
      }
      if ((At.size(0) > 1) && (ihi >= ilo + 2)) {
        for (jcol = ilo - 1; jcol + 1 < ihi - 1; jcol++) {
          jcolp1 = jcol + 2;
          for (jrow = ihi - 1; jrow + 1 > jcol + 2; jrow--) {
            internal::reflapack::xzlartg(At[(jrow + At.size(0) * jcol) - 1],
                                         At[jrow + At.size(0) * jcol], &absxk,
                                         &tmp,
                                         &At[(jrow + At.size(0) * jcol) - 1]);
            At[jrow + At.size(0) * jcol].re = 0.0;
            At[jrow + At.size(0) * jcol].im = 0.0;
            for (j = jcolp1; j <= b_n; j++) {
              ctoc = absxk * At[(jrow + At.size(0) * (j - 1)) - 1].re +
                     (tmp.re * At[jrow + At.size(0) * (j - 1)].re -
                      tmp.im * At[jrow + At.size(0) * (j - 1)].im);
              stemp_im = absxk * At[(jrow + At.size(0) * (j - 1)) - 1].im +
                         (tmp.re * At[jrow + At.size(0) * (j - 1)].im +
                          tmp.im * At[jrow + At.size(0) * (j - 1)].re);
              cto1 = At[(jrow + At.size(0) * (j - 1)) - 1].re;
              At[jrow + At.size(0) * (j - 1)].re =
                  absxk * At[jrow + At.size(0) * (j - 1)].re -
                  (tmp.re * At[(jrow + At.size(0) * (j - 1)) - 1].re +
                   tmp.im * At[(jrow + At.size(0) * (j - 1)) - 1].im);
              At[jrow + At.size(0) * (j - 1)].im =
                  absxk * At[jrow + At.size(0) * (j - 1)].im -
                  (tmp.re * At[(jrow + At.size(0) * (j - 1)) - 1].im -
                   tmp.im * cto1);
              At[(jrow + At.size(0) * (j - 1)) - 1].re = ctoc;
              At[(jrow + At.size(0) * (j - 1)) - 1].im = stemp_im;
            }
            tmp.re = -tmp.re;
            tmp.im = -tmp.im;
            for (i = 1; i <= ihi; i++) {
              ctoc = absxk * At[(i + At.size(0) * jrow) - 1].re +
                     (tmp.re * At[(i + At.size(0) * (jrow - 1)) - 1].re -
                      tmp.im * At[(i + At.size(0) * (jrow - 1)) - 1].im);
              stemp_im = absxk * At[(i + At.size(0) * jrow) - 1].im +
                         (tmp.re * At[(i + At.size(0) * (jrow - 1)) - 1].im +
                          tmp.im * At[(i + At.size(0) * (jrow - 1)) - 1].re);
              cto1 = At[(i + At.size(0) * jrow) - 1].re;
              At[(i + At.size(0) * (jrow - 1)) - 1].re =
                  absxk * At[(i + At.size(0) * (jrow - 1)) - 1].re -
                  (tmp.re * At[(i + At.size(0) * jrow) - 1].re +
                   tmp.im * At[(i + At.size(0) * jrow) - 1].im);
              At[(i + At.size(0) * (jrow - 1)) - 1].im =
                  absxk * At[(i + At.size(0) * (jrow - 1)) - 1].im -
                  (tmp.re * At[(i + At.size(0) * jrow) - 1].im - tmp.im * cto1);
              At[(i + At.size(0) * jrow) - 1].re = ctoc;
              At[(i + At.size(0) * jrow) - 1].im = stemp_im;
            }
            for (i = 1; i <= b_n; i++) {
              ctoc = absxk * V[(i + V.size(0) * jrow) - 1].re +
                     (tmp.re * V[(i + V.size(0) * (jrow - 1)) - 1].re -
                      tmp.im * V[(i + V.size(0) * (jrow - 1)) - 1].im);
              stemp_im = absxk * V[(i + V.size(0) * jrow) - 1].im +
                         (tmp.re * V[(i + V.size(0) * (jrow - 1)) - 1].im +
                          tmp.im * V[(i + V.size(0) * (jrow - 1)) - 1].re);
              cto1 = V[(i + V.size(0) * jrow) - 1].re;
              V[(i + V.size(0) * (jrow - 1)) - 1].re =
                  absxk * V[(i + V.size(0) * (jrow - 1)) - 1].re -
                  (tmp.re * V[(i + V.size(0) * jrow) - 1].re +
                   tmp.im * V[(i + V.size(0) * jrow) - 1].im);
              V[(i + V.size(0) * (jrow - 1)) - 1].im =
                  absxk * V[(i + V.size(0) * (jrow - 1)) - 1].im -
                  (tmp.re * V[(i + V.size(0) * jrow) - 1].im - tmp.im * cto1);
              V[(i + V.size(0) * jrow) - 1].re = ctoc;
              V[(i + V.size(0) * jrow) - 1].im = stemp_im;
            }
          }
        }
      }
      internal::reflapack::xzhgeqz(At, ilo, ihi, V, &jcol, alpha1, beta1);
      if (jcol == 0) {
        internal::reflapack::xztgevc(At, V);
        b_n = V.size(0);
        jcolp1 = V.size(1) - 1;
        if (ilo > 1) {
          for (i = ilo - 2; i + 1 >= 1; i--) {
            jcol = rscale[i] - 1;
            if (rscale[i] != i + 1) {
              for (j = 0; j <= jcolp1; j++) {
                tmp = V[i + V.size(0) * j];
                V[i + V.size(0) * j] = V[jcol + V.size(0) * j];
                V[jcol + V.size(0) * j] = tmp;
              }
            }
          }
        }
        if (ihi < b_n) {
          jrow = ihi + 1;
          for (i = jrow; i <= b_n; i++) {
            jcol = rscale[i - 1];
            if (jcol != i) {
              for (j = 0; j <= jcolp1; j++) {
                tmp = V[(i + V.size(0) * j) - 1];
                V[(i + V.size(0) * j) - 1] = V[(jcol + V.size(0) * j) - 1];
                V[(jcol + V.size(0) * j) - 1] = tmp;
              }
            }
          }
        }
        for (jcol = 0; jcol <= n; jcol++) {
          absxk = std::abs(V[V.size(0) * jcol].re) +
                  std::abs(V[V.size(0) * jcol].im);
          if (n + 1 > 1) {
            for (jcolp1 = 0; jcolp1 < n; jcolp1++) {
              ctoc = std::abs(V[(jcolp1 + V.size(0) * jcol) + 1].re) +
                     std::abs(V[(jcolp1 + V.size(0) * jcol) + 1].im);
              if (ctoc > absxk) {
                absxk = ctoc;
              }
            }
          }
          if (absxk >= 6.7178761075670888E-139) {
            absxk = 1.0 / absxk;
            for (jcolp1 = 0; jcolp1 <= n; jcolp1++) {
              V[jcolp1 + V.size(0) * jcol].re =
                  absxk * V[jcolp1 + V.size(0) * jcol].re;
              V[jcolp1 + V.size(0) * jcol].im =
                  absxk * V[jcolp1 + V.size(0) * jcol].im;
            }
          }
        }
        if (ilascl) {
          notdone = true;
          while (notdone) {
            stemp_im = anrmto * 2.0041683600089728E-292;
            cto1 = anrm / 4.9896007738368E+291;
            if ((stemp_im > anrm) && (anrm != 0.0)) {
              a = 2.0041683600089728E-292;
              anrmto = stemp_im;
            } else if (cto1 > anrmto) {
              a = 4.9896007738368E+291;
              anrm = cto1;
            } else {
              a = anrm / anrmto;
              notdone = false;
            }
            jcol = alpha1.size(0);
            for (jrow = 0; jrow < jcol; jrow++) {
              alpha1[jrow].re = a * alpha1[jrow].re;
              alpha1[jrow].im = a * alpha1[jrow].im;
            }
          }
        }
      }
    }
  }
  n = A.size(0);
  if (A.size(0) > 0) {
    jcol = (A.size(0) - 1) * A.size(0) + 1;
    for (jcolp1 = 1; n < 0 ? jcolp1 >= jcol : jcolp1 <= jcol; jcolp1 += n) {
      stemp_im = cblas_dznrm2((MKL_INT)n, (double *)&V[jcolp1 - 1], (MKL_INT)1);
      jrow = (jcolp1 + n) - 1;
      for (j = jcolp1; j <= jrow; j++) {
        anrmto = V[j - 1].re;
        anrm = V[j - 1].im;
        if (anrm == 0.0) {
          ctoc = anrmto / stemp_im;
          absxk = 0.0;
        } else if (anrmto == 0.0) {
          ctoc = 0.0;
          absxk = anrm / stemp_im;
        } else {
          ctoc = anrmto / stemp_im;
          absxk = anrm / stemp_im;
        }
        V[j - 1].re = ctoc;
        V[j - 1].im = absxk;
      }
    }
  }
  D.set_size(alpha1.size(0), alpha1.size(0));
  jcol = alpha1.size(0) * alpha1.size(0);
  for (jrow = 0; jrow < jcol; jrow++) {
    D[jrow].re = 0.0;
    D[jrow].im = 0.0;
  }
  jrow = alpha1.size(0);
  for (jcol = 0; jcol < jrow; jcol++) {
    anrmto = alpha1[jcol].re;
    anrm = alpha1[jcol].im;
    stemp_im = beta1[jcol].re;
    cto1 = beta1[jcol].im;
    if (cto1 == 0.0) {
      if (anrm == 0.0) {
        D[jcol + D.size(0) * jcol].re = anrmto / stemp_im;
        D[jcol + D.size(0) * jcol].im = 0.0;
      } else if (anrmto == 0.0) {
        D[jcol + D.size(0) * jcol].re = 0.0;
        D[jcol + D.size(0) * jcol].im = anrm / stemp_im;
      } else {
        D[jcol + D.size(0) * jcol].re = anrmto / stemp_im;
        D[jcol + D.size(0) * jcol].im = anrm / stemp_im;
      }
    } else if (stemp_im == 0.0) {
      if (anrmto == 0.0) {
        D[jcol + D.size(0) * jcol].re = anrm / cto1;
        D[jcol + D.size(0) * jcol].im = 0.0;
      } else if (anrm == 0.0) {
        D[jcol + D.size(0) * jcol].re = 0.0;
        D[jcol + D.size(0) * jcol].im = -(anrmto / cto1);
      } else {
        D[jcol + D.size(0) * jcol].re = anrm / cto1;
        D[jcol + D.size(0) * jcol].im = -(anrmto / cto1);
      }
    } else {
      a = std::abs(stemp_im);
      absxk = std::abs(cto1);
      if (a > absxk) {
        ctoc = cto1 / stemp_im;
        absxk = stemp_im + ctoc * cto1;
        D[jcol + D.size(0) * jcol].re = (anrmto + ctoc * anrm) / absxk;
        D[jcol + D.size(0) * jcol].im = (anrm - ctoc * anrmto) / absxk;
      } else if (absxk == a) {
        if (stemp_im > 0.0) {
          ctoc = 0.5;
        } else {
          ctoc = -0.5;
        }
        if (cto1 > 0.0) {
          absxk = 0.5;
        } else {
          absxk = -0.5;
        }
        D[jcol + D.size(0) * jcol].re = (anrmto * ctoc + anrm * absxk) / a;
        D[jcol + D.size(0) * jcol].im = (anrm * ctoc - anrmto * absxk) / a;
      } else {
        ctoc = stemp_im / cto1;
        absxk = cto1 + ctoc * stemp_im;
        D[jcol + D.size(0) * jcol].re = (ctoc * anrmto + anrm) / absxk;
        D[jcol + D.size(0) * jcol].im = (ctoc * anrm - anrmto) / absxk;
      }
    }
  }
}

} // namespace coder

// End of code generation (eigStandard.cpp)
