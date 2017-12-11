#include "pca.h"

int pca(mat& X, int nbc, double eigratio, mat& output)
{
	double er = eigratio;
	if (eigratio <= 0.)
		er = 1e8;
	if (nbc >= X.n_rows) {
		output = X;
		return -1;
	}
	mat C = cov(X.t());
	mat u, v;
	vec eigval;
	mat eigvec;
	eig_sym(eigval, eigvec, C);
	vec abs_val = abs(eigval);
	vec val = sort(abs_val, "descend");
	uvec I = sort_index(abs_val, "descend");

	int n = nbc;
	while (val(0) / val(n - 1) > er)
		n--;

	uvec II(n);
	for (uword i = 0; i < n; i++)
		II(i) = I(i);
	mat V = eigvec.cols(II);
	vec tmp(n);
	for (uword i = 0; i < n; i++)
		tmp(i) = eigval(i);
	mat D = diagmat(tmp);
	mat W = sqrt(inv(D)) * V.t();
	output = W * X;
	return 0;
}

int pca_fsvd(mat& A, mat& X, int k, int i, bool usePowerMethod)
{
	bool isTransposed = false;
	// Take (conjugate) transpose if necessary. It makes H smaller thus leading the computations to be faster.
	if (size(X, 1) < size(X, 2)) {
		X = X.t();
		isTransposed = true;
	}
	uword n = size(X, 2);
	int l = k + 2;

	// Form a real nxl matric G whose entries are iid Gaussian r.v.s of zero mean and unit variance
	mat H;
	mat G = randn(n, l);
	if (usePowerMethod) {
		// Use only the given exponent
		H = X * G;
		for (uword j = 1; j < i + 1; j++)
			H = X * (X.t() * H);
	}
	else {
		// Compute the mxl matrices H^{(0)}, ..., H{(i)}
		// Note that this is done implicitly in each iteration below.
		field<mat> localH(i + 1);
		localH(0) = X * G;
		for (uword j = 1; j < i + 1; j++)
			localH(j) = X * (X.t() * localH(j - 1));
		H = join_rows(localH(0), localH(1));
		for (uword j = 2; j < i + 1; j++)
			H = join_rows(H, localH(j));
	}

	// Using the pivoted QR-decomposition, from a real mx((i+1)l) matrix Q
	// whose columns are orthonormal, s.t. there exists a real 
	// ((i+1)l) x ((i+1)l) matrix R for which H = QR.
	// XXX: Buradaki column pivoting ile yapilmayan hali.
	mat Q, R;
	qr(Q, R, H);
	// Compute the nx((i+1)l) product matrix T = A^T Q
	mat T = X.t() * Q;

	// Form an SVD of T
	mat Vt;
	vec s;
	mat W;
	svd_econ(Vt, s, W, T);
	mat St = diagmat(s);

	// compute the mx((i+1)l) product matrix
	mat Ut = Q * W;

	// Retreive the lefmost mxk block U of Ut, the leftmost nxk block V of Vt,
	// and the lefmost uppermost kxk block S of St. The product U S V^T then approximates A.
	mat V, U;
	if (isTransposed) {
		V = Ut.cols(span(0, k - 1));
		U = Vt.cols(span(0, k - 1));
	}
	else {
		U = Ut.cols(span(0, k - 1));
		V = Vt.cols(span(0, k - 1));
	}
	mat S = St.submat(0, 0, k - 1, k - 1);
	A = U * S * V.t();
	return 0;
}