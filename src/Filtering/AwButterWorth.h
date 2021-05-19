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
#ifndef BUTTERWORTH_H
#define BUTTERWORTH_H

/* =====================          BUTTERWORTH FILTERS         ===================== */

typedef struct
{
    float hilo;		// high pass/low pass
    double cutoff;	// cutoff frequency (Nyquist units, 0-1)
    int decim;		// decimation factor before filtering
    int numOrder;	// filter numerator order
    int denOrder;	// filter denominator order
    double* num;	// numerator coeffs
    double* den;	// denominator coeffs
} LinearFilter, *LinearFilterPtr;

#define BW_DefaultOrder (4)
#define BW_LowPass (1.)
#define BW_HighPass (-1.)

LinearFilterPtr	BW_Compute		(float hilo, int order, float freq, float sampleRate);
float*			BW_Apply		(LinearFilterPtr filter, float* data, int n);
void			BW_Print		(LinearFilterPtr filter);
void			BW_Free			(LinearFilterPtr filter);
double*			BW_DownSample	(float* data, int n, int factor, int guard);
float*			BW_UpSample		(double* data, int n, int factor);

#endif