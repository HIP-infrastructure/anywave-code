//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// sortAscendLE.cpp
//
// Code generation for function 'sortAscendLE'
//

// Include files
#include "sortAscendLE.h"
#include "rt_nonfinite.h"
#include "sobi_coder_rtwutil.h"
#include "rt_defines.h"
#include <cmath>

// Function Declarations
static double rt_atan2d_snf(double u0, double u1);

// Function Definitions
static double rt_atan2d_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else if (std::isinf(u0) && std::isinf(u1)) {
    int b_u0;
    int b_u1;
    if (u0 > 0.0) {
      b_u0 = 1;
    } else {
      b_u0 = -1;
    }
    if (u1 > 0.0) {
      b_u1 = 1;
    } else {
      b_u1 = -1;
    }
    y = std::atan2(static_cast<double>(b_u0), static_cast<double>(b_u1));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }
  return y;
}

namespace coder {
namespace internal {
boolean_T sortAscendLE(const creal_T a, const creal_T b)
{
  boolean_T p;
  if (std::isnan(b.re) || std::isnan(b.im)) {
    p = true;
  } else if (std::isnan(a.re) || std::isnan(a.im)) {
    p = false;
  } else {
    double x;
    double y;
    boolean_T SCALEA;
    boolean_T SCALEB;
    if ((std::abs(a.re) > 8.9884656743115785E+307) ||
        (std::abs(a.im) > 8.9884656743115785E+307)) {
      SCALEA = true;
    } else {
      SCALEA = false;
    }
    if ((std::abs(b.re) > 8.9884656743115785E+307) ||
        (std::abs(b.im) > 8.9884656743115785E+307)) {
      SCALEB = true;
    } else {
      SCALEB = false;
    }
    if (SCALEA || SCALEB) {
      x = rt_hypotd_snf(a.re / 2.0, a.im / 2.0);
      y = rt_hypotd_snf(b.re / 2.0, b.im / 2.0);
    } else {
      x = rt_hypotd_snf(a.re, a.im);
      y = rt_hypotd_snf(b.re, b.im);
    }
    if (x == y) {
      x = rt_atan2d_snf(a.im, a.re);
      y = rt_atan2d_snf(b.im, b.re);
      if (x == y) {
        if (a.re != b.re) {
          if (x >= 0.0) {
            x = b.re;
            y = a.re;
          } else {
            x = a.re;
            y = b.re;
          }
        } else if (a.re < 0.0) {
          x = b.im;
          y = a.im;
        } else {
          x = a.im;
          y = b.im;
        }
        if (x == y) {
          x = 0.0;
          y = 0.0;
        }
      }
    }
    p = (x <= y);
  }
  return p;
}

} // namespace internal
} // namespace coder

// End of code generation (sortAscendLE.cpp)
