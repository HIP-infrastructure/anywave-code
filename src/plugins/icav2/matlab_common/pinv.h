//
//  Academic License - for use in teaching, academic research, and meeting
//  course requirements at degree granting institutions only.  Not for
//  government, commercial, or other organizational use.
//
//  pinv.h
//
//  Code generation for function 'pinv'
//


#ifndef PINV_H
#define PINV_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  void eml_pinv(const ::coder::array<double, 2U> &A, ::coder::array<double, 2U>
                &X);
}

#endif

// End of code generation (pinv.h)
