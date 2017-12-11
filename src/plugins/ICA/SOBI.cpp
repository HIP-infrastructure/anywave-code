#include "ica.h"
#include "pca.h"
#include <math/AwMath.h>


void ICA::SOBI(int nc)
{
//	mat X = AwMath::channelsToMat(m_channels);
//	if (nc < X.n_rows)
//		pca(X, nc, 1e6);
//
//	uword m = X.n_rows;
//	uword N = X.n_cols;
//	uword n = nc;
//	uword p = std::min(uword(100), N / 3);
//	uword ntrials = 1;
//	// Make the data zero mean
//	// X = X - kron(mean(X')', ones(1, N);
//	X = X - kron(mean(X.t()).t(), ones(1, N));
//
//	// Prewhiten using svd
//	mat UU;
//	vec s;
//	mat VV;
//	svd_econ(UU, s, VV, X.t());
//	mat Q = pinv(s) * VV.t();
//	X = Q * X;
//	// estimate the correlations matrices
//	int k = 1;
//	uword pm = p * m;
//	cx_mat M = zeros(m,pm), Rxp;
//	for (uword u = 0; u < pm; u += m) {
//		k++;
//		// only 1 trial here
//		Rxp = X(span::all, span(k - 1, N - 1)) * X(span::all, span(0, N - k +2)).t();
//		Rxp /= (N - k + 2);
//		//M(span::all, span(u, u + m - 1)) = norm(Rxp, "fro") * Rxp;
//		for (uword col = u; col < u+m-1; col++)
//			M.col(col) = norm(Rxp, "fro") * Rxp;
//	}
//
//	// perform joint diagonalization
//	double epsilon = 1 / sqrt(N) / 100.;
//	bool encore = true;
//	cx_mat V = eye((int)m, (int)m);
//	cx_mat g(m, p);
//	cx_double i(0., 1.);
////	int step_n = 0;
//	while (encore) {
//		encore = false;
//		for (uword p = 0; p < m - 1; p++) {
//			for (uword q = p + 1; q < m; q++) {
//				for (uword ii = 0; ii < pm; ii += m) {
//					g(0, ii) = M(p, p + ii) - M(q, q + ii);
//					g(1, ii) = M(p, q + ii) + M(q, p + ii);
//					g(2, ii) = i * (M(q, p + ii) - M(p, q + ii));
//				}
//				vec eigval;
//				mat eigvec;
//				eig_sym(eigval, eigvec, real(g * g.t()));
//				uvec K = sort_index(eigval);
//				vec angles = eigvec.col(K(2));
//				vec tmp = sign(angles);
//				angles = tmp(0) * angles;
//				double c = sqrt(0.5 + angles(0) / 2);
//				cx_double sr = 0.5 * (angles(1) - i * angles(2)) / c;
//				cx_double sc = conj(sr);
//				bool oui = abs(sr) > epsilon;
//				encore |= oui;
//				if (oui) { // calculate the M and V matrices
//					for (uword ii = 0; ii < pm; ii += m) {
//						cx_vec colp = M.col(p + ii);
//						cx_vec colq = M.col(q + ii);
//						M.col(p + ii) = c * colp + sr * colq;
//						M.col(q + ii) = c * colq - sc * colp;
//						cx_vec rowp = M.row(p);
//						cx_vec rowq = M.row(q);
//						M.row(p) = c * rowp + sc * rowq;
//						M.row(q) = c * rowq - sr * rowp;
//						cx_vec temp = V.col(p);
//						V.col(p) = c * V.col(p) + sr * V.col(q);
//						V.col(q) = c * V.col(q) - sc * temp;
//					}
//				} // end if oui
//			} // end for q
//		} // end for p
//	//	step_n++;
//	} // end while
//	cx_mat H = pinv(Q) * V;

}