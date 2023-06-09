//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// schur.h
//
// Code generation for function 'schur'
//

#ifndef SCHUR_H
#define SCHUR_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
void schur(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U> &V,
           ::coder::array<double, 2U> &T);

}

#endif
// End of code generation (schur.h)
