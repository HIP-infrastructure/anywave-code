#include "ica.h"
#include "pca.h"
#include <math/AwMath.h>

void ICA::bsscca(int nc)
{
	mat X = AwMath::channelsToMat(m_channels);
	uword d = X.n_rows;
	uword T = X.n_cols;
	uword delay = 1;

	mat Wpca;
	if (nc < X.n_rows)
		Wpca = pca(X, nc, 1e6);
	else
		Wpca = eye(X.n_rows, X.n_rows);



	mat Y = X(span::all, span(delay, T - 1));
	X = X(span::all, span(0, T - delay - 1));
	mat Cyy = Y * Y.t();
	Cyy /= T;
	mat Cxx =  X * X.t();
	Cxx /= T;
	mat Cxy =  X * Y.t();
	Cxy /= T;
	mat Cyx = Cxy.t();
	mat invCyy = pinv(Cyy);
	// calculate W

	cx_mat W;
	cx_vec r;
	eig_gen(r, W, pinv(Cxx) * Cxy * invCyy * Cyx);
	vec sqr = abs(real(r));
	sqr.for_each([](vec::elem_type& val) { val = sqrt(val); } );
	mat sorted = sort(sqr, "descend");
	uvec I = sort_index(sqr, "descend");
	mat WW = real(W);
	m_unmixing = mat(WW.n_rows, I.n_elem);
	for (uword i = 0; i < I.n_elem; i++)
		m_unmixing.col(i) = WW.col(I.at(i));
	m_unmixing = m_unmixing.t();
	m_mixing = inv(m_unmixing);
}

