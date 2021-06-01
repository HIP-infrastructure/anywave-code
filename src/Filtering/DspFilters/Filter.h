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
#ifndef DSPFILTERS_FILTER_H
#define DSPFILTERS_FILTER_H

#include "DspFilters/Common.h"
#include "DspFilters/MathSupplement.h"
#include "DspFilters/Params.h"
#include "DspFilters/State.h"
#include "DspFilters/Types.h"

namespace Dsp {

/*
 * Filter
 *
 * Full abstraction of a digital IIR filter.
 * Supports run-time introspection and modulation of filter
 * parameters.
 *
 */
class Filter
{
public:
  virtual ~Filter();

  virtual Kind getKind () const = 0;

  virtual const std::string getName () const = 0;

  virtual int getNumParams () const = 0;  

  virtual ParamInfo getParamInfo (int index) const = 0;

  Params getDefaultParams() const;

  const Params& getParams() const
  {
    return m_params;
  }

  double getParam (int paramIndex) const
  {
    assert (paramIndex >= 0 && paramIndex <= getNumParams());
    return m_params[paramIndex];
  }

  void setParam (int paramIndex, double nativeValue)
  {
    assert (paramIndex >= 0 && paramIndex <= getNumParams());
    m_params[paramIndex] = nativeValue;
    doSetParams (m_params);
  }

  int findParamId (int paramId);

  void setParamById (int paramId, double nativeValue);

  void setParams (const Params& parameters)
  {
    m_params = parameters;
    doSetParams (parameters);
  }

  // This makes a best-effort to pick up the values
  // of matching parameters from another set. It uses
  // the ParamID information to make the match.
  void copyParamsFrom (Dsp::Filter const* other);

  virtual std::vector<PoleZeroPair> getPoleZeros() const = 0;
 
  virtual complex_t response (double normalizedFrequency) const = 0;

  virtual int getNumChannels() = 0;
  virtual void reset () = 0;
  virtual void process (int numSamples, float* const* arrayOfChannels) = 0;
  virtual void process (int numSamples, double* const* arrayOfChannels) = 0;

protected:
  virtual void doSetParams (const Params& parameters) = 0;

private:
  Params m_params;
};

//------------------------------------------------------------------------------

/*
 * FilterDesign
 *
 * This container holds a filter Design (Gui-friendly layer) and
 * optionally combines it with the necessary state information to
 * process channel data.
 *
 */

// Factored to reduce template instantiations
template <class DesignClass>
class FilterDesignBase : public Filter
{
public:
  Kind getKind () const
  {
    return m_design.getKind ();
  }

  const std::string getName () const
  {
    return m_design.getName ();
  }

  int getNumParams () const
  {
    return DesignClass::NumParams;
  }

  Params getDefaultParams() const
  {
    return m_design.getDefaultParams();
  }

  ParamInfo getParamInfo (int index) const
  {
    switch (index)
    {
    case 0: return m_design.getParamInfo_0 ();
    case 1: return m_design.getParamInfo_1 ();
    case 2: return m_design.getParamInfo_2 ();
    case 3: return m_design.getParamInfo_3 ();
    case 4: return m_design.getParamInfo_4 ();
    case 5: return m_design.getParamInfo_5 ();
    case 6: return m_design.getParamInfo_6 ();
    case 7: return m_design.getParamInfo_7 ();
    };

    return ParamInfo();
  }

  std::vector<PoleZeroPair> getPoleZeros() const
  {
    return m_design.getPoleZeros();
  }
 
  complex_t response (double normalizedFrequency) const
  {
    return m_design.response (normalizedFrequency);
  }

protected:
  void doSetParams (const Params& parameters)
  {
    m_design.setParams (parameters);
  }

protected:
  DesignClass m_design;
};



template <class DesignClass,
          int Channels = 0,
          class StateType = DirectFormII>
class FilterDesign : public FilterDesignBase <DesignClass>
{
public:
  FilterDesign ()
  {
  }

  int getNumChannels()
  {
    return Channels;
  }

  void reset ()
  {
    m_state.reset();
  }

  void process (int numSamples, float* const* arrayOfChannels)
  {
    m_state.process (numSamples, arrayOfChannels,
                     FilterDesignBase<DesignClass>::m_design);
  }

  void process (int numSamples, double* const* arrayOfChannels)
  {
    m_state.process (numSamples, arrayOfChannels,
                     FilterDesignBase<DesignClass>::m_design);
  }

protected:
  ChannelsState <Channels,
                 typename DesignClass::template State <StateType> > m_state;
};

//------------------------------------------------------------------------------

/*
 * This container combines a raw filter with state information
 * so it can process channels. In order to set up the filter you
 * must call a setup function directly. Smooth changes are
 * not supported, but this class has a smaller footprint.
 *
 */
template <class FilterClass,
          int Channels = 0,
          class StateType = DirectFormII>
class SimpleFilter : public FilterClass
{
public:
  int getNumChannels()
  {
    return Channels;
  }

  void reset ()
  {
    m_state.reset();
  }

  template <typename Sample>
  void process (int numSamples, Sample* const* arrayOfChannels)
  {
    m_state.process (numSamples, arrayOfChannels, *((FilterClass*)this));
  }

protected:
  ChannelsState <Channels,
                 typename FilterClass::template State <StateType> > m_state;
};

}

#endif
