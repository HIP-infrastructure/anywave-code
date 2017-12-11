#include "ica.h"
#include "pca.h"
#include <math/AwMath.h>

void ICA::JADE(int nc)
{
	mat X = AwMath::channelsToMat(m_channels);
	if (nc < X.n_rows)
		pca(X, nc, 1e6, X);
	uword n = X.n_rows;
	uword T = X.n_cols;
	uword m = nc;

	sendMessage("Removing the mean values...");
	X = X - mean(X.t()).t() * ones(1, T);
	sendMessage("Done.");

	sendMessage("Whitening the data...");
	mat U;
	vec D;
	mat V;
	svd(U, D, V, (X * X.t()) / T);
	vec puiss = sort(D);
	uvec k = sort_index(D);
	uvec rangeW = regspace<uvec>(n - m, n - 1); 
	uword idx = 0;
	vec scales = sqrt(puiss.cols(rangeW));
	
	//mat UU(n, k.n_elem);
	//idx = 0;
	//for (uword cols = 0; cols < k.n_elem; cols++)
	//	UU.col(idx++) = U.col(cols);
		
	mat W = diagmat(1. / scales) * U.cols(k.cols(rangeW)).t();
	mat iW = U.cols(k.cols(rangeW)) * diagmat(scales);
	X = W * X;
	sendMessage("Done.");
	sendMessage("Estimating cumulant matrices...");
	uword dimsymm = (m * (m + 1)) / 2;
	uword nbcm = dimsymm;
	mat CM = zeros(m, m * nbcm);
	mat R = eye(m, m);
	mat Qij = zeros(m, m);
	mat Xim = zeros(1, m);
	mat Xjm = zeros(1, m);
	mat scale = ones(m, 1) / T;
	uvec Range = regspace<uvec>(0,m - 1);
	for (uword im = 0; im < m; im++) {
		Xim = X.row(im);
		Qij = ((scale * (Xim % Xim)) % X) % X.t() - R - 2 * R.col(im) * R.col(im).t();
		CM.cols(Range) = Qij;
		Range = Range + m;
		for (uword jm = 0; jm < im - 1; jm++) {
			Xjm = X.row(jm);
			Qij = ((scale * (Xim % Xjm)) % X) % X.t() - R.col(im) * R.col(jm).t() - R.col(jm) * R.col(im).t();
			CM.cols(Range) = sqrt(2) * Qij;
			Range = Range + m;
		}
	}

	V = eye<mat>(m, m);
	double seuil = 1 / sqrt(T) / 100;
	bool encore = true;
	int sweep = 0;
	int updates = 0;
	mat g = zeros(2, nbcm);
	mat gg = zeros(2, 2);
	mat G = zeros(2, 2);
	double c = 0., s = 0., ton = 0., toff = 0., theta = 0.;

	while (encore) {
		encore = false;
		sweep++;
		for (uword p = 0; p < m - 1; p++) {
			for (uword q = p + 1; q < m; q++) {
				uvec Ip = regspace<uvec>(p, m, m*nbcm);
				uvec Iq = regspace<uvec>(q, m, m*nbcm);
				rowvec rp = CM.row(p);
				rowvec rq = CM.row(q);
				g.row(0) = rp.cols(Ip) - rq.cols(Iq);
				g.row(1) = rp.cols(Iq) - rq.cols(Ip);
				gg = g * g.t();
				ton = gg(0, 0) - gg(1, 1);
				toff = gg(0, 1) + gg(1, 0);
				theta = 0.5 * atan2(toff, ton + sqrt(ton * ton + toff * toff));
				if (abs(theta) > seuil) {
					encore = true;
					updates++;
					double c = cos(theta);
					double s = sin(theta);
					G(0, 0) = c;
					G(0, 1) = -s;
					G(1, 0) = s;
					G(1, 1) = c;
					uvec pair(2);
					pair(0) = p;
					pair(1) = q;
					V.cols(pair) = V.cols(pair) * G;
					CM.rows(pair) = G.t() * CM.rows(pair);
					uvec IpIq = join_horiz(Ip, Iq);
					CM.cols(IpIq) = join_horiz((c * CM.cols(Ip) + s * CM.cols(Iq)), (-s * CM.cols(Ip) + c * CM.cols(Iq)));
				}
			}
		}
	}
	sendMessage(QString("Total of %d given rotations.").arg(updates));
	mat B = V.t() * W;
	sendMessage("Sorting the components...");
	mat A = iW * V;
	uvec keys = sort_index(sum(A % A));
	B = B.rows(keys);
	B = B.rows(regspace<uvec>(m - 1, -1, 0));
	sendMessage("Fixing the signs.");
	colvec b = B.col(0);
	vec signs= sign(sign(b) + 0.1);
	B = diagmat(signs) * B;
	m_mixing = B;
	m_unmixing = pinv(B);
 } 