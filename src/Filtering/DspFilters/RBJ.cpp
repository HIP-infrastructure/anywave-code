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
#include "DspFilters/Common.h"
#include "DspFilters/RBJ.h"

namespace Dsp {

namespace RBJ {

void LowPass::setup (double sampleRate,
                     double cutoffFrequency,
                     double q)
{
  double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / (2 * q);
  double b0 = (1 - cs) / 2;
  double b1 =  1 - cs;
  double b2 = (1 - cs) / 2;
  double a0 =  1 + AL;
  double a1 = -2 * cs;
  double a2 =  1 - AL;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void HighPass::setup (double sampleRate,
                      double cutoffFrequency,
                      double q)
{
  double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / ( 2 * q );
  double b0 =  (1 + cs) / 2;
  double b1 = -(1 + cs);
  double b2 =  (1 + cs) / 2;
  double a0 =  1 + AL;
  double a1 = -2 * cs;
  double a2 =  1 - AL;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void BandPass1::setup (double sampleRate,
                       double centerFrequency,
                       double bandWidth)
{
  double w0 = 2 * doublePi * centerFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / ( 2 * bandWidth );
  double b0 = bandWidth * AL;// sn / 2;
  double b1 =  0;
  double b2 = -bandWidth * AL;//-sn / 2;
  double a0 =  1 + AL;
  double a1 = -2 * cs;
  double a2 =  1 - AL;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void BandPass2::setup (double sampleRate,
                       double centerFrequency,
                       double bandWidth)
{
  double w0 = 2 * doublePi * centerFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / ( 2 * bandWidth );
  double b0 =  AL;
  double b1 =  0;
  double b2 = -AL;
  double a0 =  1 + AL;
  double a1 = -2 * cs;
  double a2 =  1 - AL;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void BandStop::setup (double sampleRate,
                      double centerFrequency,
                      double bandWidth)
{
  double w0 = 2 * doublePi * centerFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / ( 2 * bandWidth );
  double b0 =  1;
  double b1 = -2 * cs;
  double b2 =  1;
  double a0 =  1 + AL;
  double a1 = -2 * cs;
  double a2 =  1 - AL;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void LowShelf::setup (double sampleRate,
                      double cutoffFrequency,
                      double gainDb,
                      double shelfSlope)
{
  double A  = pow (10, gainDb/40);
  double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / 2 * ::std::sqrt ((A + 1/A) * (1/shelfSlope - 1) + 2);
  double sq = 2 * sqrt(A) * AL;
  double b0 =    A*( (A+1) - (A-1)*cs + sq );
  double b1 =  2*A*( (A-1) - (A+1)*cs );
  double b2 =    A*( (A+1) - (A-1)*cs - sq );
  double a0 =        (A+1) + (A-1)*cs + sq;
  double a1 =   -2*( (A-1) + (A+1)*cs );
  double a2 =        (A+1) + (A-1)*cs - sq;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void HighShelf::setup (double sampleRate,
                       double cutoffFrequency,
                       double gainDb,
                       double shelfSlope)
{
  double A  = pow (10, gainDb/40);
  double w0 = 2 * doublePi * cutoffFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / 2 * ::std::sqrt ((A + 1/A) * (1/shelfSlope - 1) + 2);
  double sq = 2 * sqrt(A) * AL;
  double b0 =    A*( (A+1) - (A-1)*cs + sq );
  double b1 = -2*A*( (A-1) - (A+1)*cs );
  double b2 =    A*( (A+1) - (A-1)*cs - sq );
  double a0 =        (A+1) + (A-1)*cs + sq;
  double a1 =    2*( (A-1) + (A+1)*cs );
  double a2 =        (A+1) + (A-1)*cs - sq;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void BandShelf::setup (double sampleRate,
                       double centerFrequency,
                       double gainDb,
                       double bandWidth)
{
  double A  = pow (10, gainDb/40);
  double w0 = 2 * doublePi * centerFrequency / sampleRate;
  double cs = cos(w0);
  double sn = sin(w0);
  double AL = sn * sinh( doubleLn2/2 * bandWidth * w0/sn );
  assert (!Dsp::is_nan (AL));
  double b0 =  1 + AL * A;
  double b1 = -2 * cs;
  double b2 =  1 - AL * A;
  double a0 =  1 + AL / A;
  double a1 = -2 * cs;
  double a2 =  1 - AL / A;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

void AllPass::setup (double sampleRate,
                     double phaseFrequency,
                     double q)
{
  double w0 = 2 * doublePi * phaseFrequency / sampleRate;
  double cs = cos (w0);
  double sn = sin (w0);
  double AL = sn / ( 2 * q );
  double b0 =  1 - AL;
  double b1 = -2 * cs;
  double b2 =  1 + AL;
  double a0 =  1 + AL;
  double a1 = -2 * cs;
  double a2 =  1 - AL;
  setCoefficients (a0, a1, a2, b0, b1, b2);
}

}

}
