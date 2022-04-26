//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// xdlanv2.cpp
//
// Code generation for function 'xdlanv2'
//

// Include files
#include "xdlanv2.h"
#include "rt_nonfinite.h"
#include "sobi_coder_rtwutil.h"
#include <cmath>

// Function Definitions
namespace coder {
namespace internal {
namespace reflapack {
void xdlanv2(double *a, double *b, double *c, double *d, double *rt1r,
             double *rt1i, double *rt2r, double *rt2i, double *cs, double *sn)
{
  if (*c == 0.0) {
    *cs = 1.0;
    *sn = 0.0;
  } else if (*b == 0.0) {
    double bcmax;
    *cs = 0.0;
    *sn = 1.0;
    bcmax = *d;
    *d = *a;
    *a = bcmax;
    *b = -*c;
    *c = 0.0;
  } else {
    double tau;
    tau = *a - *d;
    if ((tau == 0.0) && ((*b < 0.0) != (*c < 0.0))) {
      *cs = 1.0;
      *sn = 0.0;
    } else {
      double bcmax;
      double bcmis;
      double p;
      double scale;
      double z;
      int b_b;
      int b_c;
      p = 0.5 * tau;
      bcmis = std::abs(*b);
      scale = std::abs(*c);
      bcmax = std::fmax(bcmis, scale);
      if (!(*b < 0.0)) {
        b_b = 1;
      } else {
        b_b = -1;
      }
      if (!(*c < 0.0)) {
        b_c = 1;
      } else {
        b_c = -1;
      }
      bcmis = std::fmin(bcmis, scale) * static_cast<double>(b_b) *
              static_cast<double>(b_c);
      scale = std::fmax(std::abs(p), bcmax);
      z = p / scale * p + bcmax / scale * bcmis;
      if (z >= 8.8817841970012523E-16) {
        *a = std::sqrt(scale) * std::sqrt(z);
        if (p < 0.0) {
          *a = -*a;
        }
        z = p + *a;
        *a = *d + z;
        *d -= bcmax / z * bcmis;
        tau = rt_hypotd_snf(*c, z);
        *cs = z / tau;
        *sn = *c / tau;
        *b -= *c;
        *c = 0.0;
      } else {
        bcmis = *b + *c;
        tau = rt_hypotd_snf(bcmis, tau);
        *cs = std::sqrt(0.5 * (std::abs(bcmis) / tau + 1.0));
        if (!(bcmis < 0.0)) {
          b_b = 1;
        } else {
          b_b = -1;
        }
        *sn = -(p / (tau * *cs)) * static_cast<double>(b_b);
        bcmax = *a * *cs + *b * *sn;
        scale = -*a * *sn + *b * *cs;
        z = *c * *cs + *d * *sn;
        bcmis = -*c * *sn + *d * *cs;
        *b = scale * *cs + bcmis * *sn;
        *c = -bcmax * *sn + z * *cs;
        bcmax = 0.5 * ((bcmax * *cs + z * *sn) + (-scale * *sn + bcmis * *cs));
        *a = bcmax;
        *d = bcmax;
        if (*c != 0.0) {
          if (*b != 0.0) {
            if ((*b < 0.0) == (*c < 0.0)) {
              bcmis = std::sqrt(std::abs(*b));
              z = std::sqrt(std::abs(*c));
              *a = bcmis * z;
              if (!(*c < 0.0)) {
                p = *a;
              } else {
                p = -*a;
              }
              tau = 1.0 / std::sqrt(std::abs(*b + *c));
              *a = bcmax + p;
              *d = bcmax - p;
              *b -= *c;
              *c = 0.0;
              scale = bcmis * tau;
              bcmis = z * tau;
              bcmax = *cs * scale - *sn * bcmis;
              *sn = *cs * bcmis + *sn * scale;
              *cs = bcmax;
            }
          } else {
            *b = -*c;
            *c = 0.0;
            bcmax = *cs;
            *cs = -*sn;
            *sn = bcmax;
          }
        }
      }
    }
  }
  *rt1r = *a;
  *rt2r = *d;
  if (*c == 0.0) {
    *rt1i = 0.0;
    *rt2i = 0.0;
  } else {
    *rt1i = std::sqrt(std::abs(*b)) * std::sqrt(std::abs(*c));
    *rt2i = -*rt1i;
  }
}

} // namespace reflapack
} // namespace internal
} // namespace coder

// End of code generation (xdlanv2.cpp)
