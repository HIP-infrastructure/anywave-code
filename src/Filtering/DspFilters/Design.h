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
#ifndef DSPFILTERS_DESIGN_H
#define DSPFILTERS_DESIGN_H

#include "DspFilters/Common.h"
#include "DspFilters/Params.h"

namespace Dsp {

struct DesignBase
{
  // Sampling rate is the first param for every Design filter
  static const ParamInfo getParamInfo_0 ()
  {
    return ParamInfo::defaultSampleRateParam ();
  }

  // These should never get called
  static const ParamInfo getParamInfo_1 () { return ParamInfo(); }
  static const ParamInfo getParamInfo_2 () { return ParamInfo(); }
  static const ParamInfo getParamInfo_3 () { return ParamInfo(); }
  static const ParamInfo getParamInfo_4 () { return ParamInfo(); }
  static const ParamInfo getParamInfo_5 () { return ParamInfo(); }
  static const ParamInfo getParamInfo_6 () { return ParamInfo(); }
  static const ParamInfo getParamInfo_7 () { return ParamInfo(); }
};

}

#endif
