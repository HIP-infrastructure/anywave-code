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

//
//  Abstract:
//        MATLAB for code generation function to initialize non-finite, Inf and MinusInf

// Include files
#include "rtGetInf.h"
#include "rt_nonfinite.h"

// Function: rtGetInf ==================================================================
//  Abstract:
//  Initialize rtInf needed by the generated code.

real_T rtGetInf(void)
{
  return rtInf;
}

// Function: rtGetInfF =================================================================
//  Abstract:
//  Initialize rtInfF needed by the generated code.

real32_T rtGetInfF(void)
{
  return rtInfF;
}

// Function: rtGetMinusInf =============================================================
//  Abstract:
//  Initialize rtMinusInf needed by the generated code.

real_T rtGetMinusInf(void)
{
  return rtMinusInf;
}

// Function: rtGetMinusInfF ============================================================
//  Abstract:
//  Initialize rtMinusInfF needed by the generated code.

real32_T rtGetMinusInfF(void)
{
  return rtMinusInfF;
}

// End of code generation (rtGetInf.cpp)
