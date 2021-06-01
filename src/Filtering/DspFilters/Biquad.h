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
#ifndef DSPFILTERS_BIQUAD_H
#define DSPFILTERS_BIQUAD_H

#include "DspFilters/Common.h"
#include "DspFilters/MathSupplement.h"
#include "DspFilters/Types.h"

namespace Dsp {

struct BiquadPoleState;

/*
 * Holds coefficients for a second order Infinite Impulse Response
 * digital filter. This is the building block for all IIR filters.
 *
 */

// Factored interface to prevent outsiders from fiddling
class BiquadBase
{
public:
  template <class StateType>
  struct State : StateType, private DenormalPrevention
  {
    template <typename Sample>
    inline Sample process (const Sample in, const BiquadBase& b)
    {
      return static_cast<Sample> (StateType::process1 (in, b, ac()));
    }
  };

public:
  // Calculate filter response at the given normalized frequency.
  complex_t response (double normalizedFrequency) const;

  std::vector<PoleZeroPair> getPoleZeros () const;

  double getA0 () const { return m_a0; }
  double getA1 () const { return m_a1*m_a0; }
  double getA2 () const { return m_a2*m_a0; }
  double getB0 () const { return m_b0*m_a0; }
  double getB1 () const { return m_b1*m_a0; }
  double getB2 () const { return m_b2*m_a0; }

  // Process a block of samples in the given form
  template <class StateType, typename Sample>
  void process (int numSamples, Sample* dest, StateType& state) const
  {
    while (--numSamples >= 0)
      *dest++ = state.process (*dest, *this);
  }

protected:
  //
  // These are protected so you can't mess with RBJ biquads
  //

  void setCoefficients (double a0, double a1, double a2,
                        double b0, double b1, double b2);

  void setOnePole (complex_t pole, complex_t zero);

  void setTwoPole (complex_t pole1, complex_t zero1,
                   complex_t pole2, complex_t zero2);

  void setPoleZeroPair (const PoleZeroPair& pair)
  {
    if (pair.isSinglePole ())
      setOnePole (pair.poles.first, pair.zeros.first);
    else
      setTwoPole (pair.poles.first, pair.zeros.first,
                  pair.poles.second, pair.zeros.second);
  }

  void setPoleZeroForm (const BiquadPoleState& bps);

  void setIdentity ();

  void applyScale (double scale);

public:
  double m_a0;
  double m_a1;
  double m_a2;
  double m_b1;
  double m_b2;
  double m_b0;
};

//------------------------------------------------------------------------------

// Expresses a biquad as a pair of pole/zeros, with gain
// values so that the coefficients can be reconstructed precisely.
struct BiquadPoleState : PoleZeroPair
{
  BiquadPoleState () { }

  explicit BiquadPoleState (const BiquadBase& s);

  double gain;
};

// More permissive interface for fooling around
class Biquad : public BiquadBase
{
public:
  Biquad ();

  explicit Biquad (const BiquadPoleState& bps);

public:
  // Process a block of samples, interpolating from the old section's coefficients
  // to this section's coefficients, over numSamples. This implements smooth
  // parameter changes.

  template <class StateType, typename Sample>
  void smoothProcess1 (int numSamples,
                       Sample* dest,
                       StateType& state,
                       Biquad sectionPrev) const 
  {
    double t = 1. / numSamples;
    double da1 = (m_a1 - sectionPrev.m_a1) * t;
    double da2 = (m_a2 - sectionPrev.m_a2) * t;
    double db0 = (m_b0 - sectionPrev.m_b0) * t;
    double db1 = (m_b1 - sectionPrev.m_b1) * t;
    double db2 = (m_b2 - sectionPrev.m_b2) * t;

    while (--numSamples >= 0)
    {
      sectionPrev.m_a1 += da1;
      sectionPrev.m_a2 += da2;
      sectionPrev.m_b0 += db0;
      sectionPrev.m_b1 += db1;
      sectionPrev.m_b2 += db2;

      *dest++ = state.process (*dest, sectionPrev);
    }
  }

  // Process a block of samples, interpolating from the old section's pole/zeros
  // to this section's pole/zeros, over numSamples. The interpolation is done
  // in the z-plane using polar coordinates.
  template <class StateType, typename Sample>
  void smoothProcess2 (int numSamples,
                       Sample* dest,
                       StateType& state,
                       BiquadPoleState zPrev) const 
  {
    BiquadPoleState z (*this);
    double t = 1. / numSamples;
    complex_t dp0 = (z.poles.first  - zPrev.poles.first) * t;
    complex_t dp1 = (z.poles.second - zPrev.poles.second) * t;
    complex_t dz0 = (z.zeros.first  - zPrev.zeros.first) * t;
    complex_t dz1 = (z.zeros.second - zPrev.zeros.second) * t;
    double dg = (z.gain - zPrev.gain) * t;

    while (--numSamples >= 0)
    {
      zPrev.poles.first += dp0;
      zPrev.poles.second += dp1;
      zPrev.zeros.first += dz0;
      zPrev.zeros.second += dz1;
      zPrev.gain += dg;

      *dest++ = state.process (*dest, Biquad (zPrev));
    }
  }

public:
  // Export these as public

  void setOnePole (complex_t pole, complex_t zero)
  {
    BiquadBase::setOnePole (pole, zero);
  }

  void setTwoPole (complex_t pole1, complex_t zero1,
                   complex_t pole2, complex_t zero2)
  {
    BiquadBase::setTwoPole (pole1, zero1, pole2, zero2);
  }

  void setPoleZeroPair (const PoleZeroPair& pair)
  {
    BiquadBase::setPoleZeroPair (pair);
  }

  void applyScale (double scale)
  {
    BiquadBase::applyScale (scale);
  }
};

}

#endif
