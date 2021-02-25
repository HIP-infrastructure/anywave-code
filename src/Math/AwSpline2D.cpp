/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <math/AwSpline2D.h>
#include <math.h>

#ifdef MKL
#include <mkl.h>
#define dgesv_ dgesv
#else
	extern "C" void dgesv_(int *n, int *nrhs, double *A, int *lda, int *ipiv, double *B, int *ldb, int *info);
#endif

int AwSpline2D::interpolate(int np, int nz, int ni, double *x, double *y, double *z, double *xi, double *yi, double *zi)
{
	int i, j, k;
	double d, dx, dy;

	double *dist = new double[np*np];   // returned array for distances
	double *w = new double[np*nz];		// returned array for weights

	// dgesv used by find_weights overwrites input z
	for (i=0; i<(np*nz); i++) w[i] = z[i];

	if (find_weights(np, nz, x, y, w, dist) != 0)	{
		delete[] dist;
		delete[] w;
		return -1;
	}
	
#pragma omp parallel for private (i, j, k, dx, dy, d)
	for (i=0; i<ni; i++) {
		for (k=0; k<nz; k++)
			zi[i*nz + k] = 0.0;
		for (j=0; j<np; j++) {
			dx = x[j] - xi[i];
			dy = y[j] - yi[i];
			d = sqrt(dx*dx + dy*dy);
			if (d!=0.0)
				for (k=0; k<nz; k++)
					zi[i*nz + k] += d * d * (log(d) - 1.0) * w[j*nz + k];
		}
	}

	delete[] dist;
	delete[] w;
	return 0;
}


int AwSpline2D::solve_coef_sys(int np, int nz, double *g, double *z)
{
	int i, j, info=0, *ipiv;
	double *zT;

	ipiv = new int[np];
	zT = new double[np*nz];

	for (i=0; i<np; i++)
		for (j=0; j<nz; j++)
			zT[i + np*j] = z[i*nz + j];

	dgesv_(&np, &nz, g, &np, ipiv, zT, &np, &info);

	/*
	if (info!=0)
		fprintf(stderr, "%s:%d dgesv_ returned info=%d on exit\n", __FILE__, __LINE__, info);
	*/

	delete[] ipiv;

	for (i=0; i<np; i++)
		for (j=0; j<nz; j++)
			z[i*nz + j] = zT[i + np*j];

	delete[] zT;

	return info;
}


int AwSpline2D::find_weights(int np, int nz, double *x, double *y, double *z, double *dist)
{
	int i, j, sol;
	double dij, dx, dy;

	for (i=0; i<np; i++)
		for (j=0; j<np; j++)
			if (i>j) {
				dx = x[i] - x[j];
				dy = y[i] - y[j];
				dij = sqrt(dx*dx + dy*dy);
				dist[j*np+i] = dist[i*np+j] = dij * dij * (log(dij) - 1.0);
			}
			else if (i==j)
				dist[j*np+j] = 0.0;

	sol = solve_coef_sys(np, nz, dist, z);

	if (sol!=0)
		return 1;

	return 0;
}