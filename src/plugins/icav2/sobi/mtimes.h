//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  mtimes.h
//
//  Code generation for function 'mtimes'
//


#ifndef MTIMES_H
#define MTIMES_H

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
    namespace blas
    {
      void mtimes(const ::coder::array<double, 2U> &A, const ::coder::array<
                  double, 2U> &B, ::coder::array<double, 2U> &C);
    }
  }
}

#endif

// End of code generation (mtimes.h)
