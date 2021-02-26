//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  sobi_coder.cpp
//
//  Code generation for function 'sobi_coder'
//


// Include files
#include "sobi_coder.h"
#include "mtimes.h"
#include "pinv.h"
#include "rt_nonfinite.h"
#include "svd.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
void sobi_coder(coder::array<double, 2U> &data, coder::array<double, 2U> &mixing,
                coder::array<double, 2U> &unmixing)
{
  CBLAS_TRANSPOSE b_t;
  CBLAS_TRANSPOSE t;
  coder::array<double, 2U> C;
  coder::array<double, 2U> Q;
  coder::array<double, 2U> S;
  coder::array<double, 2U> U1;
  coder::array<double, 2U> V;
  coder::array<double, 2U> V1;
  coder::array<double, 2U> a;
  coder::array<double, 2U> b_data;
  coder::array<double, 2U> r;
  coder::array<double, 2U> r1;
  coder::array<double, 1U> s;
  coder::array<double, 1U> temp;
  double b_k;
  int N;
  int i;
  int i1;
  int k;
  int loop_ub;
  int m;
  int npages;
  int u1;
  int vlen;
  int xpageoffset;
  boolean_T p;

  //  sobi() - Second Order Blind Identification (SOBI) by joint diagonalization of 
  //           correlation  matrices. THIS CODE ASSUMES TEMPORALLY CORRELATED SIGNALS, 
  //           and uses correlations across times in performing the signal separation.  
  //           Thus, estimated time delayed covariance matrices must be nonsingular  
  //           for at least some time delays.
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
  //           of the usual ICA unmixing weight matrix. Sphering (pre-whitening), 
  //           used in the algorithm, is incorporated into winv. i.e.,
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
  //  Note:     Adapted by Arnaud Delorme and Scott Makeig to process data epochs by 
  //            computing covariances while respecting epoch boundaries.
  //  REFERENCES:
  //  A. Belouchrani, K. Abed-Meraim, J.-F. Cardoso, and E. Moulines, ``Second-order 
  //   blind separation of temporally correlated sources,'' in Proc. Int. Conf. on 
  //   Digital Sig. Proc., (Cyprus), pp. 346--351, 1993.
  //
  //   A. Belouchrani and K. Abed-Meraim, ``Separation aveugle au second ordre de 
  //   sources correlees,'' in  Proc. Gretsi, (Juan-les-pins),
  //   pp. 309--312, 1993.
  //
  //   A. Belouchrani, and A. Cichocki,
  //   Robust whitening procedure in blind source separation context,
  //   Electronics Letters, Vol. 36, No. 24, 2000, pp. 2050-2053.
  //
  //   A. Cichocki and S. Amari,
  //   Adaptive Blind Signal and Image Processing, Wiley,  2003.
  //  Authors note: For non-ideal data, use at least p=100 the time-delayed covariance matrices. 
  N = data.size(1);
  m = data.size(0);

  // n=m; % Source detection (hum...)
  u1 = static_cast<int>(std::ceil(static_cast<double>(data.size(1)) / 3.0));

  //  Number of time delayed correlation matrices to be diagonalized
  //
  //  Make the data zero mean
  //
  xpageoffset = data.size(1);
  loop_ub = data.size(0);
  b_data.set_size(data.size(1), data.size(0));
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < xpageoffset; i1++) {
      b_data[i1 + b_data.size(0) * i] = data[i + data.size(0) * i1];
    }
  }

  vlen = b_data.size(0);
  if ((b_data.size(0) == 0) || (b_data.size(1) == 0)) {
    r.set_size(1, b_data.size(1));
    xpageoffset = b_data.size(1);
    for (i = 0; i < xpageoffset; i++) {
      r[i] = 0.0;
    }
  } else {
    npages = b_data.size(1);
    r.set_size(1, b_data.size(1));
    for (loop_ub = 0; loop_ub < npages; loop_ub++) {
      xpageoffset = loop_ub * b_data.size(0);
      r[loop_ub] = b_data[xpageoffset];
      for (k = 2; k <= vlen; k++) {
        r[loop_ub] = r[loop_ub] + b_data[(xpageoffset + k) - 1];
      }
    }
  }

  i = r.size(0) * r.size(1);
  r.set_size(1, r.size(1));
  xpageoffset = i - 1;
  for (i = 0; i <= xpageoffset; i++) {
    r[i] = r[i] / static_cast<double>(b_data.size(0));
  }

  temp.set_size(r.size(1));
  xpageoffset = r.size(1);
  for (i = 0; i < xpageoffset; i++) {
    temp[i] = r[i];
  }

  vlen = temp.size(0);
  npages = data.size(1);
  V.set_size(temp.size(0), data.size(1));
  xpageoffset = -1;
  for (loop_ub = 0; loop_ub < npages; loop_ub++) {
    for (k = 0; k < vlen; k++) {
      xpageoffset++;
      V[xpageoffset] = temp[k];
    }
  }

  vlen = data.size(0) - 1;
  npages = data.size(1) - 1;
  V.set_size(data.size(0), data.size(1));
  for (i = 0; i <= npages; i++) {
    for (i1 = 0; i1 <= vlen; i1++) {
      V[i1 + V.size(0) * i] = data[i1 + data.size(0) * i] - V[i1 + V.size(0) * i];
    }
  }

  xpageoffset = V.size(1);
  for (i = 0; i < xpageoffset; i++) {
    loop_ub = V.size(0);
    for (i1 = 0; i1 < loop_ub; i1++) {
      data[i1 + data.size(0) * i] = V[i1 + V.size(0) * i];
    }
  }

  //
  //  Pre-whiten the data based directly on SVD
  //
  xpageoffset = data.size(1);
  loop_ub = data.size(0);
  b_data.set_size(data.size(1), data.size(0));
  for (i = 0; i < loop_ub; i++) {
    for (i1 = 0; i1 < xpageoffset; i1++) {
      b_data[i1 + b_data.size(0) * i] = data[i + data.size(0) * i1];
    }
  }

  vlen = b_data.size(0) * b_data.size(1);
  p = true;
  for (k = 0; k < vlen; k++) {
    if ((!p) || (rtIsInf(b_data[k]) || rtIsNaN(b_data[k]))) {
      p = false;
    }
  }

  if (p) {
    coder::internal::svd(b_data, V, s, C);
  } else {
    r1.set_size(b_data.size(0), b_data.size(1));
    xpageoffset = b_data.size(0) * b_data.size(1);
    for (i = 0; i < xpageoffset; i++) {
      r1[i] = 0.0;
    }

    coder::internal::svd(r1, U1, s, V1);
    V.set_size(U1.size(0), U1.size(1));
    xpageoffset = U1.size(0) * U1.size(1);
    for (i = 0; i < xpageoffset; i++) {
      V[i] = rtNaN;
    }

    vlen = s.size(0);
    s.set_size(vlen);
    for (i = 0; i < vlen; i++) {
      s[i] = rtNaN;
    }

    C.set_size(V1.size(0), V1.size(1));
    xpageoffset = V1.size(0) * V1.size(1);
    for (i = 0; i < xpageoffset; i++) {
      C[i] = rtNaN;
    }
  }

  S.set_size(V.size(1), C.size(1));
  xpageoffset = V.size(1) * C.size(1);
  for (i = 0; i < xpageoffset; i++) {
    S[i] = 0.0;
  }

  i = s.size(0) - 1;
  for (k = 0; k <= i; k++) {
    S[k + S.size(0) * k] = s[k];
  }

  if (S.size(0) < S.size(1)) {
    a.set_size(S.size(1), S.size(0));
    xpageoffset = S.size(0);
    for (i = 0; i < xpageoffset; i++) {
      loop_ub = S.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        a[i1 + a.size(0) * i] = S[i + S.size(0) * i1];
      }
    }

    coder::eml_pinv(a, S);
    a.set_size(S.size(1), S.size(0));
    xpageoffset = S.size(0);
    for (i = 0; i < xpageoffset; i++) {
      loop_ub = S.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        a[i1 + a.size(0) * i] = S[i + S.size(0) * i1];
      }
    }
  } else {
    coder::eml_pinv(S, a);
  }

  if ((a.size(0) == 0) || (a.size(1) == 0) || (C.size(0) == 0) || (C.size(1) ==
       0)) {
    Q.set_size(a.size(0), C.size(0));
    xpageoffset = a.size(0) * C.size(0);
    for (i = 0; i < xpageoffset; i++) {
      Q[i] = 0.0;
    }
  } else {
    Q.set_size(a.size(0), C.size(0));
    t = CblasNoTrans;
    b_t = CblasTrans;
    cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)a.size(0), (MKL_INT)C.size(0),
                (MKL_INT)a.size(1), 1.0, &(a.data())[0], (MKL_INT)a.size(0),
                &(C.data())[0], (MKL_INT)C.size(0), 0.0, &(Q.data())[0],
                (MKL_INT)a.size(0));
  }

  vlen = data.size(0) - 1;
  npages = data.size(1) - 1;
  b_data.set_size(data.size(0), data.size(1));
  for (i = 0; i <= npages; i++) {
    for (i1 = 0; i1 <= vlen; i1++) {
      b_data[i1 + b_data.size(0) * i] = data[i1 + data.size(0) * i];
    }
  }

  coder::internal::blas::mtimes(Q, b_data, a);
  xpageoffset = a.size(1);
  for (i = 0; i < xpageoffset; i++) {
    loop_ub = a.size(0);
    for (i1 = 0; i1 < loop_ub; i1++) {
      data[i1 + data.size(0) * i] = a[i1 + a.size(0) * i];
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
  b_k = 1.0;

  //  for convenience
  if (100 < u1) {
    u1 = 100;
  }

  i = static_cast<int>((static_cast<double>(u1) * static_cast<double>(m) + (
    static_cast<double>(m) - 1.0)) / static_cast<double>(m));
  for (k = 0; k < i; k++) {
    double d;
    int i2;
    b_k++;
    if (b_k > N) {
      i1 = -1;
      vlen = -1;
    } else {
      i1 = static_cast<int>(b_k) - 2;
      vlen = N - 1;
    }

    d = (static_cast<double>(N) - b_k) + 1.0;
    if (1.0 > d) {
      xpageoffset = 0;
    } else {
      xpageoffset = static_cast<int>(d);
    }

    loop_ub = data.size(0);
    npages = vlen - i1;
    V.set_size(data.size(0), npages);
    for (u1 = 0; u1 < npages; u1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        V[i2 + V.size(0) * u1] = data[i2 + data.size(0) * ((i1 + u1) + 1)];
      }
    }

    loop_ub = data.size(0);
    S.set_size(data.size(0), xpageoffset);
    for (u1 = 0; u1 < xpageoffset; u1++) {
      for (i2 = 0; i2 < loop_ub; i2++) {
        S[i2 + S.size(0) * u1] = data[i2 + data.size(0) * u1];
      }
    }

    if ((data.size(0) != 0) && (npages != 0) && (data.size(0) != 0) &&
        (xpageoffset != 0)) {
      C.set_size(data.size(0), data.size(0));
      t = CblasNoTrans;
      b_t = CblasTrans;
      cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)data.size(0), (MKL_INT)
                  data.size(0), (MKL_INT)(vlen - i1), 1.0, &(V.data())[0],
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
  xpageoffset = m * m;
  for (i = 0; i < xpageoffset; i++) {
    V[i] = 0.0;
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
    xpageoffset = Q.size(0);
    for (i = 0; i < xpageoffset; i++) {
      loop_ub = Q.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        a[i1 + a.size(0) * i] = Q[i + Q.size(0) * i1];
      }
    }

    coder::eml_pinv(a, S);
    a.set_size(S.size(1), S.size(0));
    xpageoffset = S.size(0);
    for (i = 0; i < xpageoffset; i++) {
      loop_ub = S.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        a[i1 + a.size(0) * i] = S[i + S.size(0) * i1];
      }
    }
  } else {
    coder::eml_pinv(Q, a);
  }

  coder::internal::blas::mtimes(a, V, mixing);
  if (mixing.size(0) < mixing.size(1)) {
    a.set_size(mixing.size(1), mixing.size(0));
    xpageoffset = mixing.size(0);
    for (i = 0; i < xpageoffset; i++) {
      loop_ub = mixing.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        a[i1 + a.size(0) * i] = mixing[i + mixing.size(0) * i1];
      }
    }

    coder::eml_pinv(a, S);
    unmixing.set_size(S.size(1), S.size(0));
    xpageoffset = S.size(0);
    for (i = 0; i < xpageoffset; i++) {
      loop_ub = S.size(1);
      for (i1 = 0; i1 < loop_ub; i1++) {
        unmixing[i1 + unmixing.size(0) * i] = S[i + S.size(0) * i1];
      }
    }
  } else {
    coder::eml_pinv(mixing, unmixing);
  }
}

// End of code generation (sobi_coder.cpp)
