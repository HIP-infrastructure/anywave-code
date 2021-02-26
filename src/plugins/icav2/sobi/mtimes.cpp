//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  mtimes.cpp
//
//  Code generation for function 'mtimes'
//


// Include files
#include "mtimes.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "mkl_cblas.h"

// Function Definitions
namespace coder
{
  namespace internal
  {
    namespace blas
    {
      void mtimes(const ::coder::array<double, 2U> &A, const ::coder::array<
                  double, 2U> &B, ::coder::array<double, 2U> &C)
      {
        if ((A.size(0) == 0) || (A.size(1) == 0) || (B.size(0) == 0) || (B.size
             (1) == 0)) {
          int loop_ub;
          C.set_size(A.size(0), B.size(1));
          loop_ub = A.size(0) * B.size(1);
          for (int i = 0; i < loop_ub; i++) {
            C[i] = 0.0;
          }
        } else {
          CBLAS_TRANSPOSE b_t;
          CBLAS_TRANSPOSE t;
          C.set_size(A.size(0), B.size(1));
          t = CblasNoTrans;
          b_t = CblasNoTrans;
          cblas_dgemm(CblasColMajor, t, b_t, (MKL_INT)A.size(0), (MKL_INT)B.size
                      (1), (MKL_INT)A.size(1), 1.0, &(((::coder::array<double,
            2U> *)&A)->data())[0], (MKL_INT)A.size(0), &(((::coder::array<double,
            2U> *)&B)->data())[0], (MKL_INT)B.size(0), 0.0, &(C.data())[0],
                      (MKL_INT)A.size(0));
        }
      }
    }
  }
}

// End of code generation (mtimes.cpp)
