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
#ifndef DSPFILTERS_CUSTOM_H
#define DSPFILTERS_CUSTOM_H

#include "DspFilters/Common.h"
#include "DspFilters/Biquad.h"
#include "DspFilters/Design.h"
#include "DspFilters/Filter.h"

namespace Dsp {

/*
 * Single pole and Biquad with parameters allowing
 * for directly setting the poles and zeros
 *
 */

namespace Custom {

//
// Raw filters
//

struct OnePole : Biquad
{
  void setup (double scale,
              double pole,
              double zero);
};

struct TwoPole : Biquad
{
  void setup (double scale,
              double poleRho,
              double poleTheta,
              double zeroRho,
              double zeroTheta);
};

//------------------------------------------------------------------------------

//
// Gui-friendly Design layer
//

namespace Design {

struct OnePole : DesignBase, Custom::OnePole
{
  enum
  {
    NumParams = 4
  };

  static int getNumParams ()
  {
    return 4;
  }

  static const ParamInfo getParamInfo_1 ()
  {
    return ParamInfo::defaultGainParam ();
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultPoleRealParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultZeroRealParam ();
  }

  static Kind getKind () { return kindOther; }
  static const char* getName() { return "Custom One-Pole"; }

  void setParams (const Params& params)
  {
    setup (pow (10., params[1]/20),
           params[2],
           params[3]);
  }
};

struct TwoPole : DesignBase, Custom::TwoPole
{
  enum
  {
    NumParams = 6
  };

  static int getNumParams ()
  {
    return 6;
  }

  static const ParamInfo getParamInfo_1 ()
  {
    return ParamInfo::defaultGainParam ();
  }

  static const ParamInfo getParamInfo_2 ()
  {
    return ParamInfo::defaultPoleRhoParam ();
  }

  static const ParamInfo getParamInfo_3 ()
  {
    return ParamInfo::defaultPoleThetaParam ();
  }

  static const ParamInfo getParamInfo_4 ()
  {
    return ParamInfo::defaultZeroRhoParam ();
  }

  static const ParamInfo getParamInfo_5 ()
  {
    return ParamInfo::defaultZeroThetaParam ();
  }


  static Kind getKind () { return kindOther; }
  static const char* getName() { return "Custom Two-Pole"; }

  void setParams (const Params& params)
  {
    setup (pow (10., params[1]/20),
           params[2],
           params[3],
           params[4],
           params[5]);
  }
};

}

}

}

#endif
