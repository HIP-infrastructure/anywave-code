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
#ifndef INFOMAX_ALGO_H
#define INFOMAX_ALGO_H


namespace InfoMaxAlgo {
	typedef struct {
		int    idx;
		double val;
	} idxelm;

	//global variables
	int extended, extblocks, pdfsize, nsub;
	int block, maxsteps;
	double lrate, annealstep, annealdeg, nochange, momentum;

	int compar(const void *a, const void *b);
	void zero(int, double *);
	void eye(int, double *);
	void rmmean(double *, int, int);
	void syproj(double*, double*, int, int, double*);
	void geproj(double*, double*, int, int, double*);
	void pcaproj(double*, double*, int, int, int, double*);
	void pca(double*, int, int, double*);
	void posact(double*, double*, int, int, double*);
	void initperm(int *, int);
	void randperm(double *, double *, double *, int *, int , int, int, double *);
	void probperm(double *data, double *trsf, double *bias, int m, int n, int frames, int epochs, double *proj);
	void pdf(double *data, double *trsf, int *perm, int m, int n, int k, double *kk);
	void do_sphere(double *data, int m, int n, double *sphe);
	void varsort(double *data, double *weights, double *sphere, double *eigv, double *bias, int *signs, int m, int n, int k); 
	double *inverse(double *data, double *eigv, int m, int n);
}
#endif