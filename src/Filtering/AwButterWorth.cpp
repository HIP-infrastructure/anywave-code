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
#include "AwButterWorth.h"
#include <cmath>
#include <stdlib.h>
#include <QtGlobal>

/* ================================================================================ */
/* ================================================================================ */
/* =====================            COMPLEX NUMBERS           ===================== */
/* ================================================================================ */
/* ================================================================================ */

#define PRIVATE_COMPLEX_LIB
#ifdef PRIVATE_COMPLEX_LIB
/* =====================            COMPLEX NUMBERS           ===================== */
typedef struct
{
    double re;	// real part
    double im;	// imaginary part
} complex;
#define __PRIVATE__ static
#else
#define __PRIVATE__
#endif
/* ================================================================================ */
__PRIVATE__ complex cmplx( double real, double imag)
/* ================================================================================ */
{
complex c;

    c.re = real;
    c.im = imag;
    return c;
}

/* ================================================================================ */
__PRIVATE__ complex cadd(complex x, complex y)
/* ================================================================================ */
{
complex i;

	i.re = x.re + y.re;
	i.im = x.im + y.im;
	return (i);
}

/* ================================================================================ */
__PRIVATE__ complex csub(complex x, complex y)
/* ================================================================================ */
{
complex i;

	i.re = x.re - y.re;
	i.im = x.im - y.im;
	return (i);
}

/* ================================================================================ */
__PRIVATE__ complex cmult(complex x, complex y)
/* ================================================================================ */
{
complex i;

	i.re = x.re * y.re - x.im * y.im;
	i.im = x.re * y.im + x.im * y.re;
	return (i);
}

/* ================================================================================ */
__PRIVATE__ complex cinv(complex x)
/* ================================================================================ */
{
complex i;
double tmpden;

	tmpden = x.re*x.re + x.im*x.im;
	if (tmpden != 0.0)
        tmpden = 1./tmpden;
	else
        tmpden = HUGE_VAL;
	i.re = x.re * tmpden;
	i.im = - x.im * tmpden;
	return (i);
}

/* ================================================================================ */
__PRIVATE__ complex cdiv(complex x, complex y)
/* ================================================================================ */
{
	return (cmult(x,cinv(y)));
}

/* ================================================================================ */
__PRIVATE__ double cabs2(complex x)
/* ================================================================================ */
{
// Cabs2 returns the squared modulus, re**2 + im**2, of the complex number x.
double i;

	i = x.re*x.re + x.im*x.im;
	return (i);
}

/* ================================================================================ */
__PRIVATE__ complex clog(complex x)
/* ================================================================================ */
{
//     Clog returns the principal value of log(a) for an arbitrary
//      complex a.  An equivalent formula is
//                 cmplx(log(cabs(a)),atan2(a.im,a.re)).
//      The imaginary part is between -J and J.

complex i;
double f;

	if ((f=cabs2(x)) == 0.0)
	{
        i.re = -HUGE_VAL;
        i.im = 0.0;
	}
	else
	{
        i.re = 0.5*log((double)f);
        i.im = atan2((double)x.im,(double)x.re);
    }
    return (i);
}

/* ================================================================================ */
__PRIVATE__ complex cexp(complex x)
/* ================================================================================ */
// Cexp returns exp(x) for an arbitrary complex x. An equivalent formula is csmul(ciexp(x.im),exp(x.re)).
{
complex i;
double amp;

	amp = exp((double)(x.re));
	i.re = amp * cos((double)(x.im));
	i.im = amp * sin((double)(x.im));
	return (i);
}

/* ================================================================================ */
__PRIVATE__ complex cspow(complex z, double s)
/* ================================================================================ */
{
/*
     Raise a complex number to a real power
     Cspow returns the principal value of a**s for an arbitrary
     complex a and real exponent s.  An equivalent formula is
     cexp(s*clog(a)).
*/
double amp,phase;

	amp = pow((double)(z.re*z.re+z.im*z.im),(double)(0.5*s));
	phase = s*atan2(z.im,z.re);
	z.re = amp*cos(phase);
	z.im = amp*sin(phase);
	return(z);
}

/* ================================================================================ */
/* ================================================================================ */
/* =====================          BUTTERWORTH FILTERS         ===================== */
/* ================================================================================ */
/* ================================================================================ */

/* ================================================================================ */
static int pad2(int n)
/* ================================================================================ */
{
int  p = 1;

	while ( p < n)
        p *= 2;

    return p;
}


/* ================================================================================ */
static void ftu (double signi, int nx, complex* cx)
/* ================================================================================ */
{
/*
!                      Subroutine ftu                             #
!##################################################################
!   complex fourier transform with unitary scaling                #
!                                                                 #
!               1         nx          signi*2*pi*i*(j-1)*(k-1)/nx #
!   cx(k)  =  -------- * sum cx(j) * e                            #
!             sqrt(nx)   j=1          for k=1,2,...,nx=2**integer #
!##################################################################
subroutine ftu (signi, nx, cx)
  implicit none
  integer nx
  real signi 
  complex cx(nx)
  integer        i, j, k, m, istep, pad2
  real        scale, arg
  complex        cmplx, cw, cdel, ct
  if (nx .ne. pad2(nx)) then
    call erexit('ftu: nx not a power of 2')
  end if
  scale = 1. / sqrt(1.*nx)
  do i= 1, nx  
    cx(i) = cx(i) * scale
  end do 
  j = 1
  k = 1
  do i= 1, nx  
    if (i<=j) then
      ct = cx(j)
      cx(j) = cx(i)
      cx(i) = ct
    end if
    m = nx/2
    do while  (j>m .and. m>1)
      j = j-m
      m = m/2
    end do 
! "&" means .AND.
    j = j+m
  end do 
  do while (1.eq.1)
    istep = 2*k
    cw = 1.
    arg = signi*3.14159265/k
    cdel = cmplx( cos(arg), sin(arg))
    do m= 1, k  
      do i= m, nx, istep  
        ct=cw*cx(i+k)
        cx(i+k)=cx(i)-ct
        cx(i)=cx(i)+ct
      end do 
      cw = cw * cdel
    end do 
    k = istep
    if (k>=nx) then
      exit
    end if
  end do
end subroutine 
*/
int i, j, k, m, istep;
double scale, arg;
complex cw, cdel, ct;

	if (nx != pad2(nx))
	{
       // fprintf(stderr, "ftu: nx not a power of 2\n");
       // exit(0);
  	}
	scale = 1. / sqrt(1.*nx);

    for (i= 0; i < nx; i++)					//	do i= 1, nx
    {
        cx[i].re *= scale;					//	cx(i) = cx(i) * scale
        cx[i].im *= scale;
    }

	j = 0;									//	j = 1

    for (i = 0; i < nx; i++)				//  do i= 1, nx
    {
        if (i < j)							//	if (i<=j) then   (why not < ???)
        {
            ct = cx[j];
            cx[j] = cx[i];
            cx[i] = ct;
        }
        m = nx/2;
        while  ((j >= m) && (m > 1))				//    do while  (j>m .and. m>1)
        {
            j -= m;
            m /= 2;
        }
        j += m;
	} 

	k = 1;									//	k = 1

	while (1)							//	do while (1.eq.1)
    {
        istep = 2 * k;						//	istep = 2*k
        cw = cmplx(1., 0);
        arg = signi*3.14159265/k;
        cdel = cmplx( cos(arg), sin(arg));
        for (m = 0; m < k; m++)				//	do m= 1, k
        {  
            for (i = m; i < nx; i += istep)	//	do i= m, nx, istep
            { 
                ct = cmult(cw, cx[i+k]);	//	ct=cw*cx(i+k)
                cx[i+k] = csub(cx[i], ct);	//	cx(i+k)=cx(i)-ct
                cx[i] = cadd(cx[i], ct);	//	cx(i)=cx(i)+ct
            }
            cw = cmult(cw, cdel);			//	cw = cw * cdel
        } 
        k = istep;							//	k = istep
        if (k >= nx)						//	if (k>=nx) then
            break;
	}
}

/* ================================================================================ */
void kolmogoroff(int n, complex* cx)
/* ================================================================================ */
{
// Spectral factorization.
// input:  cx = spectrum
// output: cx = min phase wavelet
int i;
double scale;

    for (i = 0; i < n; i++)
        cx[i] = clog(cx[i]);

	ftu( -1., n, cx);
//	scaleit( sqrt(1./n), 2*n, cx);
    scale = sqrt(1./n);
    for (i= 0; i < n; i++)
    {
        cx[i].re *= scale;
        cx[i].im *= scale;
    }

    cx[0].re /= 2.;			//	cx(1)     = cx(1)     / 2.
    cx[0].im /= 2.;

    cx[n/2].re /= 2.;		//	cx(1+n/2) = cx(1+n/2) / 2.
    cx[n/2].im /= 2.;

    for (i = 1+n/2; i < n; i++)	//	do i= 1+n/2+1, n
        cx[i] = cmplx(0., 0.);
    ftu( +1., n, cx);

//  scaleit( sqrt(1.*n), 2*n, cx);
    scale = sqrt(1.*n);
    for (i= 0; i < n; i++)
    {
        cx[i].re *= scale;
        cx[i].im *= scale;
    }

    for (i= 0; i < n; i++)
        cx[i] = cexp(cx[i]);

	ftu( -1., n, cx);
//  scaleit( sqrt(1./n), 2*n, cx);
    scale = sqrt(1./n);
    for (i= 0; i < n; i++)
    {
        cx[i].re *= scale;
        cx[i].im *= scale;
    }
}

/* ================================================================================ */
void butter(float hilo, float cutoff, int npoly, double* num, double* den)
/* ================================================================================ */
// Find the numerator and denominator Z-transforms of the Butterworth filter.
//   hilo={1.,-1.} for {low,high}-pass filter
//   cutoff in Nyquist units, i.e. cutoff=1 for (1,-1,1,-1...)(half sample rate)
{
#define SPECTRUM_RESOLUTION (2048)
int nn, i;
int nw = SPECTRUM_RESOLUTION;
double arg, tancut, pi, twopi;
complex *cx/*[SPECTRUM_RESOLUTION]*/;
double w;
complex j, sumD, sumN;

    pi = 3.141592653589793;
    twopi = 6.2831853071795864769252866;
    pi = 4.*atan(1.);
//    nw = 2048;
    cx = (complex*)malloc(nw*sizeof(complex));

    nn = npoly - 1;
    tancut = 2. * tan( cutoff*pi/2. );

#ifdef DEBUG
    printf ("butter: cutoff = %g, tancut = %g\n", cutoff, tancut);
#endif

    for (i = 0; i < nw; i++)	//	do i= 1, nw
    {
        arg = (2. * pi * (double)i / (double)nw) / 2.;	//	arg = (2. * pi * (i-1.) / nw) / 2.
        cx[i].im = 0;	// cx[] are real

        if( hilo > 0. )			// low-pass filter
            //cx(i) = (2.*cos(arg)) **(2*nn) + (2.*sin(arg) * 2./tancut ) **(2*nn)
            cx[i].re = pow(2.*cos(arg), 2.*nn) + pow(2.*sin(arg) * 2./tancut, 2.*nn);
        else					// high-pass filter
            //cx(i) = (2.*sin(arg)) **(2*nn) + (2.*cos(arg) * tancut/2. ) **(2*nn)
            cx[i].re = pow(2.*sin(arg), 2.*nn) + pow(2.*cos(arg) * tancut/2., 2.*nn);
    }

	kolmogoroff(nw, cx);       // spectral factorization

#ifdef DEBUG
    for (i = 0; i < npoly; i++)
        printf ("butter: den[%d] = %g, %g\n", i, cx[i].re, cx[i].im);
#endif

    for (i = 0; i < npoly; i++)
    {
        den[i] = cx[i].re / cx[0].re ;	// normalize to have first term == 1
    }

    for (i= 0; i < nw; i++)                     // numerator
    {
        //cx[i] = (1. + hilo * cexp( cmplx(0., 2.*pi*(i-1.)/nw))) ** nn;
        complex ct;

        ct = cexp( cmplx(0., 2.*pi*(double)i/nw));
        ct.re *= hilo;
        ct.im *= hilo;
        ct.re += 1.;
        cx[i] = cspow(ct, nn);
    }
	ftu( -1., nw, cx);	// unitary fourier transform

    // Now normalize so |H(w)| == 1
    // adapted from Matlab "butter" function

    j = cmplx(0., -1.);	// - imaginary unit

    sumD = cmplx(0., 0.);
    sumN = cmplx(0., 0.);

    if (hilo > 0) 	// low pass
        w = 0;
    else
        w = pi;		// high pass

    for (i = 0; i < npoly; i++)
    {
        complex prod;
        complex kern = cmult(j, cmplx((double)i*w, 0.));
        kern = cexp(kern);

        prod = cmult(kern, cmplx(den[i], 0.));
        sumD = cadd(sumD, prod);

        prod = cmult(kern, cx[i]);
        sumN = cadd(sumN, prod);

#ifdef DEBUG
        printf ("i = %d\n", i);
        printf ("kern = cmult(j, cmplx(i*w,0.)); -> %g + %gi\n", kern.re, kern.im);
        printf ("kern = cexp(kern); -> %g + %gi\n", kern.re, kern.im);
        printf ("prod = cmult(kern, cmplx(den[i], 0.)); -> %g + %gi\n", prod.re, prod.im);
        printf ("sumD = cadd(sumD, prod); -> %g + %gi\n", sumD.re, sumD.im);
        printf ("prod = cmult(kern, cx[i]); -> %g + %gi\n", prod.re, prod.im);
        printf ("sumN = cadd(sumN, prod); -> %g + %gi\n", sumN.re, sumN.im);
#endif
    }

    for (i= 0; i < npoly; i++)
    {
        complex prod = cmult(cx[i], sumD);
        num[i] = cdiv(prod, sumN).re;
#ifdef DEBUG
        printf ("butter: num[%d]/num[0] = %g\n", i, num[i]/num[0]);
#endif
    }
    free(cx);
}

/* ================================================================================ */
LinearFilterPtr BW_Compute(float hilo, int order, float freq, float sampleRate)
/* ================================================================================ */
// Compute and return Butterworth filter.
//   hilo={1.,-1.} for {low,high}-pass filter
{
LinearFilterPtr f;
float nyquist = sampleRate/2.;
int decim;

    if (freq > nyquist)
    {
        //fprintf(stderr, "ComputeButterworth: frequency %g Hz above Nyquist limit %g Hz, quit...\n", freq, nyquist);
		return NULL;
    }
    f = (LinearFilterPtr)calloc(sizeof(LinearFilter), 1);
    order += 1;	// number of coeffs = order + 1
    
    decim = 1;	// decimation factor
    // Butterworth filter is unstable when cutoff frequency is too low
    // limit is .005 for lowpass, .0032 for highpass
    while ((freq * decim) < (0.005 * nyquist))	// if requested frequency too low, decimate data first
        decim++;
    freq /= nyquist/decim;
#ifdef DEBUG
#endif
  //  printf ("BW_Compute: freq = %g Nyquist units, decimation = %d\n", freq, decim);

    f->hilo = hilo;		// hilo={1.,-1.} for {low,high}-pass filter
    f->cutoff = freq;	// in nyquist units
    f->decim = decim;	// decimation factor
    f->numOrder = order;
    f->denOrder = order;
    f->num = (double*)calloc(sizeof(double), order);
    f->den = (double*)calloc(sizeof(double), order);
	if (decim > 1)
		hilo = BW_LowPass;	// force lowpass because decimation is no good for highpass filters
    butter(hilo, freq, order,  f->num, f->den);
    return f;
}

/* ================================================================================ */
double* BW_DownSample(float* data, int n, int factor, int guard)
/* ================================================================================ */
// Down sample data of size n by given factor.
//  Returns pointer to double precision down sampled data
// 	factor must be >= 1
//  also allocate <guard> samples ahead of buffer, for filter initialization
{
int i,j,k;
int dn = n/factor;	// decimated number of samples
double* d;

    if (n % factor)
        dn++;
    d = (double*)malloc((guard+dn)*sizeof(double)) + guard;

    for (i = 0, j = 0; i < dn; i++)
    {
        d[i] = 0.;
        for(k = 0; k < factor; k++)
        {
            d[i] += data[j++];
            if (j >= n)
                j--;	// stick to last original sample
        }
        d[i] /= factor;	// mean value of consecutive samples
#ifdef DEBUG
        if (d[i])
            printf ("BW_DownSample: data = %g at index %d of %d\n", d[i], i, dn);
#endif
    }
    // initialize guard to first sample
    for (i = 0; i < guard; i++)
        d[i-guard] = d[0];

    return d;
}

/* ================================================================================ */
float* BW_UpSample(double* data, int n, int factor)
/* ================================================================================ */
// Resample data of size n by given factor.
//  For each original point, (factor-1) intermediate points are created
//  by interpolating with next original point.
//  Returns pointer to single precision resampled data
// 	factor must be >= 1
{
int i,j,k;
int dn = n*factor;	// new number of samples
float* d;

    d = (float*)malloc(dn*sizeof(float));

    for (i = 0, j = 0; i < dn;)
    {
        double last, next, step;
        
        last = data[j];
        d[i++] = last;
        if (++j < n)
            next = data[j];
        else
        {
            j--;
            next = data[n-2];	// mirror of underlast sample
        }
        step = (next - last)/factor;
        for(k = 1; k < factor; k++)
            d[i++] = last + step * k;
    }

    return d;
}

/* ================================================================================ */
float* BW_Apply(LinearFilterPtr filter, float* data, int samples)
/* ================================================================================ */
// Apply Butterworth filter to data of size n.
//   Returns pointer to filtered data
{
float* output;
double *input, *direct, *reverse;
int n, index, j, order, revIndex;
double x = 0., initValue;
double *num, *den;

    order = filter->numOrder;	// this is the number of coeffs, for both num and den
    num = filter->num;
    den = filter->den;

    // down sampling (also convert to double)
    input = BW_DownSample(data, samples, filter->decim, order);	// input has <order> samples ahead
    n = samples / filter->decim;
    if (samples % filter->decim)
        n++;
	
    // direct filter
	direct = (double*)malloc((order+n+order) * sizeof(double));	// direct has <order> samples ahead AND behind
		
	if ((filter->hilo == BW_LowPass) || (filter->decim > 1))	// highpass with decimation also uses lowpass
		initValue = input[0];
	else
		initValue = 0;

	for (index = 0; index < order; index++)	// presets <order> samples ahead of buffer
        *direct++ = initValue;

    for (index = 0; index < n; index++)
    {        
        x = num[0] * input[index];

        for (j = 1; j < order; j++)
            x += num[j] * input[index-j] - den[j] * direct[index-j];

        direct[index] = x;
    }
    for (index = 0; index < order; index++)	// presets <order> samples at end of buffer
        direct[n+index] = x;	// sets to last sample

    free(input-order);

    // reverse filter
	reverse = (double*)malloc((n+order) * sizeof(double));

	if ((filter->hilo == BW_LowPass) || (filter->decim > 1))	// highpass with decimation also uses lowpass
		initValue = direct[n-1];
	else
		initValue = 0;

    for (index = 0; index < order; index++)	// presets <order> samples at end of buffer
        reverse[n+index] = initValue;

    for (index = 0; index < n; index++)
    {
        revIndex = n-index-1; // n-1 to 0

        x = num[0] * direct[revIndex];

        for (j = 1; j < order; j++)
                x += num[j] * direct[revIndex+j] - den[j] * reverse[revIndex+j];

        reverse[revIndex] = x;
    }

    free(direct-order);

    // return to original sampling, single precision
	output = BW_UpSample(reverse, n, filter->decim);

    free(reverse);

	if ((filter->hilo == BW_HighPass) && (filter->decim > 1))	// highpass with decimation also uses lowpass
		for (index = 0; index < samples; index++)
			output[index] = *data++ - output[index];	// highpass is difference of original with lowpass

    return output;
}

/* ================================================================================ */
void BW_Free( LinearFilterPtr filter)
/* ================================================================================ */
// Free memory space allocated for Butterworth filter.
{
    free(filter->num);
    free(filter->den);
    free(filter);

}