//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// pca.h
//
// Code generation for function 'pca'
//

#ifndef PCA_H
#define PCA_H

// Include files
#include "rtwtypes.h"
#include "coder_array.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
void pca(const coder::array<double, 2U> &data, double ncomps,
         coder::array<double, 2U> &data_pca,
         coder::array<double, 2U> &eigenvectors);

#endif
// End of code generation (pca.h)
