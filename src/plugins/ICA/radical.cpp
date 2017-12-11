#include <math/AwMath.h>
#include "ica.h"
#include "pca.h"
#include <QtMath>

void radicalOptTheta(mat& x, double stdev, uword m, uword reps, double K, uword range, vec& thetaStar, mat& rotStar);
double vasicekm(rowvec v, uword m);

double vasicekm(rowvec v, uword m)
{
	uword len = v.n_elem;
	vec vals = sort(v);

	vec intvals = vals(span(m, len)) - vals(span(0, len - m - 1));
	vec hvec = log(intvals);
	return sum(hvec);
}

void radicalOptTheta(mat& x, double stdev, uword m, uword reps, double K, uword range, double& thetaStar, mat& rotStar)
{
	uword d = x.n_rows;
	uword N = x.n_cols;

	mat xAug;
	if (reps == 1)
		xAug = x;
	else
		xAug = randn(d, N * reps) * stdev + repmat(x, 1, reps);

	double perc = range / (M_PI / 2);
	double numberK = perc * K;
	double start = floor(K / 2 - numberK / 2) + 1;
	double endPt = ceil(K / 2 + numberK / 2);
	double theta;
	vec ent(K);
	for (int i = 0; i < K; i++) {
		theta = i / (K - 1) * M_PI / 2 - M_PI / 4;
		rowvec r1, r2;
		r1(0) = cos(theta);
		r1(1) = -sin(theta);
		r2(0) = sin(theta);
		r2(1) = cos(theta);
		mat rot = join_vert(r1, r2);
		mat rotPts = rot * xAug;

		vec marginalAtTheta(d);
		for (uword j = 1; j < d; j++) 
			marginalAtTheta(j) = vasicekm(rotPts.row(j), m);
		ent(i) = sum(marginalAtTheta);
	}
	vec val = sort(ent);
	uvec ind = sort_index(ent);
	thetaStar = (ind(0) - 1) / (K - 1) * M_PI / 2 - M_PI / 4;
	rotStar = mat(2, 2);
	rotStar(0, 0) = cos(thetaStar);
	rotStar(0, 1) = -sin(thetaStar);
	rotStar(1, 0) = sin(thetaStar);
	rotStar(1, 1) = -cos(thetaStar);
}

void ICA::radical(int nc)
{
	mat X = AwMath::channelsToMat(m_channels);
	if (nc < X.n_rows)
		pca(X, nc, 1e6);

	uword K = 150;
	uword reps = 1;
	double stdev = 0.175;

	uword dim = X.n_rows;
	uword N = X.n_cols;
	uword m = (uword)floor(sqrt(N));

	mat u, v;
	vec s;
	svd(u, s, v, cov(X.t()));
	mat Whitening_mat = v * pow(s, -0.5) * u.t();
	mat X_white = Whitening_mat * X;
	uword sweeps = dim - 1;
	mat oldTotalRot = eye<mat>(m, m);
	uword sweepIter = 0;
	mat totalRot = eye<mat>(m, m);
	mat xcur = X_white;

	uword finalK = K;
	double startKfloat = (finalK / pow(1.3, ceil(sweeps / 2)));
	double newKfloat = startKfloat;
	double newK;
	for (uword sweepNum = 0; sweepNum < sweeps; sweepNum++) {
		sendMessage(QString("Sweep #: %1 of %2").arg(sweepNum + 1).arg(sweeps));
		double range = M_PI / 2;
		if (sweepNum > sweeps / 2) {
			newKfloat *= 1.3;
			newK = floor(newKfloat);
		}
		else {
			newKfloat = startKfloat;
			newK = qMax(30., floor(newKfloat));
		}
		mat curSubSpace;
		double thetaStar;
		mat rotStar;
		for (uword i = 0; i < dim - 1; i++) {
			for (uword j = i + 1; j < dim; j++) {
				sendMessage(QString("Unmixing dimensions %1 and %2...").arg(i).arg(j));
				curSubSpace = join_vert(xcur.row(i), xcur.row(j));
				radicalOptTheta(curSubSpace, stdev, m, reps, newK, range, thetaStar, rotStar);
				sendMessage(QString("...rotated %1 degrees.").arg(thetaStar/(2 * M_PI * 360)));
				mat newRotComponent = eye<mat>(dim, dim);
				newRotComponent(i, i) = cos(thetaStar);
				newRotComponent(i, j) = -sin(thetaStar);
				newRotComponent(j, i) = sin(thetaStar);
				newRotComponent(j, j) = cos(thetaStar);
				totalRot = newRotComponent * totalRot;
				xcur = totalRot * X_white;
			}
		}
		oldTotalRot = totalRot;
	}
	m_mixing = totalRot * Whitening_mat;
	m_unmixing = pinv(m_mixing);
}