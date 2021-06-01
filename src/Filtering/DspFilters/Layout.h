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
#ifndef DSPFILTERS_LAYOUT_H
#define DSPFILTERS_LAYOUT_H

#include "DspFilters/Common.h"
#include "DspFilters/MathSupplement.h"

namespace Dsp {

//
// Describes a filter as a collection of poles and zeros along with
// normalization information to achieve a specified gain at a specified
// frequency. The poles and zeros may lie either in the s or the z plane.
//

// Base uses pointers to reduce template instantiations
class LayoutBase
{
public:
  LayoutBase ()
    : m_numPoles (0)
    , m_maxPoles (0)
  {
  }

  LayoutBase (int maxPoles, PoleZeroPair* pairs)
    : m_numPoles (0)
    , m_maxPoles (maxPoles)
    , m_pair (pairs)
  {
  }

  void setStorage (const LayoutBase& other)
  {
    m_numPoles = 0;
    m_maxPoles = other.m_maxPoles;
    m_pair = other.m_pair;
  }

  void reset ()
  {
    m_numPoles = 0;
  }

  int getNumPoles () const
  {
    return m_numPoles;
  }

  int getMaxPoles () const
  {
    return m_maxPoles;
  }

  void add (const complex_t& pole, const complex_t& zero)
  {
    assert (!(m_numPoles&1)); // single comes last
    assert (!Dsp::is_nan (pole));
    m_pair[m_numPoles/2] = PoleZeroPair (pole, zero);
    ++m_numPoles;
  }

  void addPoleZeroConjugatePairs (const complex_t pole,
                                  const complex_t zero)
  {
    assert (!(m_numPoles&1)); // single comes last
    assert (!Dsp::is_nan (pole));
    m_pair[m_numPoles/2] = PoleZeroPair (
      pole, zero, std::conj (pole), std::conj (zero));
    m_numPoles += 2;
  }

  void add (const ComplexPair& poles, const ComplexPair& zeros)
  {
    assert (!(m_numPoles&1)); // single comes last
    assert (poles.isMatchedPair ());
    assert (zeros.isMatchedPair ());
    m_pair[m_numPoles/2] = PoleZeroPair (poles.first, zeros.first,
                                         poles.second, zeros.second);
    m_numPoles += 2;
  }

  const PoleZeroPair& getPair (int pairIndex) const
  {
    assert (pairIndex >= 0 && pairIndex < (m_numPoles+1)/2);
    return m_pair[pairIndex];
  }

  const PoleZeroPair& operator[] (int pairIndex) const
  {
    return getPair (pairIndex);
  }

  double getNormalW () const
  {
    return m_normalW;
  }

  double getNormalGain () const
  {
    return m_normalGain;
  }

  void setNormal (double w, double g)
  {
    m_normalW = w;
    m_normalGain = g;
  }

private:
  int m_numPoles;
  int m_maxPoles;
  PoleZeroPair* m_pair;
  double m_normalW;
  double m_normalGain;
};

//------------------------------------------------------------------------------

// Storage for Layout
template <int MaxPoles>
class Layout
{
public:
  operator LayoutBase ()
  {
    return LayoutBase (MaxPoles, m_pairs);
  }

private:
  PoleZeroPair m_pairs[(MaxPoles+1)/2];
};

}

#endif
