#include "ica.h"
#include <aw_armadillo.h>
int ICA::run_acsobiro(int n)
{
	// build data matrix COLUMN MAJOR
	mat X(m_channels.size(), m_channels.first()->dataSize());
	for (auto r = 0; r < X.n_rows; r++)
		for (auto c = 0; c < X.n_cols; c++)
			X(r,c) = (double)m_channels.at(r)->data()[c];

	auto m = X.n_rows; 
	auto N = X.n_cols;
	uword DEFAULT_LAGS = 100;
	auto p = std::min(DEFAULT_LAGS, (uword)std::ceil(N / 3));

	// remove data means
	X = X - (mean(X.t()).t() * ones(1, N));
	// estimate the sample covariance matrix for the time delay=1, to reduce influence of white noise
	mat Rxx = (X.cols(0, N-2) * X.cols(1, N - 1).t()) / (N - 1);

	mat Ux, Vx;
	vec Sx;
	svd(Ux, Sx, Vx, X);
	mat Dx = diagmat(Sx);
	mat Q;
	if (n < m) {
		Dx = Dx - mean(Dx.cols(n, m - 1));
		Q = diagmat(sqrt(1 / Dx.cols(0, n - 1))) * Ux.cols(0, n - 1).t();
	}
	else {
		// use all sources
		Q = diagmat(sqrt(1 / Dx.cols(0, m - 1))) * Ux.cols(0, m - 1).t();
	}
	Rxx.clear();
	// prewithened data
	mat Xb = Q * X;
	auto k = 0;
	auto pn = p * n;
	mat Rxp;
	for (auto u = 0; u < m; u += pn) {
		k++;
		Rxp = Xb.cols(k - 1, N - 1) * Xb.cols(0, N - k).t() / (N - k);
		norm(Rxp, "fro");
	}
	return 0;
}