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

//
// Usage Examples
//
// This shows you how to operate the filters
//

// This is the only include you need
#include "DspFilters/Dsp.h"

#include <sstream>
#include <iostream>
#include <iomanip>

namespace {

void UsageExamples ()
{
  // create a two channel audio buffer
  int numSamples = 2000;
  float* audioData[2];
  audioData[0] = new float[numSamples];
  audioData[1] = new float[numSamples];

  // create a 2-channel RBJ Low Pass with parameter smoothing
  // and apply it to the audio data
  {
    // "1024" is the number of samples over which to fade parameter changes
    Dsp::Filter* f = new Dsp::SmoothedFilterDesign
      <Dsp::RBJ::Design::LowPass, 2> (1024);
    Dsp::Params params;
    params[0] = 44100; // sample rate
    params[1] = 4000; // cutoff frequency
    params[2] = 1.25; // Q
    f->setParams (params);
    f->process (numSamples, audioData);
  }
 
  // set up a 2-channel RBJ High Pass with parameter smoothing,
  // but bypass virtual function overhead
  {
    // the difference here is that we don't go through a pointer.
    Dsp::SmoothedFilterDesign <Dsp::RBJ::Design::LowPass, 2> f (1024);
    Dsp::Params params;
    params[0] = 44100; // sample rate
    params[1] = 4000; // cutoff frequency
    params[2] = 1.25; // Q
    f.setParams (params);
    f.process (numSamples, audioData);
  }

  // create a 2-channel Butterworth Band Pass of order 4,
  // with parameter smoothing and apply it to the audio data.
  // Output samples are generated using Direct Form II realization.
  {
    Dsp::Filter* f = new Dsp::SmoothedFilterDesign
      <Dsp::Butterworth::Design::BandPass <4>, 2, Dsp::DirectFormII> (1024);
    Dsp::Params params;
    params[0] = 44100; // sample rate
    params[1] = 4; // order
    params[2] = 4000; // center frequency
    params[3] = 880; // band width
    f->setParams (params);
    f->process (numSamples, audioData);
  }
 
  // create a 2-channel Inverse Chebyshev Low Shelf of order 5
  // and passband ripple 0.1dB, without parameter smoothing and apply it.
  {
    Dsp::Filter* f = new Dsp::FilterDesign
      <Dsp::ChebyshevII::Design::LowShelf <5>, 2>;
    Dsp::Params params;
    params[0] = 44100; // sample rate
    params[1] = 5; // order
    params[2] = 4000; // corner frequency
    params[3] = 6; // shelf gain
    params[4] = 0.1; // passband ripple
    f->setParams (params);
    f->process (numSamples, audioData);
  }
 
  // create an abstract Butterworth High Pass of order 4.
  // This one can't process channels, it can only be used for analysis
  // (i.e. extract poles and zeros).
  {
    Dsp::Filter* f = new Dsp::FilterDesign
      <Dsp::Butterworth::Design::HighPass <4> >;
    Dsp::Params params;
    params[0] = 44100; // sample rate
    params[1] = 4; // order
    params[2] = 4000; // cutoff frequency
    f->setParams (params);
    // this will cause a runtime assertion
    f->process (numSamples, audioData);
  }

  // Use the simple filter API to create a Chebyshev Band Stop of order 3
  // and 1dB ripple in the passband. The simle API has a smaller
  // footprint, but no introspection or smoothing.
  {
    // Note we use the raw filter instead of the one
    // from the Design namespace.
    Dsp::SimpleFilter <Dsp::ChebyshevI::BandStop <3>, 2> f;
    f.setup (3,    // order
             44100,// sample rate
             4000, // center frequency
             880,  // band width
             1);   // ripple dB
    f.process (numSamples, audioData);
  }

  // Set up a filter, extract the coefficients and print them to standard
  // output. Note that this filter is not capable of processing samples,
  // as it has no state. It only has coefficients.
  {
    Dsp::SimpleFilter <Dsp::RBJ::LowPass> f;
    f.setup (44100, // sample rate Hz
             440, // cutoff frequency Hz
             1); // "Q" (resonance)

    std::ostringstream os;

    os << "a0 = " << f.getA0 () << "\n"
       << "a1 = " << f.getA1 () << "\n"
       << "a2 = " << f.getA2 () << "\n"
       << "b0 = " << f.getB0 () << "\n"
       << "b1 = " << f.getB1 () << "\n"
       << "b2 = " << f.getB2 () << "\n";
      ;

    std::cout << os.str();
  }

  // Create an instance of a raw filter. This is as low as it gets, any
  // lower and we will just have either a Biquad or a Cascade, and you'll
  // be setting the coefficients manually.
  {
    // This is basically like eating uncooked food
    Dsp::RBJ::LowPass f;
    f.setup (44100, 440, 1);

    // calculate response at frequency 440 Hz
    Dsp::complex_t response = f.response (440./44100);
  }

  // Extract coefficients from a Cascade
  {
    Dsp::SimpleFilter <Dsp::Butterworth::HighPass <3> > f;
    f.setup (3, 44100, 2000);

    std::ostringstream os;

    os << "numStages = " << f.getNumStages() << "\n"
       << "a0[0] = " << f[0].getA0 () << "\n"
       << "a1[0] = " << f[0].getA1 () << "\n"
       << "a2[0] = " << f[0].getA2 () << "\n"
       << "b0[0] = " << f[0].getB0 () << "\n"
       << "b1[0] = " << f[0].getB1 () << "\n"
       << "b2[0] = " << f[0].getB2 () << "\n"
       << "a0[1] = " << f[1].getA0 () << "\n"
       << "a1[1] = " << f[1].getA1 () << "\n"
       << "a2[1] = " << f[1].getA2 () << "\n"
       << "b0[1] = " << f[1].getB0 () << "\n"
       << "b1[1] = " << f[1].getB1 () << "\n"
       << "b2[1] = " << f[1].getB2 () << "\n"
      ;

    std::cout << os.str();
  }
}

}
