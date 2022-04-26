//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// sobi_coder.h
//
// Code generation for function 'sobi_coder'
//

#ifndef SOBI_CODER_H
#define SOBI_CODER_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#if !defined(Q_OS_MAC) 
#include "omp.h"
#endif
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void sobi_coder(coder::array<double, 2U> &data, double ncomps,
                       coder::array<double, 2U> &mixing,
                       coder::array<double, 2U> &unmixing);

#endif
// End of code generation (sobi_coder.h)
