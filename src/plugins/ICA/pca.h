#pragma once
#include <armadillo>

using namespace arma;
int pca(mat& X, int nbc, double eigratio, mat& output);

/* PCA using fast svd: PCA, X = data, k = number of dimensions, i = power. Returns the projected components. */
int pca_fsvd(mat& A, mat& X, int k, int i = 1, bool usePowerMethod = false);