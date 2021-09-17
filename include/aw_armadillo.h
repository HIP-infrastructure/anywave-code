// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#pragma once

#if _MSC_VER >= 1700
#define ARMA_USE_CX11
#endif

//#define ARMA_USE_BLAS
//#define ARMA_DONT_USE_WRAPPER
//#define ARMA_USE_LAPACK
#include <armadillo>
using namespace arma;
#if defined(Q_OS_WIN)
#if defined(MKL)
#define ARMA_USE_MKL_ALLOC
#endif
#endif
