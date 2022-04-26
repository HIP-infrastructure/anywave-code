//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// pca.cpp
//
// Code generation for function 'pca'
//

// Include files
#include "pca.h"
#include "eigStandard.h"
#include "rot90.h"
#include "rt_nonfinite.h"
#include "schur.h"
#include "sort.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include <cmath>

// Function Definitions
void pca(const coder::array<double, 2U> &data, double ncomps,
         coder::array<double, 2U> &data_pca,
         coder::array<double, 2U> &eigenvectors)
{
  coder::array<creal_T, 2U> D;
  coder::array<creal_T, 2U> V;
  coder::array<creal_T, 1U> PCeigenval;
  coder::array<double, 2U> PCdat2;
  coder::array<double, 2U> PCout;
  coder::array<double, 2U> b_D;
  coder::array<double, 2U> r;
  coder::array<double, 2U> r1;
  coder::array<double, 1U> b_iidx;
  coder::array<int, 1U> iidx;
  int i;
  int loop_ub;
  int n;
  int nx;
  //  Computes principal component analysis following runica.m code.
  //  Pipeline for ICA:
  //  1- Reduce number of dimensions in 'data' with dyn_pca.
  //  2- Compute ICA on 'data_pca'.
  //  3- The ICA matrices for all channels are:
  //        > unmixing_all_channels = unmixing_pca * eigenvectors(:,1:ncomps)';
  //        > mixing_all_channels   = pinv(unmixing_all_channels)
  //
  //  Syntax:
  //    [data_pca,eigenvectors,eigenvalues]=dyn_pca(data,ncomps);
  //
  //  Inputs:
  //    data
  //    ncomps
  //
  //  Outputs:
  //    data_pca
  //    eigenvectors
  //    eigenvalues
  //
  //  See also: runica.m
  //  Author: Victor Lopez Madrona <v.lopez.madrona@gmail.com>
  //  License: BSD (3-clause)
  //  May 2020; Last revision: 5-May-2020
  PCdat2.set_size(data.size(1), data.size(0));
  nx = data.size(0);
  for (i = 0; i < nx; i++) {
    loop_ub = data.size(1);
    for (n = 0; n < loop_ub; n++) {
      PCdat2[n + PCdat2.size(0) * i] = data[i + data.size(0) * n];
    }
  }
  //  transpose data
  //  now p chans,n time points
  loop_ub = PCdat2.size(0);
  nx = PCdat2.size(0) * PCdat2.size(1);
  for (i = 0; i < nx; i++) {
    PCdat2[i] = PCdat2[i] / static_cast<double>(loop_ub);
  }
  if ((data.size(0) == 0) || (data.size(1) == 0) || (PCdat2.size(0) == 0) ||
      (PCdat2.size(1) == 0)) {
    PCout.set_size(data.size(0), PCdat2.size(1));
    nx = data.size(0) * PCdat2.size(1);
    for (i = 0; i < nx; i++) {
      PCout[i] = 0.0;
    }
  } else {
    PCout.set_size(data.size(0), PCdat2.size(1));
    cblas_dgemm(
        CblasColMajor, CblasNoTrans, CblasNoTrans, (MKL_INT)data.size(0),
        (MKL_INT)PCdat2.size(1), (MKL_INT)data.size(1), 1.0,
        &(((coder::array<double, 2U> *)&data)->data())[0],
        (MKL_INT)data.size(0), &(PCdat2.data())[0], (MKL_INT)PCdat2.size(0),
        0.0, &(PCout.data())[0], (MKL_INT)data.size(0));
  }
  n = PCout.size(0);
  V.set_size(PCout.size(0), PCout.size(0));
  D.set_size(PCout.size(0), PCout.size(0));
  if ((PCout.size(0) != 0) && (PCout.size(1) != 0)) {
    boolean_T p;
    nx = PCout.size(0) * PCout.size(1);
    p = true;
    for (loop_ub = 0; loop_ub < nx; loop_ub++) {
      if ((!p) || (std::isinf(PCout[loop_ub]) || std::isnan(PCout[loop_ub]))) {
        p = false;
      }
    }
    if (!p) {
      V.set_size(PCout.size(0), PCout.size(0));
      nx = PCout.size(0) * PCout.size(0);
      for (i = 0; i < nx; i++) {
        V[i].re = rtNaN;
        V[i].im = 0.0;
      }
      D.set_size(PCout.size(0), PCout.size(0));
      nx = PCout.size(0) * PCout.size(0);
      for (i = 0; i < nx; i++) {
        D[i].re = 0.0;
        D[i].im = 0.0;
      }
      for (loop_ub = 0; loop_ub < n; loop_ub++) {
        D[loop_ub + D.size(0) * loop_ub].re = rtNaN;
        D[loop_ub + D.size(0) * loop_ub].im = 0.0;
      }
    } else {
      p = (PCout.size(0) == PCout.size(1));
      if (p) {
        boolean_T exitg2;
        nx = 0;
        exitg2 = false;
        while ((!exitg2) && (nx <= PCout.size(1) - 1)) {
          int exitg1;
          loop_ub = 0;
          do {
            exitg1 = 0;
            if (loop_ub <= nx) {
              if (!(PCout[loop_ub + PCout.size(0) * nx] ==
                    PCout[nx + PCout.size(0) * loop_ub])) {
                p = false;
                exitg1 = 1;
              } else {
                loop_ub++;
              }
            } else {
              nx++;
              exitg1 = 2;
            }
          } while (exitg1 == 0);
          if (exitg1 == 1) {
            exitg2 = true;
          }
        }
      }
      if (p) {
        coder::schur(PCout, PCdat2, b_D);
        n = b_D.size(0);
        for (nx = 2; nx <= n; nx++) {
          b_D[(nx + b_D.size(0) * (nx - 2)) - 1] = 0.0;
          for (loop_ub = 0; loop_ub <= nx - 2; loop_ub++) {
            b_D[loop_ub + b_D.size(0) * (nx - 1)] = 0.0;
          }
        }
        V.set_size(PCdat2.size(0), PCdat2.size(1));
        nx = PCdat2.size(0) * PCdat2.size(1);
        for (i = 0; i < nx; i++) {
          V[i].re = PCdat2[i];
          V[i].im = 0.0;
        }
        D.set_size(b_D.size(0), b_D.size(1));
        nx = b_D.size(0) * b_D.size(1);
        for (i = 0; i < nx; i++) {
          D[i].re = b_D[i];
          D[i].im = 0.0;
        }
      } else {
        coder::eigStandard(PCout, V, D);
      }
    }
  }
  //  get eigenvectors/eigenvalues
  if ((D.size(0) == 1) && (D.size(1) == 1)) {
    PCeigenval.set_size(1);
    PCeigenval[0] = D[0];
  } else {
    loop_ub = D.size(0);
    nx = D.size(1);
    if (loop_ub < nx) {
      nx = loop_ub;
    }
    if (0 < D.size(1)) {
      loop_ub = nx;
    } else {
      loop_ub = 0;
    }
    PCeigenval.set_size(loop_ub);
    i = loop_ub - 1;
    for (loop_ub = 0; loop_ub <= i; loop_ub++) {
      PCeigenval[loop_ub] = D[loop_ub + D.size(0) * loop_ub];
    }
  }
  coder::internal::sort(PCeigenval, iidx);
  b_iidx.set_size(iidx.size(0));
  nx = iidx.size(0);
  for (i = 0; i < nx; i++) {
    b_iidx[i] = iidx[i];
  }
  coder::rot90(b_iidx, r);
  coder::rot90(r, b_iidx);
  nx = V.size(0);
  eigenvectors.set_size(V.size(0), b_iidx.size(0));
  loop_ub = b_iidx.size(0);
  for (i = 0; i < loop_ub; i++) {
    for (n = 0; n < nx; n++) {
      eigenvectors[n + eigenvectors.size(0) * i] =
          V[n + V.size(0) * (static_cast<int>(b_iidx[i]) - 1)].re;
    }
  }
  // PCCompressed = PCEigenVectors(:,1:ncomps)'*data;
  if (1.0 > ncomps) {
    nx = 0;
  } else {
    nx = static_cast<int>(ncomps);
  }
  loop_ub = eigenvectors.size(0);
  r1.set_size(eigenvectors.size(0), nx);
  for (i = 0; i < nx; i++) {
    for (n = 0; n < loop_ub; n++) {
      r1[n + r1.size(0) * i] = eigenvectors[n + eigenvectors.size(0) * i];
    }
  }
  if ((eigenvectors.size(0) == 0) || (nx == 0) || (data.size(0) == 0) ||
      (data.size(1) == 0)) {
    data_pca.set_size(nx, data.size(1));
    nx *= data.size(1);
    for (i = 0; i < nx; i++) {
      data_pca[i] = 0.0;
    }
  } else {
    data_pca.set_size(nx, data.size(1));
    cblas_dgemm(CblasColMajor, CblasTrans, CblasNoTrans, (MKL_INT)nx,
                (MKL_INT)data.size(1), (MKL_INT)eigenvectors.size(0), 1.0,
                &(r1.data())[0], (MKL_INT)eigenvectors.size(0),
                &(((coder::array<double, 2U> *)&data)->data())[0],
                (MKL_INT)data.size(0), 0.0, &(data_pca.data())[0], (MKL_INT)nx);
  }
}

// End of code generation (pca.cpp)
