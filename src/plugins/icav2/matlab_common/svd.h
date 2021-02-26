//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  svd.h
//
//  Code generation for function 'svd'
//


#ifndef SVD_H
#define SVD_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  namespace internal
  {
    void b_svd(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U>
               &U, ::coder::array<double, 1U> &s, ::coder::array<double, 2U> &V);
    void svd(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U> &U,
             ::coder::array<double, 1U> &s, ::coder::array<double, 2U> &V);
  }
}

#endif

// End of code generation (svd.h)
