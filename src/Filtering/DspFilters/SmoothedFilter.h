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
#ifndef DSPFILTERS_SMOOTHEDFILTER_H
#define DSPFILTERS_SMOOTHEDFILTER_H

#include "DspFilters/Common.h"
#include "DspFilters/Filter.h"
#if _MSC_VER > 1700   // 1700 is Visual Studio 12
#include <algorithm>
#endif
namespace Dsp {

/*
 * Implements smooth modulation of time-varying filter parameters
 *
 */
template <class DesignClass,
          int Channels,
          class StateType = DirectFormII>
class SmoothedFilterDesign
  : public FilterDesign <DesignClass,
                       Channels,
                       StateType>
{
public:
  typedef FilterDesign <DesignClass, Channels, StateType> filter_type_t;

  SmoothedFilterDesign (int transitionSamples)
    : m_transitionSamples (transitionSamples)
    , m_remainingSamples (-1) // first time flag
  {
  }

  // Process a block of samples.
  template <typename Sample>
  void processBlock (int numSamples,
                     Sample* const* destChannelArray)
  {
    const int numChannels = this->getNumChannels();

    // If this goes off it means setup() was never called
    assert (m_remainingSamples >= 0);

    // first handle any transition samples
    int remainingSamples = std::min (m_remainingSamples, numSamples);

    if (remainingSamples > 0)
    {
      // interpolate parameters for each sample
      const double t = 1. / m_remainingSamples;
      double dp[maxParameters];
      for (int i = 0; i < DesignClass::NumParams; ++i)
        dp[i] = (this->getParams()[i] - m_transitionParams[i]) * t;

      for (int n = 0; n < remainingSamples; ++n)
      {
        for (int i = DesignClass::NumParams; --i >=0;)
          m_transitionParams[i] += dp[i];

        m_transitionFilter.setParams (m_transitionParams);
        
        for (int i = numChannels; --i >= 0;)
        {
          Sample* dest = destChannelArray[i]+n;
          *dest = this->m_state[i].process (*dest, m_transitionFilter);
        }
      }

      m_remainingSamples -= remainingSamples;

      if (m_remainingSamples == 0)
        m_transitionParams = this->getParams();
    }

    // do what's left
    if (numSamples - remainingSamples > 0)
    {
      // no transition
      for (int i = 0; i < numChannels; ++i)
        this->m_design.process (numSamples - remainingSamples,
                          destChannelArray[i] + remainingSamples,
                          this->m_state[i]);
    }
  }

  void process (int numSamples, float* const* arrayOfChannels)
  {
    processBlock (numSamples, arrayOfChannels);
  }

  void process (int numSamples, double* const* arrayOfChannels)
  {
    processBlock (numSamples, arrayOfChannels);
  }

protected:
  void doSetParams (const Params& parameters)
  {
    if (m_remainingSamples >= 0)
    {
      m_remainingSamples = m_transitionSamples;
    }
    else
    {
      // first time
      m_remainingSamples = 0;
      m_transitionParams = parameters;
    }

    filter_type_t::doSetParams (parameters);
  }

protected:
  Params m_transitionParams;
  DesignClass m_transitionFilter;
  int m_transitionSamples;

  int m_remainingSamples;        // remaining transition samples
};

}

#endif
