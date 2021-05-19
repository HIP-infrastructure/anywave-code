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
#include "cca.h"
#include "pinv.h"
#include "qr.h"
#include "rt_nonfinite.h"
#include "svd.h"
#include "coder_array.h"
#include "mkl_cblas.h"
#include "rt_nonfinite.h"

// Function Definitions
void cca(coder::array<double, 2U> &data, coder::array<double, 2U> &mixing, coder::
         array<double, 2U> &unmixing)
{
  CBLAS_TRANSPOSE b_t;
  CBLAS_TRANSPOSE t;
  coder::array<double, 2U> C;
  coder::array<double, 2U> Q_x;
  coder::array<double, 2U> Q_yt1;
  coder::array<double, 2U> R_x;
  coder::array<double, 2U> R_yt1;
  coder::array<double, 2U> b_data;
  coder::array<double, 2U> w;
  coder::array<double, 2U> y;
  coder::array<double, 1U> s;
  int i;
  int j;
  int k;
  int vlen;
  int vstride;
  int xoffset;
  boolean_T p;

  //  function co compute cca mixing and unmixing matrices
  //
  //  input :   - data : N*t matrix of data( N chan, t samples)
  //
  //  output :  - unmixing : such that unmixing'*data = sources
  //            - mixing : such that mixing*sources = data
  vlen = data.size(1);
  if ((data.size(0) == 0) || (data.size(1) == 0)) {
    s.set_size(data.size(0));
    j = data.size(0);
    for (k = 0; k < j; k++) {
      s[k] = 0.0;
    }
  } else {
    vstride = data.size(0);
    s.set_size(data.size(0));
    for (j = 0; j < vstride; j++) {
      s[j] = data[j];
    }

    for (k = 2; k <= vlen; k++) {
      xoffset = (k - 1) * vstride;
      for (j = 0; j < vstride; j++) {
        s[j] = s[j] + data[xoffset + j];
      }
    }
  }

  //  zero mean
  vlen = data.size(1);
  j = s.size(0);
  for (k = 0; k < j; k++) {
    s[k] = s[k] / static_cast<double>(data.size(1));
  }

  data.set_size(s.size(0), data.size(1));
  for (k = 0; k < vlen; k++) {
    j = s.size(0);
    for (i = 0; i < j; i++) {
      data[i + data.size(0) * k] = data[i + data.size(0) * k] - s[i];
    }
  }

  //  CCABSS -  Blind Source Separation by Canonical Correlation Analysis
  //
  //  Y = CCABSS(X) is the BSS of X=A*S where S is a set of unknown source signals 
  //  and A is an unknown mixing matrix. The components in S are supposed to
  //  be independent. Y is an estimate of S appart from permutation and scaling. 
  //  For mixed 1-D signals, X is 2-D. The first index refer to the different
  //  components and the second index refers to the signal parameter (e.g. time) 
  //  For mixed images, X is 3-D where the first index refers to the different
  //  mixed images and the second and third indeces are the spatial coordinates. 
  //
  //  [Y W] = CCABSS(X) also gives the 'de-mixing' matrix W, such that Y = W'*X. 
  //
  //  © 2000 Magnus Borga
  //  norm_data=X;
  //  m1=mean(norm_data,2);
  //  norm_data=norm_data-m1(:,ones(1,size(norm_data,2)));
  //  s1=std(norm_data,[],2);
  //  norm_data=norm_data./s1(:,ones(1,size(norm_data,2)));
  //  X=norm_data;
  //    spatial_mode = 0;
  //    if type=='con'
  //        A = X(:,2:end-1);
  //        B = conv2(X,[1 0 1],'valid'); % Temporal correlation
  //    elseif type=='del'
  if (1 > data.size(1) - 1) {
    j = 0;
  } else {
    j = data.size(1) - 1;
  }

  if (2 > data.size(1)) {
    k = 0;
    i = 0;
  } else {
    k = 1;
    i = data.size(1);
  }

  //    end
  //  CCA
  // y = wa'*X;
  //  ------------
  //  --- CCA ----
  //  ------------
  //  --- Calculate covariance matrices ---
  xoffset = data.size(0);
  b_data.set_size(j, data.size(0));
  for (vlen = 0; vlen < xoffset; vlen++) {
    for (vstride = 0; vstride < j; vstride++) {
      b_data[vstride + b_data.size(0) * vlen] = data[vlen + data.size(0) *
        vstride];
    }
  }

  coder::qr(b_data, Q_x, R_x);
  j = data.size(0);
  xoffset = i - k;
  b_data.set_size(xoffset, data.size(0));
  for (i = 0; i < j; i++) {
    for (vlen = 0; vlen < xoffset; vlen++) {
      b_data[vlen + b_data.size(0) * i] = data[i + data.size(0) * (k + vlen)];
    }
  }

  coder::qr(b_data, Q_yt1, R_yt1);
  if ((Q_x.size(0) == 0) || (Q_x.size(1) == 0) || (Q_yt1.size(0) == 0) ||
      (Q_yt1.size(1) == 0)) {
    y.set_size(Q_x.size(1), Q_yt1.size(1));
    j = Q_x.size(1) * Q_yt1.size(1);
    for (k = 0; k < j; k++) {
      y[k] = 0.0;
    }
  } else {
    y.set_size(Q_x.size(1), Q_yt1.size(1));
    t = CblasTrans;
    b_t = CblasNoTrans;
    cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)Q_x.size(1), (MKL_INT)Q_yt1.size
                (1), (MKL_INT)Q_x.size(0), 1.0, &(Q_x.data())[0], (MKL_INT)
                Q_x.size(0), &(Q_yt1.data())[0], (MKL_INT)Q_yt1.size(0), 0.0,
                &(y.data())[0], (MKL_INT)Q_x.size(1));
  }

  vlen = y.size(0) * y.size(1);
  p = true;
  for (k = 0; k < vlen; k++) {
    if ((!p) || (rtIsInf(y[k]) || rtIsNaN(y[k]))) {
      p = false;
    }
  }

  if (p) {
    coder::internal::svd(y, R_yt1, s, Q_yt1);
  } else {
    b_data.set_size(y.size(0), y.size(1));
    j = y.size(0) * y.size(1);
    for (k = 0; k < j; k++) {
      b_data[k] = 0.0;
    }

    coder::internal::svd(b_data, Q_yt1, s, R_yt1);
    R_yt1.set_size(Q_yt1.size(0), Q_yt1.size(1));
    j = Q_yt1.size(0) * Q_yt1.size(1);
    for (k = 0; k < j; k++) {
      R_yt1[k] = rtNaN;
    }
  }

  if ((Q_x.size(0) != 0) && (Q_x.size(1) != 0) && (R_yt1.size(0) != 0) &&
      (R_yt1.size(1) != 0)) {
    C.set_size(Q_x.size(0), R_yt1.size(1));
    t = CblasNoTrans;
    b_t = CblasNoTrans;
    cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)Q_x.size(0), (MKL_INT)R_yt1.size
                (1), (MKL_INT)Q_x.size(1), 1.0, &(Q_x.data())[0], (MKL_INT)
                Q_x.size(0), &(R_yt1.data())[0], (MKL_INT)R_yt1.size(0), 0.0,
                &(C.data())[0], (MKL_INT)Q_x.size(0));
  }

  if (R_x.size(0) < R_x.size(1)) {
    b_data.set_size(R_x.size(1), R_x.size(0));
    j = R_x.size(0);
    for (k = 0; k < j; k++) {
      xoffset = R_x.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = R_x[k + R_x.size(0) * i];
      }
    }

    coder::eml_pinv(b_data, Q_yt1);
    vlen = Q_yt1.size(1);
    b_data.set_size(R_x.size(1), R_x.size(0));
    j = R_x.size(0);
    for (k = 0; k < j; k++) {
      xoffset = R_x.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = R_x[k + R_x.size(0) * i];
      }
    }

    coder::eml_pinv(b_data, Q_yt1);
    vstride = Q_yt1.size(0);
    b_data.set_size(R_x.size(1), R_x.size(0));
    j = R_x.size(0);
    for (k = 0; k < j; k++) {
      xoffset = R_x.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = R_x[k + R_x.size(0) * i];
      }
    }

    coder::eml_pinv(b_data, Q_yt1);
    b_data.set_size(Q_yt1.size(1), Q_yt1.size(0));
    j = Q_yt1.size(0);
    for (k = 0; k < j; k++) {
      xoffset = Q_yt1.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = Q_yt1[k + Q_yt1.size(0) * i];
      }
    }

    C.set_size(vlen, vstride);
    j = vlen * vstride;
    for (k = 0; k < j; k++) {
      C[k] = b_data[k];
    }
  } else {
    coder::eml_pinv(R_x, Q_yt1);
    C.set_size(Q_yt1.size(0), Q_yt1.size(1));
    j = Q_yt1.size(0) * Q_yt1.size(1);
    for (k = 0; k < j; k++) {
      C[k] = Q_yt1[k];
    }
  }

  if ((C.size(0) == 0) || (C.size(1) == 0) || (R_yt1.size(0) == 0) ||
      (R_yt1.size(1) == 0)) {
    w.set_size(C.size(0), R_yt1.size(1));
    j = C.size(0) * R_yt1.size(1);
    for (k = 0; k < j; k++) {
      w[k] = 0.0;
    }
  } else {
    w.set_size(C.size(0), R_yt1.size(1));
    t = CblasNoTrans;
    b_t = CblasNoTrans;
    cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)C.size(0), (MKL_INT)R_yt1.size(1),
                (MKL_INT)C.size(1), 1.0, &(C.data())[0], (MKL_INT)C.size(0),
                &(R_yt1.data())[0], (MKL_INT)R_yt1.size(0), 0.0, &(w.data())[0],
                (MKL_INT)C.size(0));
  }

  unmixing.set_size(w.size(1), w.size(0));
  j = w.size(0);
  for (k = 0; k < j; k++) {
    xoffset = w.size(1);
    for (i = 0; i < xoffset; i++) {
      unmixing[i + unmixing.size(0) * k] = w[k + w.size(0) * i];
    }
  }

  if (unmixing.size(0) < unmixing.size(1)) {
    b_data.set_size(unmixing.size(1), unmixing.size(0));
    j = unmixing.size(0);
    for (k = 0; k < j; k++) {
      xoffset = unmixing.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = unmixing[k + unmixing.size(0) * i];
      }
    }

    coder::eml_pinv(b_data, Q_yt1);
    vlen = Q_yt1.size(1);
    b_data.set_size(unmixing.size(1), unmixing.size(0));
    j = unmixing.size(0);
    for (k = 0; k < j; k++) {
      xoffset = unmixing.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = unmixing[k + unmixing.size(0) * i];
      }
    }

    coder::eml_pinv(b_data, Q_yt1);
    vstride = Q_yt1.size(0);
    b_data.set_size(unmixing.size(1), unmixing.size(0));
    j = unmixing.size(0);
    for (k = 0; k < j; k++) {
      xoffset = unmixing.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = unmixing[k + unmixing.size(0) * i];
      }
    }

    coder::eml_pinv(b_data, Q_yt1);
    b_data.set_size(Q_yt1.size(1), Q_yt1.size(0));
    j = Q_yt1.size(0);
    for (k = 0; k < j; k++) {
      xoffset = Q_yt1.size(1);
      for (i = 0; i < xoffset; i++) {
        b_data[i + b_data.size(0) * k] = Q_yt1[k + Q_yt1.size(0) * i];
      }
    }

    mixing.set_size(vlen, vstride);
    j = vlen * vstride;
    for (k = 0; k < j; k++) {
      mixing[k] = b_data[k];
    }
  } else {
    coder::eml_pinv(unmixing, Q_yt1);
    mixing.set_size(Q_yt1.size(0), Q_yt1.size(1));
    j = Q_yt1.size(0) * Q_yt1.size(1);
    for (k = 0; k < j; k++) {
      mixing[k] = Q_yt1[k];
    }
  }
}

// End of code generation (cca.cpp)
