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
#include "DspFilters/Butterworth.h"

namespace Dsp {

namespace Butterworth {

AnalogLowPass::AnalogLowPass ()
  : m_numPoles (-1)
{
  setNormal (0, 1);
}

void AnalogLowPass::design (int numPoles)
{
  if (m_numPoles != numPoles)
  {
    m_numPoles = numPoles;

    reset ();

    const double n2 = 2 * numPoles;
    const int pairs = numPoles / 2;
    for (int i = 0; i < pairs; ++i)
    {
      complex_t c = std::polar (1., doublePi_2 + (2 * i + 1) * doublePi / n2);
      addPoleZeroConjugatePairs (c, infinity());
    }

    if (numPoles & 1)
      add (-1, infinity());
  }
}

//------------------------------------------------------------------------------

AnalogLowShelf::AnalogLowShelf ()
  : m_numPoles (-1)
{
  setNormal (doublePi, 1);
}

void AnalogLowShelf::design (int numPoles, double gainDb)
{
  if (m_numPoles != numPoles ||
      m_gainDb != gainDb)
  {
    m_numPoles = numPoles;
    m_gainDb = gainDb;

    reset ();

    const double n2 = numPoles * 2;
    const double g = pow (pow (10., gainDb/20), 1. / n2);
    const double gp = -1. / g;
    const double gz = -g;

    const int pairs = numPoles / 2;
    for (int i = 1; i <= pairs; ++i)
    {
      const double theta = doublePi * (0.5 - (2 * i - 1) / n2);
      addPoleZeroConjugatePairs (std::polar (gp, theta), std::polar (gz, theta));
    }
    
    if (numPoles & 1)
      add (gp, gz);
  }
}

//------------------------------------------------------------------------------

void LowPassBase::setup (int order,
                         double sampleRate,
                         double cutoffFrequency)
{
  m_analogProto.design (order);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void HighPassBase::setup (int order,
                          double sampleRate,
                          double cutoffFrequency)
{
  m_analogProto.design (order);

  HighPassTransform (cutoffFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandPassBase::setup (int order,
                          double sampleRate,
                          double centerFrequency,
                          double widthFrequency)
{
  m_analogProto.design (order);

  BandPassTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandStopBase::setup (int order,
                          double sampleRate,
                          double centerFrequency,
                          double widthFrequency)
{
  m_analogProto.design (order);

  BandStopTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void LowShelfBase::setup (int order,
                         double sampleRate,
                         double cutoffFrequency,
                         double gainDb)
{
  m_analogProto.design (order, gainDb);

  LowPassTransform (cutoffFrequency / sampleRate,
                    m_digitalProto,
                    m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void HighShelfBase::setup (int order,
                           double sampleRate,
                           double cutoffFrequency,
                           double gainDb)
{
  m_analogProto.design (order, gainDb);

  HighPassTransform (cutoffFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  Cascade::setLayout (m_digitalProto);
}

void BandShelfBase::setup (int order,
                           double sampleRate,
                           double centerFrequency,
                           double widthFrequency,
                           double gainDb)
{
  m_analogProto.design (order, gainDb);

  BandPassTransform (centerFrequency / sampleRate,
                     widthFrequency / sampleRate,
                     m_digitalProto,
                     m_analogProto);

  // HACK!
  m_digitalProto.setNormal (((centerFrequency/sampleRate) < 0.25) ? doublePi : 0, 1);

  Cascade::setLayout (m_digitalProto);
}

}

}
