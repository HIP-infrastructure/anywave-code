//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// rot90.cpp
//
// Code generation for function 'rot90'
//

// Include files
#include "rot90.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
namespace coder {
void rot90(const ::coder::array<double, 1U> &A, ::coder::array<double, 2U> &B)
{
  int b_j;
  int m;
  m = A.size(0);
  B.set_size(1, A.size(0));
  if (A.size(0) >= 8192) {
#pragma omp parallel for num_threads(omp_get_max_threads()) private(b_j)

    for (int j = 0; j < 1; j++) {
      for (b_j = 0; b_j < m; b_j++) {
        B[b_j] = A[b_j];
      }
    }
  } else {
    for (int j{0}; j < m; j++) {
      B[j] = A[j];
    }
  }
}

void rot90(const ::coder::array<double, 2U> &A, ::coder::array<double, 1U> &B)
{
  int b_i;
  int n;
  n = A.size(1);
  B.set_size(A.size(1));
  if (A.size(1) >= 8192) {
    int i;
    i = A.size(1) - 1;
#pragma omp parallel for num_threads(omp_get_max_threads()) private(b_i)

    for (b_i = 0; b_i <= i; b_i++) {
      B[b_i] = A[(n - b_i) - 1];
    }
  } else {
    for (int i{0}; i < n; i++) {
      B[i] = A[(n - i) - 1];
    }
  }
}

} // namespace coder

// End of code generation (rot90.cpp)
