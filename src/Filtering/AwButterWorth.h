/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Authors: Bruno Colombet, Antoine Ducorps � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//    cenir - antoine.ducorps@umpc.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

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