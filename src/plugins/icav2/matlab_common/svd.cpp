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
#include "svd.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
namespace coder
{
  namespace internal
  {
    void b_svd(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U>
               &U, ::coder::array<double, 1U> &s, ::coder::array<double, 2U> &V)
    {
      array<double, 2U> Vf;
      array<double, 2U> b_A;
      array<double, 1U> b_s;
      array<double, 1U> e;
      array<double, 1U> work;
      double nrm;
      int k;
      int m;
      int minnp;
      int n;
      int ns;
      int p;
      int qjj;
      b_A.set_size(A.size(0), A.size(1));
      ns = A.size(0) * A.size(1);
      for (m = 0; m < ns; m++) {
        b_A[m] = A[m];
      }

      n = A.size(0);
      p = A.size(1);
      qjj = A.size(0) + 1;
      ns = A.size(1);
      if (qjj < ns) {
        ns = qjj;
      }

      qjj = A.size(0);
      minnp = A.size(1);
      if (qjj < minnp) {
        minnp = qjj;
      }

      b_s.set_size(ns);
      for (m = 0; m < ns; m++) {
        b_s[m] = 0.0;
      }

      e.set_size(A.size(1));
      ns = A.size(1);
      for (m = 0; m < ns; m++) {
        e[m] = 0.0;
      }

      work.set_size(A.size(0));
      ns = A.size(0);
      for (m = 0; m < ns; m++) {
        work[m] = 0.0;
      }

      U.set_size(A.size(0), minnp);
      ns = A.size(0) * minnp;
      for (m = 0; m < ns; m++) {
        U[m] = 0.0;
      }

      Vf.set_size(A.size(1), A.size(1));
      ns = A.size(1) * A.size(1);
      for (m = 0; m < ns; m++) {
        Vf[m] = 0.0;
      }

      if ((A.size(0) == 0) || (A.size(1) == 0)) {
        int ii;
        qjj = A.size(0);
        if (qjj >= minnp) {
          qjj = minnp;
        }

        for (ii = 0; ii < qjj; ii++) {
          U[ii + U.size(0) * ii] = 1.0;
        }

        m = A.size(1);
        for (ii = 0; ii < m; ii++) {
          Vf[ii + Vf.size(0) * ii] = 1.0;
        }
      } else {
        double f;
        double rt;
        double snorm;
        int ii;
        int nct;
        int nctp1;
        int nmq;
        int nrt;
        int q;
        int qp1;
        int qq;
        if (A.size(1) > 2) {
          qjj = A.size(1) - 2;
        } else {
          qjj = 0;
        }

        nrt = A.size(0);
        if (qjj < nrt) {
          nrt = qjj;
        }

        if (A.size(0) > 1) {
          qjj = A.size(0) - 1;
        } else {
          qjj = 0;
        }

        nct = A.size(1);
        if (qjj < nct) {
          nct = qjj;
        }

        nctp1 = nct + 1;
        if (nct > nrt) {
          m = nct;
        } else {
          m = nrt;
        }

        for (q = 0; q < m; q++) {
          boolean_T apply_transform;
          qp1 = q + 2;
          qq = q + n * q;
          nmq = n - q;
          apply_transform = false;
          if (q + 1 <= nct) {
            if (nmq < 1) {
              nrm = 0.0;
            } else {
              nrm = cblas_dnrm2((MKL_INT)nmq, &b_A[qq], (MKL_INT)1);
            }

            if (nrm > 0.0) {
              apply_transform = true;
              if (b_A[qq] < 0.0) {
                rt = -nrm;
                b_s[q] = -nrm;
              } else {
                rt = nrm;
                b_s[q] = nrm;
              }

              if (std::abs(rt) >= 1.0020841800044864E-292) {
                if (nmq >= 1) {
                  cblas_dscal((MKL_INT)nmq, 1.0 / rt, &b_A[qq], (MKL_INT)1);
                }
              } else {
                ns = qq + nmq;
                for (k = qq + 1; k <= ns; k++) {
                  b_A[k - 1] = b_A[k - 1] / b_s[q];
                }
              }

              b_A[qq] = b_A[qq] + 1.0;
              b_s[q] = -b_s[q];
            } else {
              b_s[q] = 0.0;
            }
          }

          for (k = qp1; k <= p; k++) {
            qjj = q + n * (k - 1);
            if (apply_transform && (nmq >= 1)) {
              nrm = cblas_ddot((MKL_INT)nmq, &b_A[qq], (MKL_INT)1, &b_A[qjj],
                               (MKL_INT)1);
              cblas_daxpy((MKL_INT)nmq, -(nrm / b_A[q + b_A.size(0) * q]),
                          &b_A[qq], (MKL_INT)1, &b_A[qjj], (MKL_INT)1);
            }

            e[k - 1] = b_A[qjj];
          }

          if (q + 1 <= nct) {
            for (ii = q + 1; ii <= n; ii++) {
              U[(ii + U.size(0) * q) - 1] = b_A[(ii + b_A.size(0) * q) - 1];
            }
          }

          if (q + 1 <= nrt) {
            qq = (p - q) - 1;
            if (qq < 1) {
              nrm = 0.0;
            } else {
              nrm = cblas_dnrm2((MKL_INT)qq, &e[q + 1], (MKL_INT)1);
            }

            if (nrm == 0.0) {
              e[q] = 0.0;
            } else {
              if (e[q + 1] < 0.0) {
                e[q] = -nrm;
              } else {
                e[q] = nrm;
              }

              f = e[q];
              if (std::abs(e[q]) >= 1.0020841800044864E-292) {
                if (qq >= 1) {
                  cblas_dscal((MKL_INT)qq, 1.0 / e[q], &e[q + 1], (MKL_INT)1);
                }
              } else {
                ns = q + qq;
                for (k = qp1; k <= ns + 1; k++) {
                  e[k - 1] = e[k - 1] / f;
                }
              }

              e[q + 1] = e[q + 1] + 1.0;
              e[q] = -e[q];
              if (q + 2 <= n) {
                for (ii = qp1; ii <= n; ii++) {
                  work[ii - 1] = 0.0;
                }

                for (k = qp1; k <= p; k++) {
                  if (nmq - 1 >= 1) {
                    cblas_daxpy((MKL_INT)(nmq - 1), e[k - 1], &b_A[(q + n * (k -
                      1)) + 1], (MKL_INT)1, &work[q + 1], (MKL_INT)1);
                  }
                }

                for (k = qp1; k <= p; k++) {
                  if (nmq - 1 >= 1) {
                    cblas_daxpy((MKL_INT)(nmq - 1), -e[k - 1] / e[q + 1],
                                &work[q + 1], (MKL_INT)1, &b_A[(q + n * (k - 1))
                                + 1], (MKL_INT)1);
                  }
                }
              }
            }

            for (ii = qp1; ii <= p; ii++) {
              Vf[(ii + Vf.size(0) * q) - 1] = e[ii - 1];
            }
          }
        }

        if (A.size(1) < A.size(0) + 1) {
          m = A.size(1) - 1;
        } else {
          m = A.size(0);
        }

        if (nct < A.size(1)) {
          b_s[nct] = b_A[nct + b_A.size(0) * nct];
        }

        if (A.size(0) < m + 1) {
          b_s[m] = 0.0;
        }

        if (nrt + 1 < m + 1) {
          e[nrt] = b_A[nrt + b_A.size(0) * m];
        }

        e[m] = 0.0;
        if (nct + 1 <= minnp) {
          for (k = nctp1; k <= minnp; k++) {
            for (ii = 0; ii < n; ii++) {
              U[ii + U.size(0) * (k - 1)] = 0.0;
            }

            U[(k + U.size(0) * (k - 1)) - 1] = 1.0;
          }
        }

        for (q = nct; q >= 1; q--) {
          qp1 = q + 1;
          ns = (n - q) + 1;
          qq = (q + n * (q - 1)) - 1;
          if (b_s[q - 1] != 0.0) {
            for (k = qp1; k <= minnp; k++) {
              qjj = (q + n * (k - 1)) - 1;
              if (ns >= 1) {
                nrm = cblas_ddot((MKL_INT)ns, &U[qq], (MKL_INT)1, &U[qjj],
                                 (MKL_INT)1);
                cblas_daxpy((MKL_INT)ns, -(nrm / U[qq]), &U[qq], (MKL_INT)1,
                            &U[qjj], (MKL_INT)1);
              }
            }

            for (ii = q; ii <= n; ii++) {
              U[(ii + U.size(0) * (q - 1)) - 1] = -U[(ii + U.size(0) * (q - 1))
                - 1];
            }

            U[qq] = U[qq] + 1.0;
            for (ii = 0; ii <= q - 2; ii++) {
              U[ii + U.size(0) * (q - 1)] = 0.0;
            }
          } else {
            for (ii = 0; ii < n; ii++) {
              U[ii + U.size(0) * (q - 1)] = 0.0;
            }

            U[qq] = 1.0;
          }
        }

        for (q = p; q >= 1; q--) {
          if ((q <= nrt) && (e[q - 1] != 0.0)) {
            qp1 = q + 1;
            qq = p - q;
            ns = q + p * (q - 1);
            for (k = qp1; k <= p; k++) {
              qjj = q + p * (k - 1);
              if (qq >= 1) {
                nrm = cblas_ddot((MKL_INT)qq, &Vf[ns], (MKL_INT)1, &Vf[qjj],
                                 (MKL_INT)1);
                cblas_daxpy((MKL_INT)qq, -(nrm / Vf[ns]), &Vf[ns], (MKL_INT)1,
                            &Vf[qjj], (MKL_INT)1);
              }
            }
          }

          for (ii = 0; ii < p; ii++) {
            Vf[ii + Vf.size(0) * (q - 1)] = 0.0;
          }

          Vf[(q + Vf.size(0) * (q - 1)) - 1] = 1.0;
        }

        for (q = 0; q <= m; q++) {
          if (b_s[q] != 0.0) {
            rt = std::abs(b_s[q]);
            nrm = b_s[q] / rt;
            b_s[q] = rt;
            if (q + 1 < m + 1) {
              e[q] = e[q] / nrm;
            }

            if ((q + 1 <= n) && (n >= 1)) {
              cblas_dscal((MKL_INT)n, nrm, &U[n * q], (MKL_INT)1);
            }
          }

          if ((q + 1 < m + 1) && (e[q] != 0.0)) {
            rt = std::abs(e[q]);
            nrm = rt / e[q];
            e[q] = rt;
            b_s[q + 1] = b_s[q + 1] * nrm;
            if (p >= 1) {
              cblas_dscal((MKL_INT)p, nrm, &Vf[p * (q + 1)], (MKL_INT)1);
            }
          }
        }

        nmq = m;
        qq = 0;
        snorm = 0.0;
        for (ii = 0; ii <= m; ii++) {
          nrm = std::abs(b_s[ii]);
          rt = std::abs(e[ii]);
          if ((nrm > rt) || rtIsNaN(rt)) {
            rt = nrm;
          }

          if ((!(snorm > rt)) && (!rtIsNaN(rt))) {
            snorm = rt;
          }
        }

        while ((m + 1 > 0) && (qq < 75)) {
          boolean_T exitg1;
          ii = m;
          exitg1 = false;
          while (!(exitg1 || (ii == 0))) {
            nrm = std::abs(e[ii - 1]);
            if ((nrm <= 2.2204460492503131E-16 * (std::abs(b_s[ii - 1]) + std::
                  abs(b_s[ii]))) || (nrm <= 1.0020841800044864E-292) || ((qq >
                  20) && (nrm <= 2.2204460492503131E-16 * snorm))) {
              e[ii - 1] = 0.0;
              exitg1 = true;
            } else {
              ii--;
            }
          }

          if (ii == m) {
            ns = 4;
          } else {
            qjj = m + 1;
            ns = m + 1;
            exitg1 = false;
            while ((!exitg1) && (ns >= ii)) {
              qjj = ns;
              if (ns == ii) {
                exitg1 = true;
              } else {
                nrm = 0.0;
                if (ns < m + 1) {
                  nrm = std::abs(e[ns - 1]);
                }

                if (ns > ii + 1) {
                  nrm += std::abs(e[ns - 2]);
                }

                rt = std::abs(b_s[ns - 1]);
                if ((rt <= 2.2204460492503131E-16 * nrm) || (rt <=
                     1.0020841800044864E-292)) {
                  b_s[ns - 1] = 0.0;
                  exitg1 = true;
                } else {
                  ns--;
                }
              }
            }

            if (qjj == ii) {
              ns = 3;
            } else if (qjj == m + 1) {
              ns = 1;
            } else {
              ns = 2;
              ii = qjj;
            }
          }

          switch (ns) {
           case 1:
            {
              f = e[m - 1];
              e[m - 1] = 0.0;
              for (k = m; k >= ii + 1; k--) {
                double sm;
                double sqds;
                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&b_s[k - 1], &f, &sqds, &sm);
                if (k > ii + 1) {
                  nrm = e[k - 2];
                  f = -sm * nrm;
                  e[k - 2] = nrm * sqds;
                }

                if (p >= 1) {
                  cblas_drot((MKL_INT)p, &Vf[p * (k - 1)], (MKL_INT)1, &Vf[p * m],
                             (MKL_INT)1, sqds, sm);
                }
              }
            }
            break;

           case 2:
            {
              f = e[ii - 1];
              e[ii - 1] = 0.0;
              for (k = ii + 1; k <= m + 1; k++) {
                double sm;
                double sqds;
                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&b_s[k - 1], &f, &sqds, &sm);
                nrm = e[k - 1];
                f = -sm * nrm;
                e[k - 1] = nrm * sqds;
                if (n >= 1) {
                  cblas_drot((MKL_INT)n, &U[n * (k - 1)], (MKL_INT)1, &U[n * (ii
                              - 1)], (MKL_INT)1, sqds, sm);
                }
              }
            }
            break;

           case 3:
            {
              double b;
              double scale;
              double sm;
              double sqds;
              scale = std::abs(b_s[m]);
              nrm = b_s[m - 1];
              rt = std::abs(nrm);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              f = e[m - 1];
              rt = std::abs(f);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              rt = std::abs(b_s[ii]);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              rt = std::abs(e[ii]);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              sm = b_s[m] / scale;
              nrm /= scale;
              rt = f / scale;
              sqds = b_s[ii] / scale;
              b = ((nrm + sm) * (nrm - sm) + rt * rt) / 2.0;
              nrm = sm * rt;
              nrm *= nrm;
              if ((b != 0.0) || (nrm != 0.0)) {
                rt = std::sqrt(b * b + nrm);
                if (b < 0.0) {
                  rt = -rt;
                }

                rt = nrm / (b + rt);
              } else {
                rt = 0.0;
              }

              f = (sqds + sm) * (sqds - sm) + rt;
              nrm = sqds * (e[ii] / scale);
              for (k = ii + 1; k <= m; k++) {
                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&f, &nrm, &sqds, &sm);
                if (k > ii + 1) {
                  e[k - 2] = f;
                }

                nrm = e[k - 1];
                rt = b_s[k - 1];
                f = sqds * rt + sm * nrm;
                e[k - 1] = sqds * nrm - sm * rt;
                b = sm * b_s[k];
                rt = b_s[k] * sqds;
                if (p >= 1) {
                  cblas_drot((MKL_INT)p, &Vf[p * (k - 1)], (MKL_INT)1, &Vf[p * k],
                             (MKL_INT)1, sqds, sm);
                }

                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&f, &b, &sqds, &sm);
                b_s[k - 1] = f;
                f = sqds * e[k - 1] + sm * rt;
                rt = -sm * e[k - 1] + sqds * rt;
                b_s[k] = rt;
                nrm = sm * e[k];
                e[k] = e[k] * sqds;
                if ((k < n) && (n >= 1)) {
                  cblas_drot((MKL_INT)n, &U[n * (k - 1)], (MKL_INT)1, &U[n * k],
                             (MKL_INT)1, sqds, sm);
                }
              }

              e[m - 1] = f;
              qq++;
            }
            break;

           default:
            if (b_s[ii] < 0.0) {
              b_s[ii] = -b_s[ii];
              if (p >= 1) {
                cblas_dscal((MKL_INT)p, -1.0, &Vf[p * ii], (MKL_INT)1);
              }
            }

            qp1 = ii + 1;
            while ((ii + 1 < nmq + 1) && (b_s[ii] < b_s[qp1])) {
              rt = b_s[ii];
              b_s[ii] = b_s[qp1];
              b_s[qp1] = rt;
              if (ii + 1 < p) {
                ns = p * ii;
                qjj = p * (ii + 1);
                for (k = 0; k < p; k++) {
                  nrm = Vf[ns];
                  Vf[ns] = Vf[qjj];
                  Vf[qjj] = nrm;
                  ns++;
                  qjj++;
                }
              }

              if (ii + 1 < n) {
                ns = n * ii;
                qjj = n * (ii + 1);
                for (k = 0; k < n; k++) {
                  nrm = U[ns];
                  U[ns] = U[qjj];
                  U[qjj] = nrm;
                  ns++;
                  qjj++;
                }
              }

              ii = qp1;
              qp1++;
            }

            qq = 0;
            m--;
            break;
          }
        }
      }

      s.set_size(minnp);
      V.set_size(A.size(1), minnp);
      for (k = 0; k < minnp; k++) {
        s[k] = b_s[k];
        for (ns = 0; ns < p; ns++) {
          V[ns + V.size(0) * k] = Vf[ns + Vf.size(0) * k];
        }
      }
    }

    void svd(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U> &U,
             ::coder::array<double, 1U> &s, ::coder::array<double, 2U> &V)
    {
      array<double, 2U> b_A;
      array<double, 1U> b_s;
      array<double, 1U> e;
      array<double, 1U> work;
      double nrm;
      int k;
      int m;
      int minnp;
      int n;
      int ns;
      int p;
      int qjj;
      b_A.set_size(A.size(0), A.size(1));
      ns = A.size(0) * A.size(1);
      for (m = 0; m < ns; m++) {
        b_A[m] = A[m];
      }

      n = A.size(0);
      p = A.size(1);
      qjj = A.size(0) + 1;
      ns = A.size(1);
      if (qjj < ns) {
        ns = qjj;
      }

      qjj = A.size(0);
      minnp = A.size(1);
      if (qjj < minnp) {
        minnp = qjj;
      }

      b_s.set_size(ns);
      for (m = 0; m < ns; m++) {
        b_s[m] = 0.0;
      }

      e.set_size(A.size(1));
      ns = A.size(1);
      for (m = 0; m < ns; m++) {
        e[m] = 0.0;
      }

      work.set_size(A.size(0));
      ns = A.size(0);
      for (m = 0; m < ns; m++) {
        work[m] = 0.0;
      }

      U.set_size(A.size(0), minnp);
      ns = A.size(0) * minnp;
      for (m = 0; m < ns; m++) {
        U[m] = 0.0;
      }

      V.set_size(A.size(1), A.size(1));
      ns = A.size(1) * A.size(1);
      for (m = 0; m < ns; m++) {
        V[m] = 0.0;
      }

      if ((A.size(0) == 0) || (A.size(1) == 0)) {
        int ii;
        qjj = A.size(0);
        if (qjj >= minnp) {
          qjj = minnp;
        }

        for (ii = 0; ii < qjj; ii++) {
          U[ii + U.size(0) * ii] = 1.0;
        }

        m = A.size(1);
        for (ii = 0; ii < m; ii++) {
          V[ii + V.size(0) * ii] = 1.0;
        }
      } else {
        double f;
        double rt;
        double snorm;
        int ii;
        int nct;
        int nctp1;
        int nmq;
        int nrt;
        int q;
        int qp1;
        int qq;
        if (A.size(1) > 2) {
          qjj = A.size(1) - 2;
        } else {
          qjj = 0;
        }

        nrt = A.size(0);
        if (qjj < nrt) {
          nrt = qjj;
        }

        if (A.size(0) > 1) {
          qjj = A.size(0) - 1;
        } else {
          qjj = 0;
        }

        nct = A.size(1);
        if (qjj < nct) {
          nct = qjj;
        }

        nctp1 = nct + 1;
        if (nct > nrt) {
          m = nct;
        } else {
          m = nrt;
        }

        for (q = 0; q < m; q++) {
          boolean_T apply_transform;
          qp1 = q + 2;
          qq = q + n * q;
          nmq = n - q;
          apply_transform = false;
          if (q + 1 <= nct) {
            if (nmq < 1) {
              nrm = 0.0;
            } else {
              nrm = cblas_dnrm2((MKL_INT)nmq, &b_A[qq], (MKL_INT)1);
            }

            if (nrm > 0.0) {
              apply_transform = true;
              if (b_A[qq] < 0.0) {
                rt = -nrm;
                b_s[q] = -nrm;
              } else {
                rt = nrm;
                b_s[q] = nrm;
              }

              if (std::abs(rt) >= 1.0020841800044864E-292) {
                if (nmq >= 1) {
                  cblas_dscal((MKL_INT)nmq, 1.0 / rt, &b_A[qq], (MKL_INT)1);
                }
              } else {
                ns = qq + nmq;
                for (k = qq + 1; k <= ns; k++) {
                  b_A[k - 1] = b_A[k - 1] / b_s[q];
                }
              }

              b_A[qq] = b_A[qq] + 1.0;
              b_s[q] = -b_s[q];
            } else {
              b_s[q] = 0.0;
            }
          }

          for (k = qp1; k <= p; k++) {
            qjj = q + n * (k - 1);
            if (apply_transform && (nmq >= 1)) {
              nrm = cblas_ddot((MKL_INT)nmq, &b_A[qq], (MKL_INT)1, &b_A[qjj],
                               (MKL_INT)1);
              cblas_daxpy((MKL_INT)nmq, -(nrm / b_A[q + b_A.size(0) * q]),
                          &b_A[qq], (MKL_INT)1, &b_A[qjj], (MKL_INT)1);
            }

            e[k - 1] = b_A[qjj];
          }

          if (q + 1 <= nct) {
            for (ii = q + 1; ii <= n; ii++) {
              U[(ii + U.size(0) * q) - 1] = b_A[(ii + b_A.size(0) * q) - 1];
            }
          }

          if (q + 1 <= nrt) {
            qq = (p - q) - 1;
            if (qq < 1) {
              nrm = 0.0;
            } else {
              nrm = cblas_dnrm2((MKL_INT)qq, &e[q + 1], (MKL_INT)1);
            }

            if (nrm == 0.0) {
              e[q] = 0.0;
            } else {
              if (e[q + 1] < 0.0) {
                e[q] = -nrm;
              } else {
                e[q] = nrm;
              }

              f = e[q];
              if (std::abs(e[q]) >= 1.0020841800044864E-292) {
                if (qq >= 1) {
                  cblas_dscal((MKL_INT)qq, 1.0 / e[q], &e[q + 1], (MKL_INT)1);
                }
              } else {
                ns = q + qq;
                for (k = qp1; k <= ns + 1; k++) {
                  e[k - 1] = e[k - 1] / f;
                }
              }

              e[q + 1] = e[q + 1] + 1.0;
              e[q] = -e[q];
              if (q + 2 <= n) {
                for (ii = qp1; ii <= n; ii++) {
                  work[ii - 1] = 0.0;
                }

                for (k = qp1; k <= p; k++) {
                  if (nmq - 1 >= 1) {
                    cblas_daxpy((MKL_INT)(nmq - 1), e[k - 1], &b_A[(q + n * (k -
                      1)) + 1], (MKL_INT)1, &work[q + 1], (MKL_INT)1);
                  }
                }

                for (k = qp1; k <= p; k++) {
                  if (nmq - 1 >= 1) {
                    cblas_daxpy((MKL_INT)(nmq - 1), -e[k - 1] / e[q + 1],
                                &work[q + 1], (MKL_INT)1, &b_A[(q + n * (k - 1))
                                + 1], (MKL_INT)1);
                  }
                }
              }
            }

            for (ii = qp1; ii <= p; ii++) {
              V[(ii + V.size(0) * q) - 1] = e[ii - 1];
            }
          }
        }

        if (A.size(1) < A.size(0) + 1) {
          m = A.size(1) - 1;
        } else {
          m = A.size(0);
        }

        if (nct < A.size(1)) {
          b_s[nct] = b_A[nct + b_A.size(0) * nct];
        }

        if (A.size(0) < m + 1) {
          b_s[m] = 0.0;
        }

        if (nrt + 1 < m + 1) {
          e[nrt] = b_A[nrt + b_A.size(0) * m];
        }

        e[m] = 0.0;
        if (nct + 1 <= minnp) {
          for (k = nctp1; k <= minnp; k++) {
            for (ii = 0; ii < n; ii++) {
              U[ii + U.size(0) * (k - 1)] = 0.0;
            }

            U[(k + U.size(0) * (k - 1)) - 1] = 1.0;
          }
        }

        for (q = nct; q >= 1; q--) {
          qp1 = q + 1;
          ns = (n - q) + 1;
          qq = (q + n * (q - 1)) - 1;
          if (b_s[q - 1] != 0.0) {
            for (k = qp1; k <= minnp; k++) {
              qjj = (q + n * (k - 1)) - 1;
              if (ns >= 1) {
                nrm = cblas_ddot((MKL_INT)ns, &U[qq], (MKL_INT)1, &U[qjj],
                                 (MKL_INT)1);
                cblas_daxpy((MKL_INT)ns, -(nrm / U[qq]), &U[qq], (MKL_INT)1,
                            &U[qjj], (MKL_INT)1);
              }
            }

            for (ii = q; ii <= n; ii++) {
              U[(ii + U.size(0) * (q - 1)) - 1] = -U[(ii + U.size(0) * (q - 1))
                - 1];
            }

            U[qq] = U[qq] + 1.0;
            for (ii = 0; ii <= q - 2; ii++) {
              U[ii + U.size(0) * (q - 1)] = 0.0;
            }
          } else {
            for (ii = 0; ii < n; ii++) {
              U[ii + U.size(0) * (q - 1)] = 0.0;
            }

            U[qq] = 1.0;
          }
        }

        for (q = p; q >= 1; q--) {
          if ((q <= nrt) && (e[q - 1] != 0.0)) {
            qp1 = q + 1;
            qq = p - q;
            ns = q + p * (q - 1);
            for (k = qp1; k <= p; k++) {
              qjj = q + p * (k - 1);
              if (qq >= 1) {
                nrm = cblas_ddot((MKL_INT)qq, &V[ns], (MKL_INT)1, &V[qjj],
                                 (MKL_INT)1);
                cblas_daxpy((MKL_INT)qq, -(nrm / V[ns]), &V[ns], (MKL_INT)1,
                            &V[qjj], (MKL_INT)1);
              }
            }
          }

          for (ii = 0; ii < p; ii++) {
            V[ii + V.size(0) * (q - 1)] = 0.0;
          }

          V[(q + V.size(0) * (q - 1)) - 1] = 1.0;
        }

        for (q = 0; q <= m; q++) {
          if (b_s[q] != 0.0) {
            rt = std::abs(b_s[q]);
            nrm = b_s[q] / rt;
            b_s[q] = rt;
            if (q + 1 < m + 1) {
              e[q] = e[q] / nrm;
            }

            if ((q + 1 <= n) && (n >= 1)) {
              cblas_dscal((MKL_INT)n, nrm, &U[n * q], (MKL_INT)1);
            }
          }

          if ((q + 1 < m + 1) && (e[q] != 0.0)) {
            rt = std::abs(e[q]);
            nrm = rt / e[q];
            e[q] = rt;
            b_s[q + 1] = b_s[q + 1] * nrm;
            if (p >= 1) {
              cblas_dscal((MKL_INT)p, nrm, &V[p * (q + 1)], (MKL_INT)1);
            }
          }
        }

        nmq = m;
        qq = 0;
        snorm = 0.0;
        for (ii = 0; ii <= m; ii++) {
          nrm = std::abs(b_s[ii]);
          rt = std::abs(e[ii]);
          if ((nrm > rt) || rtIsNaN(rt)) {
            rt = nrm;
          }

          if ((!(snorm > rt)) && (!rtIsNaN(rt))) {
            snorm = rt;
          }
        }

        while ((m + 1 > 0) && (qq < 75)) {
          boolean_T exitg1;
          ii = m;
          exitg1 = false;
          while (!(exitg1 || (ii == 0))) {
            nrm = std::abs(e[ii - 1]);
            if ((nrm <= 2.2204460492503131E-16 * (std::abs(b_s[ii - 1]) + std::
                  abs(b_s[ii]))) || (nrm <= 1.0020841800044864E-292) || ((qq >
                  20) && (nrm <= 2.2204460492503131E-16 * snorm))) {
              e[ii - 1] = 0.0;
              exitg1 = true;
            } else {
              ii--;
            }
          }

          if (ii == m) {
            ns = 4;
          } else {
            qjj = m + 1;
            ns = m + 1;
            exitg1 = false;
            while ((!exitg1) && (ns >= ii)) {
              qjj = ns;
              if (ns == ii) {
                exitg1 = true;
              } else {
                nrm = 0.0;
                if (ns < m + 1) {
                  nrm = std::abs(e[ns - 1]);
                }

                if (ns > ii + 1) {
                  nrm += std::abs(e[ns - 2]);
                }

                rt = std::abs(b_s[ns - 1]);
                if ((rt <= 2.2204460492503131E-16 * nrm) || (rt <=
                     1.0020841800044864E-292)) {
                  b_s[ns - 1] = 0.0;
                  exitg1 = true;
                } else {
                  ns--;
                }
              }
            }

            if (qjj == ii) {
              ns = 3;
            } else if (qjj == m + 1) {
              ns = 1;
            } else {
              ns = 2;
              ii = qjj;
            }
          }

          switch (ns) {
           case 1:
            {
              f = e[m - 1];
              e[m - 1] = 0.0;
              for (k = m; k >= ii + 1; k--) {
                double sm;
                double sqds;
                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&b_s[k - 1], &f, &sqds, &sm);
                if (k > ii + 1) {
                  nrm = e[k - 2];
                  f = -sm * nrm;
                  e[k - 2] = nrm * sqds;
                }

                if (p >= 1) {
                  cblas_drot((MKL_INT)p, &V[p * (k - 1)], (MKL_INT)1, &V[p * m],
                             (MKL_INT)1, sqds, sm);
                }
              }
            }
            break;

           case 2:
            {
              f = e[ii - 1];
              e[ii - 1] = 0.0;
              for (k = ii + 1; k <= m + 1; k++) {
                double sm;
                double sqds;
                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&b_s[k - 1], &f, &sqds, &sm);
                nrm = e[k - 1];
                f = -sm * nrm;
                e[k - 1] = nrm * sqds;
                if (n >= 1) {
                  cblas_drot((MKL_INT)n, &U[n * (k - 1)], (MKL_INT)1, &U[n * (ii
                              - 1)], (MKL_INT)1, sqds, sm);
                }
              }
            }
            break;

           case 3:
            {
              double b;
              double scale;
              double sm;
              double sqds;
              scale = std::abs(b_s[m]);
              nrm = b_s[m - 1];
              rt = std::abs(nrm);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              f = e[m - 1];
              rt = std::abs(f);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              rt = std::abs(b_s[ii]);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              rt = std::abs(e[ii]);
              if ((!(scale > rt)) && (!rtIsNaN(rt))) {
                scale = rt;
              }

              sm = b_s[m] / scale;
              nrm /= scale;
              rt = f / scale;
              sqds = b_s[ii] / scale;
              b = ((nrm + sm) * (nrm - sm) + rt * rt) / 2.0;
              nrm = sm * rt;
              nrm *= nrm;
              if ((b != 0.0) || (nrm != 0.0)) {
                rt = std::sqrt(b * b + nrm);
                if (b < 0.0) {
                  rt = -rt;
                }

                rt = nrm / (b + rt);
              } else {
                rt = 0.0;
              }

              f = (sqds + sm) * (sqds - sm) + rt;
              nrm = sqds * (e[ii] / scale);
              for (k = ii + 1; k <= m; k++) {
                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&f, &nrm, &sqds, &sm);
                if (k > ii + 1) {
                  e[k - 2] = f;
                }

                nrm = e[k - 1];
                rt = b_s[k - 1];
                f = sqds * rt + sm * nrm;
                e[k - 1] = sqds * nrm - sm * rt;
                b = sm * b_s[k];
                rt = b_s[k] * sqds;
                if (p >= 1) {
                  cblas_drot((MKL_INT)p, &V[p * (k - 1)], (MKL_INT)1, &V[p * k],
                             (MKL_INT)1, sqds, sm);
                }

                sqds = 0.0;
                sm = 0.0;
                cblas_drotg(&f, &b, &sqds, &sm);
                b_s[k - 1] = f;
                f = sqds * e[k - 1] + sm * rt;
                rt = -sm * e[k - 1] + sqds * rt;
                b_s[k] = rt;
                nrm = sm * e[k];
                e[k] = e[k] * sqds;
                if ((k < n) && (n >= 1)) {
                  cblas_drot((MKL_INT)n, &U[n * (k - 1)], (MKL_INT)1, &U[n * k],
                             (MKL_INT)1, sqds, sm);
                }
              }

              e[m - 1] = f;
              qq++;
            }
            break;

           default:
            if (b_s[ii] < 0.0) {
              b_s[ii] = -b_s[ii];
              if (p >= 1) {
                cblas_dscal((MKL_INT)p, -1.0, &V[p * ii], (MKL_INT)1);
              }
            }

            qp1 = ii + 1;
            while ((ii + 1 < nmq + 1) && (b_s[ii] < b_s[qp1])) {
              rt = b_s[ii];
              b_s[ii] = b_s[qp1];
              b_s[qp1] = rt;
              if (ii + 1 < p) {
                ns = p * ii;
                qjj = p * (ii + 1);
                for (k = 0; k < p; k++) {
                  nrm = V[ns];
                  V[ns] = V[qjj];
                  V[qjj] = nrm;
                  ns++;
                  qjj++;
                }
              }

              if (ii + 1 < n) {
                ns = n * ii;
                qjj = n * (ii + 1);
                for (k = 0; k < n; k++) {
                  nrm = U[ns];
                  U[ns] = U[qjj];
                  U[qjj] = nrm;
                  ns++;
                  qjj++;
                }
              }

              ii = qp1;
              qp1++;
            }

            qq = 0;
            m--;
            break;
          }
        }
      }

      s.set_size(minnp);
      for (k = 0; k < minnp; k++) {
        s[k] = b_s[k];
      }
    }
  }
}

// End of code generation (svd.cpp)
