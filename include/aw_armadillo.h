#pragma once

#if _MSC_VER >= 1700
#define ARMA_USE_CX11
#endif

#define ARMA_USE_BLAS
#define ARMA_DONT_USE_WRAPPER
#define ARMA_USE_LAPACK
#include <armadillo>
using namespace arma;
#if defined(Q_OS_WIN)
#if defined(MKL)
#define ARMA_USE_MKL_ALLOC
#endif
#endif
