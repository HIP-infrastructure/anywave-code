//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// sobi_coder.cpp
//
// Code generation for function 'sobi_coder'
//

// Include files
#include "sobi_coder.h"
#include "mtimes.h"
#include "pca.h"
#include "pinv.h"
#include "rt_nonfinite.h"
#include "sobi_coder_data.h"
#include "sobi_coder_initialize.h"
#include "svd.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include <cmath>

// Function Definitions
void sobi_coder(coder::array<double, 2U> &data, double ncomps,
                coder::array<double, 2U> &mixing,
                coder::array<double, 2U> &unmixing)
{
  coder::array<double, 2U> C;
  coder::array<double, 2U> Q;
  coder::array<double, 2U> S;
  coder::array<double, 2U> U1;
  coder::array<double, 2U> V;
  coder::array<double, 2U> V1;
  coder::array<double, 2U> a;
  coder::array<double, 2U> b_data;
  coder::array<double, 2U> eigenvectors;
  coder::array<double, 2U> pca_data;
  coder::array<double, 2U> r;
  coder::array<double, 2U> r1;
  coder::array<double, 1U> s;
  coder::array<double, 1U> temp;
  double bsum;
  int N;
  int firstBlockLength;
  int hi;
  int ib;
  int k;
  int lastBlockLength;
  int m;
  int nblocks;
  int npages;
  int xblockoffset;
  int xi;
  int xpageoffset;
  boolean_T p;
  boolean_T pca_applied;

  //  sobi() - Second Order Blind Identification (SOBI) by joint diagonalization
  //  of
  //           correlation  matrices. THIS CODE ASSUMES TEMPORALLY CORRELATED
  //           SIGNALS, and uses correlations across times in performing the
  //           signal separation. Thus, estimated time delayed covariance
  //           matrices must be nonsingular for at least some time delays.
  //  Usage:
  //          >> winv = sobi(data);
  //          >> [winv,act] = sobi(data,n,p);
  //  Inputs:
  //    data - data matrix of size [m,N] ELSE of size [m,N,t] where
  //                 m is the number of sensors,
  //                 N is the  number of samples,
  //                 t is the  number of trials (avoid epoch boundaries)
  //          n - number of sources {Default: n=m}
  //          p - number of correlation matrices to be diagonalized
  //              {Default: min(100, N/3)} Note that for non-ideal data,
  //              the authors strongly recommend using at least 100 time delays.
  //
  //  Outputs:
  //    winv - Matrix of size [m,n], an estimate of the *mixing* matrix. Its
  //           columns are the component scalp maps. NOTE: This is the inverse
  //           of the usual ICA unmixing weight matrix. Sphering
  //           (pre-whitening), used in the algorithm, is incorporated into
  //           winv. i.e.,
  //
  //              >> icaweights = pinv(winv); icasphere = eye(m);
  //
  //    act  - matrix of dimension [n,N] an estimate of the source activities
  //
  //              >> data            = winv            * act;
  //                 [size m,N]        [size m,n]        [size n,N]
  //              >> act = pinv(winv) * data;
  //
  //  Authors:  A. Belouchrani and A. Cichocki (references: See function body)
  //  Note:     Adapted by Arnaud Delorme and Scott Makeig to process data
  //  epochs by
  //            computing covariances while respecting epoch boundaries.
  //  REFERENCES:
  //  A. Belouchrani, K. Abed-Meraim, J.-F. Cardoso, and E. Moulines,
  //  ``Second-order
  //   blind separation of temporally correlated sources,'' in Proc. Int. Conf.
  //   on Digital Sig. Proc., (Cyprus), pp. 346--351, 1993.
  //
  //   A. Belouchrani and K. Abed-Meraim, ``Separation aveugle au second ordre
  //   de sources correlees,'' in  Proc. Gretsi, (Juan-les-pins), pp. 309--312,
  //   1993.
  //
  //   A. Belouchrani, and A. Cichocki,
  //   Robust whitening procedure in blind source separation context,
  //   Electronics Letters, Vol. 36, No. 24, 2000, pp. 2050-2053.
  //
  //   A. Cichocki and S. Amari,
  //   Adaptive Blind Signal and Image Processing, Wiley,  2003.
  //  Authors note: For non-ideal data, use at least p=100 the time-delayed
  //  covariance matrices.
  N = data.size(1);
  m = data.size(0);
  eigenvectors.set_size(0, 0);
  pca_applied = false;
  if (data.size(0) > ncomps) {
    pca_applied = true;
    pca(data, ncomps, pca_data, eigenvectors);
    N = pca_data.size(1);
    m = pca_data.size(0);
    data.set_size(pca_data.size(0), pca_data.size(1));
    nblocks = pca_data.size(0) * pca_data.size(1);
    for (ib = 0; ib < nblocks; ib++) {
      data[ib] = pca_data[ib];
    }
  }
  // n=m; % Source detection (hum...)
  //  Number of time delayed correlation matrices to be diagonalized
  //
  //  Make the data zero mean
  //
  nblocks = data.size(1);
  lastBlockLength = data.size(0);
  b_data.set_size(data.size(1), data.size(0));
  for (ib = 0; ib < lastBlockLength; ib++) {
    for (xi = 0; xi < nblocks; xi++) {
      b_data[xi + b_data.size(0) * ib] = data[ib + data.size(0) * xi];
    }
  }
  if ((b_data.size(0) == 0) || (b_data.size(1) == 0)) {
    r.set_size(1, b_data.size(1));
    nblocks = b_data.size(1);
    for (ib = 0; ib < nblocks; ib++) {
      r[ib] = 0.0;
    }
  } else {
    npages = b_data.size(1);
    r.set_size(1, b_data.size(1));
    if (b_data.size(0) <= 1024) {
      firstBlockLength = b_data.size(0);
      lastBlockLength = 0;
      nblocks = 1;
    } else {
      firstBlockLength = 1024;
      nblocks = b_data.size(0) / 1024;
      lastBlockLength = b_data.size(0) - (nblocks << 10);
      if (lastBlockLength > 0) {
        nblocks++;
      } else {
        lastBlockLength = 1024;
      }
    }
    for (xi = 0; xi < npages; xi++) {
      xpageoffset = xi * b_data.size(0);
      r[xi] = b_data[xpageoffset];
      for (k = 2; k <= firstBlockLength; k++) {
        r[xi] = r[xi] + b_data[(xpageoffset + k) - 1];
      }
      for (ib = 2; ib <= nblocks; ib++) {
        xblockoffset = xpageoffset + ((ib - 1) << 10);
        bsum = b_data[xblockoffset];
        if (ib == nblocks) {
          hi = lastBlockLength;
        } else {
          hi = 1024;
        }
        for (k = 2; k <= hi; k++) {
          bsum += b_data[(xblockoffset + k) - 1];
        }
        r[xi] = r[xi] + bsum;
      }
    }
  }
  r.set_size(1, r.size(1));
  nblocks = r.size(1) - 1;
  for (ib = 0; ib <= nblocks; ib++) {
    r[ib] = r[ib] / static_cast<double>(b_data.size(0));
  }
  temp.set_size(r.size(1));
  nblocks = r.size(1);
  for (ib = 0; ib < nblocks; ib++) {
    temp[ib] = r[ib];
  }
  npages = temp.size(0);
  V.set_size(temp.size(0), N);
  firstBlockLength = -1;
  for (nblocks = 0; nblocks < N; nblocks++) {
    for (lastBlockLength = 0; lastBlockLength < npages; lastBlockLength++) {
      V[(firstBlockLength + lastBlockLength) + 1] = temp[lastBlockLength];
    }
    firstBlockLength += npages;
  }
  npages = data.size(0) - 1;
  firstBlockLength = data.size(1) - 1;
  V.set_size(data.size(0), data.size(1));
  for (ib = 0; ib <= firstBlockLength; ib++) {
    for (xi = 0; xi <= npages; xi++) {
      V[xi + V.size(0) * ib] =
          data[xi + data.size(0) * ib] - V[xi + V.size(0) * ib];
    }
  }
  nblocks = V.size(1);
  for (ib = 0; ib < nblocks; ib++) {
    lastBlockLength = V.size(0);
    for (xi = 0; xi < lastBlockLength; xi++) {
      data[xi + data.size(0) * ib] = V[xi + V.size(0) * ib];
    }
  }
  //
  //  Pre-whiten the data based directly on SVD
  //
  nblocks = data.size(1);
  lastBlockLength = data.size(0);
  b_data.set_size(data.size(1), data.size(0));
  for (ib = 0; ib < lastBlockLength; ib++) {
    for (xi = 0; xi < nblocks; xi++) {
      b_data[xi + b_data.size(0) * ib] = data[ib + data.size(0) * xi];
    }
  }
  npages = b_data.size(0) * b_data.size(1);
  p = true;
  for (k = 0; k < npages; k++) {
    if ((!p) || (std::isinf(b_data[k]) || std::isnan(b_data[k]))) {
      p = false;
    }
  }
  if (p) {
    coder::internal::svd(b_data, V, s, C);
  } else {
    r1.set_size(b_data.size(0), b_data.size(1));
    nblocks = b_data.size(0) * b_data.size(1);
    for (ib = 0; ib < nblocks; ib++) {
      r1[ib] = 0.0;
    }
    coder::internal::svd(r1, U1, s, V1);
    V.set_size(U1.size(0), U1.size(1));
    nblocks = U1.size(0) * U1.size(1);
    for (ib = 0; ib < nblocks; ib++) {
      V[ib] = rtNaN;
    }
    npages = s.size(0);
    s.set_size(npages);
    for (ib = 0; ib < npages; ib++) {
      s[ib] = rtNaN;
    }
    C.set_size(V1.size(0), V1.size(1));
    nblocks = V1.size(0) * V1.size(1);
    for (ib = 0; ib < nblocks; ib++) {
      C[ib] = rtNaN;
    }
  }
  S.set_size(V.size(1), C.size(1));
  nblocks = V.size(1) * C.size(1);
  for (ib = 0; ib < nblocks; ib++) {
    S[ib] = 0.0;
  }
  ib = s.size(0) - 1;
  for (k = 0; k <= ib; k++) {
    S[k + S.size(0) * k] = s[k];
  }
  if (S.size(0) < S.size(1)) {
    a.set_size(S.size(1), S.size(0));
    nblocks = S.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      lastBlockLength = S.size(1);
      for (xi = 0; xi < lastBlockLength; xi++) {
        a[xi + a.size(0) * ib] = S[ib + S.size(0) * xi];
      }
    }
    coder::eml_pinv(a, S);
    a.set_size(S.size(1), S.size(0));
    nblocks = S.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      lastBlockLength = S.size(1);
      for (xi = 0; xi < lastBlockLength; xi++) {
        a[xi + a.size(0) * ib] = S[ib + S.size(0) * xi];
      }
    }
  } else {
    coder::eml_pinv(S, a);
  }
  if ((a.size(0) == 0) || (a.size(1) == 0) || (C.size(0) == 0) ||
      (C.size(1) == 0)) {
    Q.set_size(a.size(0), C.size(0));
    nblocks = a.size(0) * C.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      Q[ib] = 0.0;
    }
  } else {
    Q.set_size(a.size(0), C.size(0));
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans, (MKL_INT)a.size(0),
                (MKL_INT)C.size(0), (MKL_INT)a.size(1), 1.0, &(a.data())[0],
                (MKL_INT)a.size(0), &(C.data())[0], (MKL_INT)C.size(0), 0.0,
                &(Q.data())[0], (MKL_INT)a.size(0));
  }
  npages = data.size(0) - 1;
  firstBlockLength = data.size(1) - 1;
  b_data.set_size(data.size(0), data.size(1));
  for (ib = 0; ib <= firstBlockLength; ib++) {
    for (xi = 0; xi <= npages; xi++) {
      b_data[xi + b_data.size(0) * ib] = data[xi + data.size(0) * ib];
    }
  }
  if ((Q.size(0) == 0) || (Q.size(1) == 0) || (b_data.size(0) == 0) ||
      (b_data.size(1) == 0)) {
    a.set_size(Q.size(0), b_data.size(1));
    nblocks = Q.size(0) * b_data.size(1);
    for (ib = 0; ib < nblocks; ib++) {
      a[ib] = 0.0;
    }
  } else {
    a.set_size(Q.size(0), b_data.size(1));
    cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, (MKL_INT)Q.size(0),
                (MKL_INT)b_data.size(1), (MKL_INT)Q.size(1), 1.0,
                &(Q.data())[0], (MKL_INT)Q.size(0), &(b_data.data())[0],
                (MKL_INT)b_data.size(0), 0.0, &(a.data())[0],
                (MKL_INT)Q.size(0));
  }
  nblocks = a.size(1);
  for (ib = 0; ib < nblocks; ib++) {
    lastBlockLength = a.size(0);
    for (xi = 0; xi < lastBlockLength; xi++) {
      data[xi + data.size(0) * ib] = a[xi + a.size(0) * ib];
    }
  }
  //  Alternate whitening code
  //  Rx=(X*X')/T;
  //  if m<n, % assumes white noise
  //    [U,D]=eig(Rx);
  //    [puiss,k]=sort(diag(D));
  //    ibl= sqrt(puiss(n-m+1:n)-mean(puiss(1:n-m)));
  //     bl = ones(m,1) ./ ibl ;
  //    BL=diag(bl)*U(1:n,k(n-m+1:n))';
  //    IBL=U(1:n,k(n-m+1:n))*diag(ibl);
  //  else    % assumes no noise
  //     IBL=sqrtm(Rx);
  //     Q=inv(IBL);
  //  end;
  //  X=Q*X;
  //
  //  Estimate the correlation matrices
  //
  bsum = 1.0;
  //  for convenience
  ib = static_cast<int>(
      (std::fmin(100.0, std::ceil(static_cast<double>(N) / 3.0)) *
           static_cast<double>(m) +
       (static_cast<double>(m) - 1.0)) /
      static_cast<double>(m));
  for (xpageoffset = 0; xpageoffset < ib; xpageoffset++) {
    double d;
    bsum++;
    if (bsum > N) {
      xi = -1;
      npages = -1;
    } else {
      xi = static_cast<int>(bsum) - 2;
      npages = N - 1;
    }
    d = (static_cast<double>(N) - bsum) + 1.0;
    if (1.0 > d) {
      nblocks = 0;
    } else {
      nblocks = static_cast<int>(d);
    }
    lastBlockLength = data.size(0);
    firstBlockLength = npages - xi;
    V.set_size(data.size(0), firstBlockLength);
    for (xblockoffset = 0; xblockoffset < firstBlockLength; xblockoffset++) {
      for (hi = 0; hi < lastBlockLength; hi++) {
        V[hi + V.size(0) * xblockoffset] =
            data[hi + data.size(0) * ((xi + xblockoffset) + 1)];
      }
    }
    lastBlockLength = data.size(0);
    S.set_size(data.size(0), nblocks);
    for (xblockoffset = 0; xblockoffset < nblocks; xblockoffset++) {
      for (hi = 0; hi < lastBlockLength; hi++) {
        S[hi + S.size(0) * xblockoffset] =
            data[hi + data.size(0) * xblockoffset];
      }
    }
    if ((data.size(0) != 0) && (firstBlockLength != 0) && (nblocks != 0)) {
      C.set_size(data.size(0), data.size(0));
      cblas_dgemm(CblasColMajor, CblasNoTrans, CblasTrans,
                  (MKL_INT)data.size(0), (MKL_INT)data.size(0),
                  (MKL_INT)(npages - xi), 1.0, &(V.data())[0],
                  (MKL_INT)data.size(0), &(S.data())[0], (MKL_INT)data.size(0),
                  0.0, &(C.data())[0], (MKL_INT)data.size(0));
    }
    //  Frobenius norm =
  }
  //  sqrt(sum(diag(Rxp'*Rxp)))
  //
  //  Perform joint diagonalization
  //
  V.set_size(m, m);
  nblocks = m * m;
  for (ib = 0; ib < nblocks; ib++) {
    V[ib] = 0.0;
  }
  if (m > 0) {
    for (k = 0; k < m; k++) {
      V[k + V.size(0) * k] = 1.0;
    }
  }
  //  while
  //
  //  Estimate the mixing matrix
  //
  if (Q.size(0) < Q.size(1)) {
    a.set_size(Q.size(1), Q.size(0));
    nblocks = Q.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      lastBlockLength = Q.size(1);
      for (xi = 0; xi < lastBlockLength; xi++) {
        a[xi + a.size(0) * ib] = Q[ib + Q.size(0) * xi];
      }
    }
    coder::eml_pinv(a, S);
    a.set_size(S.size(1), S.size(0));
    nblocks = S.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      lastBlockLength = S.size(1);
      for (xi = 0; xi < lastBlockLength; xi++) {
        a[xi + a.size(0) * ib] = S[ib + S.size(0) * xi];
      }
    }
  } else {
    coder::eml_pinv(Q, a);
  }
  coder::internal::blas::mtimes(a, V, mixing);
  if (mixing.size(0) < mixing.size(1)) {
    a.set_size(mixing.size(1), mixing.size(0));
    nblocks = mixing.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      lastBlockLength = mixing.size(1);
      for (xi = 0; xi < lastBlockLength; xi++) {
        a[xi + a.size(0) * ib] = mixing[ib + mixing.size(0) * xi];
      }
    }
    coder::eml_pinv(a, S);
    unmixing.set_size(S.size(1), S.size(0));
    nblocks = S.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      lastBlockLength = S.size(1);
      for (xi = 0; xi < lastBlockLength; xi++) {
        unmixing[xi + unmixing.size(0) * ib] = S[ib + S.size(0) * xi];
      }
    }
  } else {
    coder::eml_pinv(mixing, unmixing);
  }
  if (pca_applied) {
    if (1.0 > ncomps) {
      nblocks = 0;
    } else {
      nblocks = static_cast<int>(ncomps);
    }
    npages = eigenvectors.size(0) - 1;
    lastBlockLength = eigenvectors.size(0);
    for (ib = 0; ib < nblocks; ib++) {
      for (xi = 0; xi < lastBlockLength; xi++) {
        eigenvectors[xi + (npages + 1) * ib] =
            eigenvectors[xi + eigenvectors.size(0) * ib];
      }
    }
    eigenvectors.set_size(npages + 1, nblocks);
    b_data.set_size(unmixing.size(0), unmixing.size(1));
    nblocks = unmixing.size(0) * unmixing.size(1) - 1;
    for (ib = 0; ib <= nblocks; ib++) {
      b_data[ib] = unmixing[ib];
    }
    coder::internal::blas::mtimes(b_data, eigenvectors, unmixing);
  }
}

// End of code generation (sobi_coder.cpp)
