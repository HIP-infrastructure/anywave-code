//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// rot90.h
//
// Code generation for function 'rot90'
//

#ifndef ROT90_H
#define ROT90_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
void rot90(const ::coder::array<double, 1U> &A, ::coder::array<double, 2U> &B);

void rot90(const ::coder::array<double, 2U> &A, ::coder::array<double, 1U> &B);

} // namespace coder

#endif
// End of code generation (rot90.h)
