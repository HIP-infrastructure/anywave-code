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
#include "ica.h"
#include "ICAInfomax.h"
#include "infomax_algo.h"
#include <math.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include "r250.h"


/******************************************************************************/
/* For all support and information please contact:                            */
/*                                                                            */
/*   Sigurd Enghoff                                                           */
/*   The Salk Institute, CNL                                                  */
/*   enghoff@salk.edu                                                         */
/*                                                                            */
/* Additional ICA software:                                                   */
/*   http://www.cnl.salk.edu/~enghoff/                                        */
/*                                                                            */
/* Modification:                                                              */
/*   Chenge version from Version 1.3 (September 20, 1999) to 1.4 (Feb. 2002)  */
/*   Jeng-Ren Duann, 2002-02-14                                               */
/*   CNL, Salk Inst.                            
/*                                                                            */
/******************************************************************************/

#define VER_INFO               "Version 1.4  (Feb. 14, 2002)"

/* If R250 is defined random number are generated using the r250 algorithm    */
/* otherwise standard srand and rand function calls are used                  */
#define R250


#ifdef R250
#define SRAND(seed) r250_init(seed)
#define RAND        r250
#else
#define SRAND(seed) srand(seed)
#define RAND        rand
#endif


/* If KURTOSIS_ESTIMATE is defined sub- and super-Gaussian distributions are  */
/* estimated using:                                                           */
/*    k = E{u^2}^2 / E{x^4} - 3                                               */
/* else:                                                                      */
/*    k = E{sech(u)^2} * E{u^2} - E{tanh(u) * u}                              */

#define KURTOSIS_ESTIMATE

#define MAX_WEIGHT             1e8
#define DEFAULT_STOP           0.000001
#define DEFAULT_ANNEALDEG      60
#define DEFAULT_ANNEALSTEP     0.90
#define DEFAULT_EXTANNEAL      0.98
#define DEFAULT_MAXSTEPS       512
#define DEFAULT_MOMENTUM       0.0

#define DEFAULT_BLOWUP         1000000000.0
#define DEFAULT_BLOWUP_FAC     0.8
#define DEFAULT_RESTART_FAC    0.9

#define MIN_LRATE              0.000001
#define MAX_LRATE              0.1
#define DEFAULT_LRATE(chans)   0.015/std::log((double)chans)

#define DEFAULT_BLOCK(length)  (int)sqrt((double)length/3.0)

/* Extended-ICA option: */
#define DEFAULT_EXTENDED       0
#define DEFAULT_EXTBLOCKS      1
#define DEFAULT_NSUB           1

#define DEFAULT_EXTMOMENTUM    0.5
#define MAX_PDFSIZE            6000
#define MIN_PDFSIZE            2000
#define SIGNCOUNT_THRESHOLD    25

#ifdef KURTOSIS_ESTIMATE
#define DEFAULT_SIGNSBIAS      0.02
#else
#define DEFAULT_SIGNSBIAS      0.0
#endif

#define SIGNCOUNT_STEP         2

#define DEFAULT_BIASFLAG       1

#define MIN(x,y) (((x) > (y)) ? (y) : (x))
#define DEGCONST               180.0/ICA_PI
#define ICA_PI                 3.14159265358979324

#define DEFAULT_PCAFLAG        0
#define DEFAULT_SPHEREFLAG     1
#define DEFAULT_POSACT         1
#define DEFAULT_VERBOSE        1

#define NWINDOW                5
#define FRAMEWINDOW            0
#define FRAMESTEP              1
#define EPOCHWINDOW            2
#define EPOCHSTEP              3
#define BASELINE               4

#ifdef Q_OS_WIN
#ifdef MKL
#include <mkl.h>
#define dscal_ dscal
#define ddot_ ddot
#define idamax_ idamax
#define ilaenv_ ilaenv
#define dcopy_ dcopy
#define dswap_ dswap
#define daxpy_ daxpy
#define dgemv_ dgemv
#define dgemm_ dgemm
#define dsymm_ dsymm
#define dsyrk_ dsyrk
#define dsyev_ dsyev
#define dgesv_ dgesv
#define dgetri_ dgetri
#define dgetrf_ dgetrf
#else
extern "C" int idamax_(int *, double *, int *);
extern "C" void dcopy_(int *, double *, int *, double *, int *);
extern "C" void daxpy_(int *, double *, double *, int *, double *, int *);
extern "C" void dscal_(int *, double *, double *, int *);
extern "C" void dswap_(int *, double *, int *, double *, int *);
extern "C" void dsymm_(char *, char *, int *, int *, double *, double *, int *, double *, int *, double *, double *, int *);
extern "C" int ilaenv_(int *, char *, char *, int *, int *, int *, int *);
#endif
#endif

// #ifdef Q_OS_LINUX
// #include <mkl.h>
// #define dscal_ dscal
// #define ddot_ ddot
// #define idamax_ idamax
// #define ilaenv_ ilaenv
// #define dcopy_ dcopy
// #define dswap_ dswap
// #define daxpy_ daxpy
// #define dgemv_ dgemv
// #define dgemm_ dgemm
// #define dsymm_ dsymm
// #define dsyrk_ dsyrk
// #define dsyev_ dsyev
// #define dgesv_ dgesv
// #define dgetri_ dgetri
// #define dgetrf_ dgetrf
// #endif

// on Mac with clang, always use Accelerate Framework for blas routines even if MKL is installed
#ifdef Q_OS_LINUX
//#include <Accelerate/Accelerate.h>
extern "C" int idamax_(int *, double *, int *);
extern "C" void dcopy_(int *, double *, int *, double *, int *);
extern "C" void dsymm_(char *, char *, int *, int *, double *, double *, int *, double *, int *, double *, double *, int *);
extern "C" void daxpy_(int *, double *, double *, int *, double *, int *);
extern "C" int ilaenv_(int *, char *, char *, int *, int *, int *, int *);
extern "C" void dscal_(int *, double *, double *, int *);
extern "C" void dswap_(int *, double *, int *, double *, int *);
extern "C" void dgetri_(int*, double*, int*, int*, double*, int*, int*);
extern "C" void dgetrf_(int*, int*, double*, int*, int*, int*);
extern "C" double ddot_(int*, double*, int*, double*, int *);
extern "C" void dsyev_(char*, char*, int*, double*, int*, double*, double*, int*, int*);		
extern "C" void dsyrk_(char*, char*, int*, int*, double*, double*, int*, double*, double*, int*);
extern "C" void dgemm_(char*, char*, int*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int *);
// extern "C" void dgemv_(const char* transA, const blas_int* m, const blas_int* n, const double*   alpha, const double*   A, const blas_int* ldA, const double*   x, const blas_int* incx, const double*   beta, double*   y, const blas_int* incy, blas_len transA_len);
 extern "C" void dgemv_(char *, blas_int *, blas_int *, double *, double *, blas_int *, double *, blas_int *, double *, double *, blas_int *);
 extern "C" void dgesv_(int*, int*, double*, int*, int*, double*, int*, int*);
#endif



// on Mac with clang, always use Accelerate Framework for blas routines even if MKL is installed
#ifdef Q_OS_MAC
//#include <Accelerate/Accelerate.h>
extern "C" int idamax_(int *, double *, int *);
extern "C" void dcopy_(int *, double *, int *, double *, int *);
extern "C" void dsymm_(char *, char *, int *, int *, double *, double *, int *, double *, int *, double *, double *, int *);
extern "C" void daxpy_(int *, double *, double *, int *, double *, int *);
extern "C" int ilaenv_(int *, char *, char *, int *, int *, int *, int *);
extern "C" void dscal_(int *, double *, double *, int *);
extern "C" void dswap_(int *, double *, int *, double *, int *);
extern "C" void dgetri_(int*, double*, int*, int*, double*, int*, int*);
extern "C" void dgetrf_(int*, int*, double*, int*, int*, int*);
extern "C" double ddot_(int*, double*, int*, double*, int *);
extern "C" void dsyev_(char*, char*, int*, double*, int*, double*, double*, int*, int*);		
extern "C" void dsyrk_(char*, char*, int*, int*, double*, double*, int*, double*, double*, int*);
extern "C" void dgemm_(char*, char*, int*, int*, int*, double*, double*, int*, double*, int*, double*, double*, int *);
// extern "C" void dgemv_(const char* transA, const blas_int* m, const blas_int* n, const double*   alpha, const double*   A, const blas_int* ldA, const double*   x, const blas_int* incx, const double*   beta, double*   y, const blas_int* incy, blas_len transA_len);
 extern "C" void dgemv_(char *, blas_int *, blas_int *, double *, double *, blas_int *, double *, blas_int *, double *, double *, blas_int *);
 extern "C" void dgesv_(int*, int*, double*, int*, int*, double*, int*, int*);
#endif

using namespace InfoMaxAlgo;

static double dsum_(int *n, double *dx, int *incx)
{


    /* System generated locals */
    int i__1, i__2;
    double ret_val, d__1, d__2, d__3, d__4, d__5, d__6;

    /* Local variables */
    static int i, m;
    static double dtemp;
    static int nincx, mp1;


/*     takes the sum of the values.   
       jack dongarra, linpack, 3/11/78.   
       modified 3/93 to return if incx .le. 0.   
       modified 12/3/93, array(1) declarations changed to array(*)   


    
   Parameter adjustments   
       Function Body */
#define DX(I) dx[(I)-1]


    ret_val = 0.;
    dtemp = 0.;
    if (*n <= 0 || *incx <= 0) {
	return ret_val;
    }
    if (*incx == 1) {
	goto L20;
    }

/*        code for increment not equal to 1 */

    nincx = *n * *incx;
    i__1 = nincx;
    i__2 = *incx;
    for (i = 1; *incx < 0 ? i >= nincx : i <= nincx; i += *incx) {
	dtemp += (d__1 = DX(i), d__1);
/* L10: */
    }
    ret_val = dtemp;
    return ret_val;

/*        code for increment equal to 1   


          clean-up loop */

L20:
    m = *n % 6;
    if (m == 0) {
	goto L40;
    }
    i__2 = m;
    for (i = 1; i <= m; ++i) {
	dtemp += (d__1 = DX(i), d__1);
/* L30: */
    }
    if (*n < 6) {
	goto L60;
    }
L40:
    mp1 = m + 1;
    i__2 = *n;
    for (i = mp1; i <= *n; i += 6) {
	dtemp = dtemp + (d__1 = DX(i), d__1) + (d__2 = DX(i + 1), 
		d__2) + (d__3 = DX(i + 2), d__3) + (d__4 = DX(i + 3), 
		d__4) + (d__5 = DX(i + 4), d__5) + (d__6 = DX(i + 5)
		, d__6);
/* L50: */
    }
L60:
    ret_val = dtemp;
    return ret_val;
} /* dsum_ */


void InfoMaxAlgo::zero(int n, double *A)
{
	double dzero = 0.;
	int izero = 0, ione = 1;
	dcopy_(&n, &dzero, &izero, A, &ione);
}

void InfoMaxAlgo::eye(int m, double *A)
{
	double dzero = 0., done = 1.0;
	int izero = 0, ione = 1;
	int mxm = m * m, m1 = m + 1;
	dcopy_(&mxm, &dzero, &izero, A, &ione);
	dcopy_(&m, &done, &izero, A, &m1);
}

/*********************** Initialize a permutation vector **********************/
/* Initialize a vector perm of n elements for constructing a random.          */
/*                                                                            */
/* permutation of the number in the range [0..n-1]                            */
/* perm: integer array [n] (output)                                           */
/* n:    integer (input)                                                      */

void InfoMaxAlgo::initperm(int *perm, int n)
{
	int i;
	for (i = 0; i < n; i++)
		perm[i] = i;
}

/******************* Obtain and project a random permutation ******************/
/* n elements of vector perm are picked at random and used as indecies into   */
/* data. The designated elements of data are projected by trsf employing the  */
/* bias, if not NULL. k denotes the number of elements of perm left to be     */
/* extracted; perm is updated while calling routine must update k. The        */
/* resulting projections are returned in proj.                                */
/*                                                                            */
/* data: double array [m,?] (input)                                           */
/* trsf: double array [m,m] (input)                                           */
/* bias: double array [m] or NULL (input)                                     */
/* perm: integer array [k>] (input/output)                                    */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* k:    integer (input)                                                      */
/* proj: double array [m,n] (output)                                          */
void InfoMaxAlgo::randperm(double *data, double *trsf, double *bias, int *perm, int m, int n, int k, double *proj)
{
	int i, im, swap, inc = 1;
	double alpha = 1.0, beta;
	char trans = 'N';

	if (bias) {
		for (i = 0, im = 0; i < n; i++, im += m)
			dcopy_(&m, bias, &inc, &proj[im], &inc);
		beta = 1.0;
	}
	else
		beta = 0.;

	for (i = 0, im = 0; i < n; i++, im += m) {
		swap = RAND() % k;
		dgemv_(&trans, &m, &m, &alpha, trsf, &m, &data[perm[swap] * m], &inc, &beta, &proj[im], &inc);
		perm[swap] = perm[--k];
	}
}

/********* Obtain and project data points from Laplacian distribution *********/
/* n samples are picked from a Laplacian distribution centered at the center  */
/* point of a length frames window. Data points are extracted from frames in  */
/* data according to the windowed distribution while chosen uniformly over    */
/* epochs. The designated elements of data are projected by trsf employing    */
/* the bias, if not NULL. The resulting projections are returned in proj.     */
/*                                                                            */
/* data:   double array [m,?] (input)                                         */
/* trsf:   double array [m,m] (input)                                         */
/* bias:   double array [m] or NULL (input)                                   */
/* m:      integer (input)                                                    */
/* n:      integer (input)                                                    */
/* frames: integer (input)                                                    */
/* epochs: integer (input)                                                    */
/* proj:   double array [m,n] (output)                                        */
void InfoMaxAlgo::probperm(double *data, double *trsf, double *bias, int m, int n, int frames, int epochs, double *proj)
{
	int i, im, idx, norm, prob, inc = 1;
	double alpha = 1.0, beta;
	char trans = 'N';

	if (bias) {
		for (i = 0, im = 0; i < n; i++, im += m)
			dcopy_(&m, bias, &inc, &proj[im], &inc);
		beta = 1.0;
	}
	else
		beta = 0.;

	norm = (1<<((frames + 1) / 2 + 1)) - 2 * ((frames % 2) - 1);

	for (i = 0, im = 0; i < n; i++, im += m) {
		prob = RAND() % norm;

		if (prob & 1) {
			idx = 0;
			prob = (prob >> 1) + 1;
			while ( prob > 1) {
				prob >>= 1;
				idx--;
			}
		}
		else {
			idx = frames - 1;
			prob = (prob >> 1) + 1 ;
			while (prob > 1) {
				prob >>= 1;
				idx--;
			}
		}

		idx += frames * (RAND() % epochs);
		dgemv_(&trans, &m, &m, &alpha, trsf, &m, &data[idx * m], &inc, &beta, &proj[im], &inc);
	}
}

/**************************** Compute PDF estimate ****************************/
/* If perm is not NULL, n elements of vector perm are picked at random and    */
/* used as indecies into data. If perm is NULL, the first n elements of data  */
/* are referenced. The designated n elements of data used for the Kurosis     */
/* estimate. k denotes the number of elements of perm left to be extracted;   */
/* perm is updated while calling routine must update k. The resulting         */
/* PDF estimate is returned in kk.                                            */
/*                                                                            */
/* data: double array [m,? or n] (input)                                      */
/* trsf: double array [m,m] (input)                                           */
/* perm: integer array [k>] (input/output) or NULL                            */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* k:    integer (input)                                                      */
/* kk:   double array [m,n] (output)                                          */
void InfoMaxAlgo::pdf(double *data, double *trsf, int *perm, int m, int n, int k, double *kk)
{
	int i, j, im, swap, inc = 1;
	double alpha = 1.0, beta = 0.0, tmp;
	char trans='N';

#ifdef KURTOSIS_ESTIMATE
    zero(2 * m, &kk[m]);

    for (i = 0, im = 0 ; i < n ; i++,im += m) {
        if (perm) {
            swap = RAND()%k;
            dgemv_(&trans,&m,&m,&alpha,trsf,&m,&data[perm[swap]*m],&inc,&beta,kk,&inc);
            perm[swap] = perm[--k];
        }
        else
            dgemv_(&trans,&m,&m,&alpha,trsf,&m,&data[im],&inc,&beta,kk,&inc);
        
        for (j=0 ; j<m ; j++) {
            tmp = kk[j]*kk[j];
            kk[j+m] += tmp;
            kk[j+2*m] += tmp*tmp;
        }
    }

    for (i=0 ; i<m ; i++)
        kk[i] = kk[i+2*m]*(double)n / (kk[i+m]*kk[i+m]) - 3.0;

#else

	zero(3*m,&kk[m]);

	for (i=0,im=0 ; i<n ; i++,im+=m) {
		if (perm) {
			swap = RAND()%k;
			dgemv_(&trans,&m,&m,&alpha,trsf,&m,&data[perm[swap]*m],&inc,&beta,kk,&inc);
			perm[swap] = perm[--k];
		}
		else
			dgemv_(&trans,&m,&m,&alpha,trsf,&m,&data[im],&inc,&beta,kk,&inc);
		
		for (j=0 ; j<m ; j++) {
			tmp = 2.0 / (exp(kk[j])+exp(-kk[j]));
			kk[j+m] += tmp*tmp;
			kk[j+2*m] += kk[j]*kk[j];
			kk[j+3*m] += tanh(kk[j])*kk[j];
		}
	}

	for (i=0 ; i<m ; i++)
		kk[i] = (kk[i+m]*kk[i+2*m]/((double)n)-kk[i+3*m])/((double)n);
#endif
}

/******************* Project data using a general projection ******************/
/* Project data using trsf. Return projections in proj.                       */
/*                                                                            */
/* data: double array [m,n] (input)                                           */
/* trsf: double array [m,m] (input)                                           */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* proj: double array [m,n] (output)                                          */

void InfoMaxAlgo::geproj(double *data, double *trsf, int m, int n, double *proj)
{
	double alpha = 1.0, beta = 0.0;
	char trans='N';

	dgemm_(&trans,&trans,&m,&n,&m,&alpha,trsf,&m,data,&m,&beta,proj,&m);
}

/***************** Project data using a symmetrical projection ****************/
/* Project data using the upper triangular part of trsf, assuming trsf is     */
/* symmetrical. Return projections in proj.                                   */
/*                                                                            */
/* data: double array [m,n] (input)                                           */
/* trsf: double array [m,m] (input)                                           */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* proj: double array [m,n] (output)                                          */

void InfoMaxAlgo::syproj(double *data, double *trsf, int m, int n, double *proj)
{
	double alpha = 1.0, beta = 0.0;
	char uplo='U', side = 'L';

	dsymm_(&side,&uplo,&m,&n,&alpha,trsf,&m,data,&m,&beta,proj,&m);
}

/******************* Project data using a reduced projection ******************/
/* Project data using the first k rows of trsf only. Return projections       */
/* in proj.                                                                   */
/*                                                                            */
/* data: double array [k,n] (input)                                           */
/* trsf: double array [m,k] (input)                                           */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* k:    integer (input)                                                      */
/* proj: double array [m,n] (output)                                          */

void InfoMaxAlgo::pcaproj(double *data, double *eigv, int m, int n, int k, double *proj)
{
	double alpha = 1.0, beta = 0.0;
	char transn='N', transt='T';

	dgemm_(&transt,&transn,&m,&n,&k,&alpha,eigv,&k,data,&k,&beta,proj,&m);
}

/******************************* Remove row mean ******************************/
/* Remove row means from data.                                                */
/*                                                                            */
/* data: double array [m,n] (input/output)                                    */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */

void InfoMaxAlgo::rmmean(double *data, int m, int n)
{
	double *mean, alpha;
	int i, j, k, one = 1;
	
	mean = new double[m];
	zero(m,mean);
	
	alpha = 1.0;
	for (i=0,j=0 ; i<n ; i++,j+=m) {
		daxpy_(&m,&alpha,&data[j],&one,mean,&one);
	}

	alpha = -1.0/(double)n;
	for (i=0,j=0 ; i<n ; i++,j+=m) {
		daxpy_(&m,&alpha,mean,&one,&data[j],&one);
	}	
	
	delete[] mean;
}

/*************************** Compute sphering matrix **************************/
/* Compute sphering matrix based on data.                                     */
/*                                                                            */
/* data: double array [m,n] (input)                                           */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* sphe: double array [m,m] (output)                                          */

void InfoMaxAlgo::do_sphere(double *data, int m, int n, double *sphe)
{
	char name[] = "SSYTRD\0", opts[] = "U\0";
	double alpha = 1.0/(double)(n-1);
	double beta = 0.0;
	int info = 0, ispec = 1, na = -1;
	
	int nb = ilaenv_(&ispec,name,opts,&m,&na,&na,&na);
	int i, im, lwork = (nb+2)*m, inc = 1, mxm = m*m;

	char uplo='U', transn='N', jobz='V';
	double *eigv = new double[m * m];
	double *eigd = new double[m];
	int *ipiv = new int[m];
	double *work = new double[lwork];

/*
   sphere = 2*inv(sqrtm(cov(data'))) is computes as:

   [v d] = eig(data*data')
   sphere = inv(diag(0.5 * sqrt(d))*v')*v'
*/

	dsyrk_(&uplo,&transn,&m,&n,&alpha,data,&m,&beta,sphe,&m);
	dsyev_(&jobz,&uplo,&m,sphe,&m,eigd,work,&lwork,&info);

/* Store transpose of sphe in eigv */
	for (i=0,im=0 ; i<m ; i++,im+=m)
		dcopy_(&m,&sphe[im],&inc,&eigv[i],&m);
	
/* Copy eigv to sphe */
	dcopy_(&mxm,eigv,&inc,sphe,&inc);
	
/* Scale rows of eigv by corresponding eigd values */
	for (i=0 ; i<m ; i++) {
		eigd[i] = 0.5 * sqrt(eigd[i]);
		dscal_(&m,&eigd[i],&eigv[i],&m);
	}

/* Solve eigv * sphere = sphe  ~  diag(0.5*sqrt(d))*v' * sphere = v' */
	dgesv_(&m,&m,eigv,&m,ipiv,sphe,&m,&info);

	delete[] eigv;
	delete[] eigd;
	delete[] ipiv;
	delete[] work;
}

/***************************** Compute PCA matrix *****************************/
/* Compute PCA decomposition matrix based on data.                            */
/*                                                                            */
/* data: double array [m,n] (input)                                           */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* trsf: double array [m,m] (output)                                          */

void InfoMaxAlgo::pca(double *data, int m, int n, double *trsf)
{
	char name[] = "SSYTRD\0", opts[] = "U\0";
	double alpha = 1.0/(double)(n-1);
	double beta = 0.0;
	int info = 0, ispec = 1, na = -1;
	
	int nb = ilaenv_(&ispec,name,opts,&m,&na,&na,&na);
	int lwork = (nb+2)*m;

	char uplo='U', transn='N', jobz='V';
	double *eigd = new double[m];
	double *work = new double[lwork];

	dsyrk_(&uplo,&transn,&m,&n,&alpha,data,&m,&beta,trsf,&m);
	dsyev_(&jobz,&uplo,&m,trsf,&m,eigd,work,&lwork,&info);
	
	delete[] eigd;
	delete[] work;
}

/*************** Orient components toward positive activations ****************/
/* Project data using trsf. Negate components and their corresponding weights */
/* to assure positive RMS. Returns projections in proj.                       */
/*                                                                            */
/* data: double array [m,n] (input)                                           */
/* trsf: double array [m,m] (input/output)                                    */
/* m:    integer (input)                                                      */
/* n:    integer (input)                                                      */
/* proj: double array [m,n] (output)                                          */

void InfoMaxAlgo::posact(double *data, double *trsf, int m, int n, double *proj)
{
	char trans='N';
	double alpha = 1.0;
	double beta = 0.0;
	double posrms, negrms;
	int pos, neg, i, j;
	
	dgemm_(&trans,&trans,&m,&n,&m,&alpha,trsf,&m,data,&m,&beta,proj,&m);

	for (i=0 ; i<m ; i++) {
		posrms = 0.0; negrms = 0.0;
		pos = 0; neg = 0;
		for (j=i ; j<m*n ; j+=m)
			if (proj[j] >= 0) {
				posrms += proj[j]*proj[j];
				pos++;
			}
			else {
				negrms += proj[j]*proj[j];
				neg++;
			}

		if (negrms*(double)pos > posrms*(double)neg) {
			for (j=i ; j<m*n ; j+=m) proj[j] = -proj[j];
			for (j=i ; j<m*m ; j+=m) trsf[j] = -trsf[j];
		}
	}
}


/******************************* Used by varsort ******************************/

int InfoMaxAlgo::compar(const void *x, const void *y)
{
	if (((idxelm*)x)->val < ((idxelm*)y)->val) return 1;
	if (((idxelm*)x)->val > ((idxelm*)y)->val) return -1;
	return 0;
}
/****************** Sort data according to projected variance *****************/
/* Compute back-projected variances for each component based on inverse of    */
/* weights and sphere or pseudoinverse of weights, sphere, and eigv. Reorder  */
/* data and weights accordingly. Also if not NULL, reorder bia and signs.    */
/*                                                                            */
/* data:    double array [m,n] (input/output)                                 */
/* weights: double array [m,m] (input/output)                                 */
/* sphere:  double array [m,m] (input)                                        */
/* eigv:    double array [m,k] (input) or NULL                                */
/* bias:    double array [m] (input/output) or NULL                           */
/* signs:   integer array [m] (input/output) or NULL                          */
/* m:       integer (input)                                                   */
/* n:       integer (input)                                                   */
/* k:       integer (input)                                                   */

void InfoMaxAlgo::varsort(double *data, double *weights, double *sphere, double *eigv, double *bias, int *signs, int m, int n, int k)
{
	char name[] = "DGETRI\0", opts[] = "\0";
	double alpha = 1.0, beta = 0.0;
	int i, j, l, jm, ik, info = 0, ispec = 1, na = -1;
	char transn='N', transt='T', uplo='U', side = 'R';
	
	int nb = ilaenv_(&ispec,name,opts,&m,&na,&na,&na);
	int itmp, lwork = m*nb, inc = 1;
	double act, dtmp, *wcpy;

	int *ipiv = new int[m];
	double *work = new double[lwork];
	double *winv = new double[m * k];
	double *sum = new double[n];
	idxelm *meanvar = new idxelm[m];
	
	if (eigv) {
/* Compute pseudoinverse of weights*sphere*eigv */
		wcpy = new double[m * m];
		dsymm_(&side,&uplo,&m,&m,&alpha,sphere,&m,weights,&m,&beta,wcpy,&m);
		dgemm_(&transn,&transt,&m,&k,&m,&alpha,wcpy,&m,eigv,&k,&beta,weights,&m);

		dgetrf_(&m,&m,wcpy,&m,ipiv,&info);
		dgetri_(&m,wcpy,&m,ipiv,work,&lwork,&info);
		dgemm_(&transn,&transn,&k,&m,&m,&alpha,eigv,&k,wcpy,&m,&beta,winv,&k);
		//free(wcpy);
		delete[] wcpy;
	}
	else {
/* Compute inverse of weights*sphere */
		dsymm_(&side,&uplo,&m,&m,&alpha,sphere,&m,weights,&m,&beta,winv,&m);

		dgetrf_(&m,&m,winv,&m,ipiv,&info);
		dgetri_(&m,winv,&m,ipiv,work,&lwork,&info);
	}


/* Compute mean variances for back-projected components */
	for (i=0 ; i<m*k ; i++) winv[i] = winv[i]*winv[i];

	for (i=0,ik=0 ; i<m ; i++,ik+=k) {
		for (j=0,jm=0 ; j<n ; j++,jm+=m) {
			sum[j] = 0;
			act = data[i+jm]*data[i+jm];
			for(l=0 ; l<k ; l++) sum[j] += act*winv[l+ik];
		}
		
		meanvar[i].idx = i;
		meanvar[i].val = dsum_(&n,sum,&inc);
	}
	
/* Sort meanvar */
	qsort(meanvar,m,sizeof(idxelm),compar);

/* Perform in-place reordering of weights, data, bias, and signs */
	for (i=0 ; i<m-1 ; i++) {
		j = meanvar[i].idx;
		if (i != j) {
			dswap_(&k,&weights[i],&m,&weights[j],&m);
			dswap_(&n,&data[i],&m,&data[j],&m);

			if (bias) {
				dtmp = bias[i];
				bias[i] = bias[j];
				bias[j] = dtmp;
			}

			if (signs) {
				itmp = signs[i];
				signs[i] = signs[j];
				signs[j] = itmp;
			}
			
			for (l=i+1 ; i!=meanvar[l].idx ; l++);
			meanvar[l].idx = j;
		}
	}
	
	delete[] ipiv;
	delete[] work;
	delete[] winv;
	delete[] sum;
	delete[] meanvar;
}

/******************************* Perform infomax ******************************/
/* Perform infomax or extended-infomax on data. If any elements of weights    */
/* are none-zero use weights as starting weights else use identity matrix.    */
/* If bias not NULL employ biasing. The following externally accessible       */
/* variables are assumed initialized: extended, extblocks, pdfsize, nsub,     */
/* verbose, block, maxsteps, lrate, annealstep, annealdeg, nochange, and      */
/* momentum. If the boolean variable extended is set, signs must be defined   */
/* (i.e. not NULL)                                                            */
/*                                                                            */
/* data:    double array [chans,frames*epoch] (input)                         */
/* weights: double array [chans,chans] (input/output)                         */
/* chans:   integer (input)                                                   */
/* frames:  integer (input)                                                   */
/* epochs:  integer (input)                                                   */
/* bias:    double array [chans] (output) or NULL                             */
/* signs:   integer array [chans] (output) or NULL                            */
void ICAInfomax::runica(double *data, double *weights, int chans, int samples, double *bias, int *signs) {
	double alpha = 1.0, beta = 0.0, gamma = -1.0, epsilon;
	double change, oldchange, angledelta;
	char uplo='U', transn='N', transt='T';
	int i, j, t = 0, inc = 1;
	
	int datalength = samples;
	int chxch = chans*chans;
	int norm = (1<<((samples+1)/2+1))-2*(samples%2)-2;

	// init defaults
	double signsbias = DEFAULT_SIGNSBIAS;
	double extmomentum = DEFAULT_EXTMOMENTUM;
	double momentum = DEFAULT_MOMENTUM;

	int wts_blowup = 0;
	int signcount = 0;
	int pleft = 0;
	int step = 0;
	int blockno = 1;
	int urextblocks;
	double *sph, *dataB;

/* Allocate arrays */
	double *startweights = new double[chxch];
	double *oldweights = new double[chxch];
	double *tmpweights = new double[chxch];
	double *delta = new double[chxch];
	double *olddelta = new double[chxch];
	double *u = new double[chans * block];
	double *y = new double[chans * block];
	double *yu = new double[chxch];
	double *prevweights, *prevwtchange;
	double *bsum, *kk, *old_kk;
	int *oldsigns, *pdfperm;

#ifndef PROB_WINDOW
	int *dataperm = new int[datalength];
#endif

/* Initialize weights if zero array */
#if defined(Q_OS_MAC) || defined(MKL_BLAS_LAPACK_FUNCTIONS)
	if (weights[cblas_idamax(chxch, weights, inc) - 1] == 0.0) eye(chans, weights);
#else
	if (weights[idamax_(&chxch,weights,&inc)-1] == 0.0) eye(chans,weights);
#endif

/* Allocate and initialize arrays and variables needed for momentum */
	if (momentum > 0.0) {
		prevweights = new double[chxch];
		prevwtchange = new double[chxch];
		dcopy_(&chxch,weights,&inc,prevweights,&inc);
		zero(chxch,prevwtchange);
	}
	else
		prevweights = prevwtchange = NULL;

/* Allocate and initialize arrays and variables needed for biasing */
	if (bias) {
		bsum = new double[chans];
		zero(chans,bias);
	}
	else
		bsum = NULL;

/* Allocate and initialize arrays and variables needed for extended-ICA */
	if (extended) {
		oldsigns = new int[chans];
		for (i=0 ; i<chans ; i++) oldsigns[i] = -1;
		for (i=0 ; i<nsub ; i++) signs[i] = 1;
		for (i=nsub ; i<chans ; i++) signs[i] = 0;
		
		if (pdfsize < datalength)
			pdfperm = new int[datalength];
		else {
			pdfsize = datalength;
			pdfperm = NULL;
		}
		
#ifdef KURTOSIS_ESTIMATE
		kk = new double[3 * chans];
#else
		kk = new double[4 * chans];
#endif
		old_kk = new double[chans];
		zero(chans,old_kk);
	}
	else {
		oldsigns = pdfperm = NULL;
		old_kk = kk = NULL;
	}
	urextblocks = extblocks;

/*************************** Initialize ICA training **************************/
	emit progressChanged("Initializing ICA training...");
	dcopy_(&chxch,weights,&inc,startweights,&inc);
	dcopy_(&chxch,weights,&inc,oldweights,&inc);
	
#ifdef FIX_SEED	
	SRAND(1);
#else
	SRAND((int)time(NULL));
#endif

	while (step < maxsteps) {

#ifndef PROB_WINDOW
		initperm(dataperm,datalength);
#endif

		if (m_process->isAborted()) {
			if (bsum) delete[] bsum;
			if (oldsigns)  delete[] oldsigns;
			if (prevweights)  delete[] prevweights;
			if (prevwtchange)  delete[] prevwtchange;
			if (startweights)  delete[] startweights;
			if (oldweights)  delete[] oldweights;
			if (tmpweights)  delete[] tmpweights;
			if (delta)  delete[] delta;
			if (olddelta)  delete[] olddelta;
			if (kk)  delete[] kk;
			if (old_kk)  delete[] old_kk;
			if (yu)  delete[] yu;
			if (y)  delete[] y;
			if (u)  delete[] u;
			if (pdfperm)  delete[] pdfperm;

#ifndef PROB_WINDOW
			if (dataperm)  delete[] dataperm;
#endif
			return;
		}

/***************************** ICA training block *****************************/
		for (t=0 ; t<datalength-block && !wts_blowup ; t+=block) {

#ifdef PROB_WINDOW
			probperm(data,weights,bias,chans,block,frames,epochs,u);
#else
			randperm(data,weights,bias,dataperm,chans,block,datalength-t,u);
#endif

			if (!extended) {
/************************* Logistic ICA weight update *************************/
				for (i=0 ; i<chans*block ; i++)
/*					y[i] = 1.0 - 2.0 / (1.0+exp(-u[i]));*/
					y[i] = -tanh(u[i]/2.0);

/*Bias sum for logistic ICA */
				if (bias)
					for (i=0 ; i<chans ; i++)
						bsum[i] = dsum_(&block,&y[i],&chans);

/* Compute: (1-2*y) * u' */
				dgemm_(&transn,&transt,&chans,&chans,&block,&alpha,y,&chans,u,&chans,&beta,yu,&chans);
			}
			else {
/************************* Extended-ICA weight update *************************/
				for (i=0 ; i<chans*block ; i++)
					y[i] = tanh(u[i]);

/* Bias sum for extended-ICA */
				if (bias)
					for (i=0 ; i<chans ; i++)
						bsum[i] = -2*dsum_(&block,&y[i],&chans);

/* Apply sign matrix */
				for (i=0 ; i<chans ; i++)
					if (signs[i])
						for (j=i ; j<block*chans ; j+=chans)
							y[j] = -y[j];

/* Compute: u * u' */
				dsyrk_(&uplo,&transn,&chans,&block,&alpha,u,&chans,&beta,yu,&chans);

				j = chxch - 2;
				for (i=1 ; i<chans ; i++) {
#if defined(Q_OS_MAC) || defined(MKL_BLAS_LAPACK_FUNCTIONS)
					cblas_dcopy(i, &yu[j], chans, &yu[j - chans + 1], inc);
#else
					dcopy_(&i,&yu[j],&chans,&yu[j-chans+1],&inc);
#endif
					j -= chans+1;
				}

/* Compute: -y * u' -u*u' */
				dgemm_(&transn,&transt,&chans,&chans,&block,&gamma,y,&chans,u,&chans,&gamma,yu,&chans);
			}
			
/* Add block identity matix */
			for (i=0 ; i<chxch ; i+=(chans+1))
				yu[i] += (double)block;

/* Apply weight change */
			dcopy_(&chxch,weights,&inc,tmpweights,&inc);
			dgemm_(&transn,&transn,&chans,&chans,&chans,&lrate,yu,&chans,tmpweights,&chans,&alpha,weights,&chans);
			
/* Apply bias change */
#if defined(Q_OS_MAC) || defined(MKL_BLAS_LAPACK_FUNCTIONS)
			if (bias) cblas_daxpy(chans, lrate, bsum, inc, bias, inc);
#else
			if (bias) daxpy_(&chans,&lrate,bsum,&inc,bias,&inc);
#endif
			
/******************************** Add momentum ********************************/
			if (momentum > 0.0) {
				daxpy_(&chxch,&momentum,prevwtchange,&inc,weights,&inc);
				for (i=0 ; i<chxch ; i++)
					prevwtchange[i] = weights[i] - prevweights[i];

				dcopy_(&chxch,weights,&inc,prevweights,&inc);
			}
#if defined(Q_OS_MAC) || defined(MKL_BLAS_LAPACK_FUNCTIONS)
			if (fabs(weights[cblas_idamax(chxch, weights, inc) - 1]) > MAX_WEIGHT)
#else
			if (fabs(weights[idamax_(&chxch,weights,&inc)-1]) > MAX_WEIGHT)
#endif
				wts_blowup = 1;

			if (extended && !wts_blowup && extblocks>0 && blockno%extblocks==0) {
				if (pdfperm && pleft < pdfsize) {
					initperm(pdfperm,datalength);
					pleft = datalength;
				}
				
				pdf(data,weights,pdfperm,chans,pdfsize,pleft,kk);
				pleft -= pdfsize;
				
				if (extmomentum > 0.0) {
					epsilon = 1.0-extmomentum;
					dscal_(&chans,&epsilon,kk,&inc);
					daxpy_(&chans,&extmomentum,old_kk,&inc,kk,&inc);
					dcopy_(&chans,kk,&inc,old_kk,&inc);
				}
				
				for (i=0 ; i<chans ; i++)
					signs[i] = kk[i] < -signsbias;
				
				for (i=0 ; i<chans && signs[i]==oldsigns[i] ; i++);
				if (i==chans) signcount++;
				else signcount = 0;
				
				for (i=0 ; i<chans ; i++)
					oldsigns[i] = signs[i];
								
				if (signcount >= SIGNCOUNT_THRESHOLD) {
					extblocks = (int)(extblocks * SIGNCOUNT_STEP);
					signcount = 0;
				}
			}
			blockno++;			
		}

		if (!wts_blowup) {
			step++;
			angledelta = 0.0;
			
			for (i=0 ; i<chxch ; i++)
				delta[i] = weights[i]-oldweights[i];
			
			change = ddot_(&chxch,delta,&inc,delta,&inc);
		}
		
/************************* Restart if weights blow up *************************/
		if (wts_blowup) {
			step = 0;
			change = nochange;
			wts_blowup = 0;
			blockno = 1;
			extblocks = urextblocks;
			lrate = lrate*DEFAULT_RESTART_FAC;
			dcopy_(&chxch,startweights,&inc,weights,&inc);
			zero(chxch,delta);
			zero(chxch,olddelta);

			if (bias) zero(chans,bias);
			
			if (momentum > 0.0) {
				dcopy_(&chxch,startweights,&inc,oldweights,&inc);
				dcopy_(&chxch,startweights,&inc,prevweights,&inc);
				zero(chxch,prevwtchange);
			}
						
			if (extended) {
				for (i=0 ; i<chans ; i++) oldsigns[i] = -1;
				for (i=0 ; i<nsub ; i++) signs[i] = 1;
				for (i=nsub ; i<chans ; i++) signs[i] = 0;
			}
			
			if (lrate > MIN_LRATE)
				emit progressChanged(QString("Lowering learning rate to %1 and starting again.").arg(lrate));
			else {
				emit progressChanged("QUITTING - weight matrix may not be invertible!");
				return;
			}
		}
		else {

/*********************** Print weight update information **********************/
			if (step > 2) {
				epsilon = ddot_(&chxch,delta,&inc,olddelta,&inc);
				angledelta = acos(epsilon/sqrt(change*oldchange));

				if (!extended) {
					QString s = QString("step %1 - lrate %2, wchange %3, angledelta %4 deg").arg(step).arg(lrate).arg(change).arg(DEGCONST * angledelta);
					emit progressChanged(s);
				}
				else {
					for (i=0,j=0 ; i<chans ; i++) j += signs[i];
					QString s =  QString("step %1 - lrate %2, wchange %3, angledelta %4 deg %5 subgauss").arg(step).arg(lrate).arg(change).arg(DEGCONST * angledelta).arg(j);
					emit progressChanged(s);
				}
			}
			else
				if (!extended) {
					QString s = QString("step %1 - lrate %2, wchange %3").arg(step).arg(lrate).arg(change);
					emit progressChanged(s);
				}
				else {
					
					for (i=0,j=0 ; i<chans ; i++) j += signs[i];
					QString s = QString("step %1 - lrate %2, wchange %3, %4 subgauss").arg(step).arg(lrate).arg(j);
					emit progressChanged(s);
				}
		}

/**************************** Save current values *****************************/
		dcopy_(&chxch,weights,&inc,oldweights,&inc);
		
		if (DEGCONST*angledelta > annealdeg) {
			dcopy_(&chxch,delta,&inc,olddelta,&inc);
			lrate = lrate*annealstep;
			oldchange = change;
		}
		else
			if (step == 1) {
				dcopy_(&chxch,delta,&inc,olddelta,&inc);
				oldchange = change;
			}

		if (step > 2 && change < nochange)
			step = maxsteps;
		else
			if (change > DEFAULT_BLOWUP)
				lrate = lrate*DEFAULT_BLOWUP_FAC;
	}

	if (bsum) delete[] bsum;
	if (oldsigns)  delete[] oldsigns;
	if (prevweights)  delete[] prevweights;
	if (prevwtchange)  delete[] prevwtchange;
	if (startweights)  delete[] startweights;
	if (oldweights)  delete[] oldweights;
	if (tmpweights)  delete[] tmpweights;
	if (delta)  delete[] delta;
	if (olddelta)  delete[] olddelta;
	if (kk)  delete[] kk;
	if (old_kk)  delete[] old_kk;
	if (yu)  delete[] yu;
	if (y)  delete[] y;
	if (u)  delete[] u;
	if (pdfperm)  delete[] pdfperm;

#ifndef PROB_WINDOW
	if (dataperm)  delete[] dataperm;
#endif
}

void ICAInfomax::run()
{
	int  m = m_channels.size();
	qint64 n = m_channels.first()->dataSize();
	int nc = m_nComp;
	if (nc == 0)
		nc = m;
	// build data matrix COLUMN MAJOR
	double *M = new double[m*n];
	double *save = M;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			*M++ = (double)m_channels.at(j)->data()[i];

	M = save;

	// saving memory by clearing loaded data
	for  (AwChannel *c : m_channels)
		c->clearData();

	// init defaults settings
	int pcaflag    = DEFAULT_PCAFLAG;
	int sphering   = DEFAULT_SPHEREFLAG;
	int posactflag = DEFAULT_POSACT;
	int biasflag   = DEFAULT_BIASFLAG;
	
	block      = DEFAULT_BLOCK(n);
	lrate      = DEFAULT_LRATE(m);
	annealdeg  = DEFAULT_ANNEALDEG;
	annealstep = 0.0;
	nochange   = DEFAULT_STOP;
	momentum   = DEFAULT_MOMENTUM;
	maxsteps   = DEFAULT_MAXSTEPS;

	auto args = m_process->pdi.input.settings;
	extended = args.value("infomax_extended").toBool() ? 1 : 0;
	//extended   = DEFAULT_EXTENDED;
	extblocks  = DEFAULT_EXTBLOCKS;
	pdfsize    = MAX_PDFSIZE;
	nsub       = DEFAULT_NSUB;

	double *eigv = NULL;
	double *EE = NULL;
	double *dataB= NULL;

	if (nc < m) // number of components reduced
		pcaflag = 1;
	if (extended) {
		annealstep = DEFAULT_EXTANNEAL;
		emit progressChanged("Extended mode active.");
	}
	else
		annealstep = DEFAULT_ANNEALSTEP;
	if (extended && extblocks > 0) {
		pdfsize = MIN(pdfsize, n);
		if (pdfsize < MIN_PDFSIZE)
			emit progressChanged("Warning: PDF values are inexact.");
	}

	QString  msg = QString("Input data size [%1, %2] = %3 channels, %4 samples").arg(m).arg(n).arg(m).arg(n);
	emit progressChanged(msg);
	if (pcaflag)
		emit progressChanged("After PCA dimension reduction, finding ");
	else
		emit progressChanged("Finding ");
	if (!extended) {
		msg = QString("%1 ICA components using logistic ICA").arg(nc);
		emit progressChanged(msg);
	}
	else {
		msg = QString("%1 ICA components using extended ICA").arg(nc);
		emit progressChanged(msg);

		if (extblocks > 0) {
			msg  = QString("PDF will be calculated initially every %1 blocks using %2 data points.").arg(extblocks).arg(pdfsize);
			emit progressChanged(msg);
		}
		else {
			msg  = QString("PDF will not be calculated. Exactly %1 sub-Gaussian components assumed.").arg(nsub);
			emit progressChanged(msg);
		}
	}
	msg = QString("Initial learning rate will be %1, block size %2").arg(lrate).arg(block);
	emit progressChanged(msg);

	if (momentum > 0.) {
		msg = QString("Momentum wil be %1").arg(momentum);
		emit progressChanged(msg);
	}
	msg = QString("Learning rate will be multiplied by %1 whenever angledelta >= %2 deg").arg(annealstep).arg(annealdeg);
	emit progressChanged(msg);
	msg = QString("Training will end when wchange < %1 or after %2 steps.").arg(nochange).arg(maxsteps);
	emit progressChanged(msg);

	if (biasflag)
		emit progressChanged("Online bias adjustment will be used.");
	else
		emit progressChanged("Online bias adjustment will not be used.");

	double *weights = new double[nc * m];
	zero(nc * m, weights);
	double *sphere = new double[m * m];
	double *bias = NULL;
	int *signs = NULL;
	if (biasflag) 
		bias = new double[nc];
	if (extended)
		signs = new int[nc];

	emit progressChanged("Removing mean of each channel...");
	rmmean(M, m, n);

	if (pcaflag) {
		msg = QString("Reducing the data dimensions to %1 principal dimensions...").arg(nc);
		emit progressChanged(msg);

		eigv = new double[m * m];
		EE = new double[m * m];
		pca(M, m, n, eigv);
		memcpy(EE, eigv, m * m * sizeof(double));

		dataB = new double[m * n];
		pcaproj(M, &eigv[m * (m - nc)], nc, n, m, dataB);
		delete[] M;
		M = dataB;
	}
	if (sphering == 1) {
		emit progressChanged("Computing the sphering matrix...");
		do_sphere(M, nc, n, sphere);
		emit progressChanged("Sphering the data...");
		dataB = new double[nc * n];
		syproj(M, sphere, nc, n, dataB);
		delete[] M;
		M = dataB;
	}
	else if (sphering == 0) {
		emit progressChanged("Using the sphering matrix as the starting weight matrix...");
		do_sphere(M, nc, n, weights);

		emit progressChanged("Returning the identity matrix in variable 'sphere'...");
		eye(nc, sphere);
	}
	else if (sphering == -2) {
		emit progressChanged("Returning the identity matrix in variable 'sphere'...");
		eye(nc, sphere);
	}

	runica(M, weights, nc, n, bias, signs);
	if (m_process->isAborted()) {
		emit progressChanged("Process aborted.");
		if (bias) delete[] bias;
		if (signs) delete[] signs;
		if (eigv) delete[] eigv;
		if (EE) delete[] EE;	
		delete[] M;
		return;
	}

	/*************** Orient components toward positive activations ****************/
	dataB = new double[nc * n];
	if (posactflag) 
		posact(M, weights, nc, n, dataB);
	else
		geproj(M, weights, nc, n, dataB);

	delete[] M;
	M = dataB;

	if (pcaflag) {
		msg = QString("Composing the eigenvector, weights, and sphere matrices into a single rectangular weights matrix; sphere = eye(%1)").arg(m);
		emit progressChanged(msg);
	}
	emit progressChanged("Sorting components in descending order of mean projected variance...");

	if (eigv) {
		varsort(M, weights, sphere, &eigv[m * (m - nc)], bias, signs, nc, n, m);
		eye(m, sphere);
	}
	else
		varsort(M, weights, sphere, NULL, bias, signs, nc, n, m);

	delete[] M;

	// Armadillo matrices are COLUMN MAJOR !!!!
	arma::mat mweigths(weights, nc, m, false);
	arma::mat msphere(sphere, m, m, false);

	m_unmixing = mweigths * msphere;
	
	if (nc < m) 
		m_mixing = pinv(m_unmixing);
	else
		m_mixing = inv(m_unmixing);

	if (bias) delete[] bias;
	if (signs) delete[] signs;
	if (eigv) delete[] eigv;
	if (EE) delete[] EE;	
	delete[] weights;
	delete[] sphere;
}