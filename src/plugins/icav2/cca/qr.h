//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  qr.h
//
//  Code generation for function 'qr'
//


#ifndef QR_H
#define QR_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  void qr(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U> &Q, ::
          coder::array<double, 2U> &R);
}

#endif

// End of code generation (qr.h)
