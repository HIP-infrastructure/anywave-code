#include "ica.h"

#define MAX_ITERATIONS	1000
#define TOLERANCE		0.0001
#define ALPHA			1

// remove mean from columns
vec mat_center(mat M)
{
	vec means(M.n_cols);
	means.fill(0.);

	int i, j;
	for (i = 0; i < M.n_rows; i++) 
		for (j = 0; j < M.n_cols; j++)
			means[j] += M[i,j];
	for (i = 0; i < M.n_cols; i++)
		means[i] /= M.n_rows;

	return means;
}

/**
 * De-center mat M. (Adds the mean to every column)
 */
void mat_decenter(mat M, vec means)
{
	int i,j;

	for(i=0; i<M.n_rows; i++)
		for(j=0; j<M.n_cols; j++)
			M[i,j] += means[j]; 
}

vec mat_mean_rows(mat M)
{
	//double sum;
	//for (int i = 0; i < M.n_rows; i++) {
	//	sum = 0;
	//	for (int j = 0; j < M.n_cols; j++)
	//		sum += M[i,j];
	//	v[i] = sum / M.n_cols;
	//}

	vec v = sum(M, 1);
	v /= M.n_cols;
	return v;
}

/**
 * ICA function. Computes the W matrix from the
 * preprocessed data.
 */
mat ICA_compute(mat& X, int nc)
{
	mat TXp, GWX, W, Wd, W1, D, TU, TMP;
	vec d, lim;
	int i, it;

	// matrix creation
	TXp.set_size(X.n_cols, nc);
	GWX.set_size(nc, X.n_cols);
	//W.set_size(nc, nc);
	Wd.set_size(nc, nc);
	D.set_size(nc, nc);
	TMP.set_size(nc, nc);
	TU.set_size(nc, nc);
	W1.set_size(nc, nc);
	d.set_size(nc);

	// W rand init
	W = randu<mat>(nc, nc);
	
	// sW <- La.svd(W)
	Wd = W;
	svd(Wd, d, W, Wd);

	// W <- sW$u %*% diag(1/sW$d) %*% t(sW$u) %*% W
	//	vect_apply_fx(d, rows, fx_inv, 0);
	//	mat_diag(d, rows, D);
	TU = Wd.t();
	for (int i = 0; i < d.size(); i++)
		d[i] = 1 / d[i];
	
	D = diagmat(d);

//	mat_mult(Wd, rows, rows, D, rows, rows, TMP);
	TMP = Wd * D;
//	mat_mult(TMP, rows, rows, TU, rows, rows, D);
//	D = TMP * TU;
//	mat_mult(D, rows, rows, W, rows, rows, Wd); // W = Wd
//	Wd = D * W;

	Wd = TMP * TU * W;

	// W1 <- W 
	//mat_copy(Wd, rows, rows, W1);
	W1 = Wd;

	// lim <- rep(1000, maxit); it = 1
//	lim = vect_create(MAX_ITERATIONS);
	lim.set_size(MAX_ITERATIONS);
	lim.fill(1000);
	//for (i=0; i<MAX_ITERATIONS; i++)
	//	lim[i] = 1000;
	it = 1;


	// t(X)/p
//	mat_transpose(X, rows, cols, TXp);
	TXp = X.t();
//	mat_apply_fx(TXp, cols, rows, fx_div_c, cols);
	TXp = X.n_cols * TXp;

	while (lim[it] > TOLERANCE && it < MAX_ITERATIONS) {
		// wx <- W %*% X
//		mat_mult(Wd, rows, rows, X, rows, cols, GWX);
		GWX = Wd * X;
		// gwx <- tanh(alpha * wx)
//		mat_apply_fx(GWX, rows, cols, fx_tanh, 0);
		for (int i = 0; i < GWX.n_rows; i++)
			for (int j = 0; j < GWX.n_cols; j++)
				GWX[i,j] = tanh(ALPHA * GWX[i,j]);

		// v1 <- gwx %*% t(X)/p
		//mat_mult(GWX, rows, cols, TXp, cols, rows, TMP); // V1 = TMP
		TMP = GWX * TXp;
		
		// g.wx <- alpha * (1 - (gwx)^2)
		//mat_apply_fx(GWX, rows, cols, fx_1sub_sqr, 0);
		for (int i = 0; i < GWX.n_rows; i++)
			for (int j = 0; j < GWX.n_cols; j++)
				GWX[i,j] = ALPHA - (1 - GWX[i,j] * GWX[i,j]);

		// v2 <- diag(apply(g.wx, 1, FUN = mean)) %*% W
		//mat_mean_rows(GWX, rows, cols, d);
		d = mat_mean_rows(GWX);

		//mat_diag(d, rows, D);
		D = diagmat(d);
		//mat_mult(D, rows, rows, Wd, rows, rows, TU); // V2 = TU
		TU = D * Wd;

		// W1 <- v1 - v2
		//mat_sub(TMP, TU, rows, rows, W1);
		W1 = TMP - TU;
		
		// sW1 <- La.svd(W1)
		//mat_copy(W1, rows, rows, W);
		W = W1;
		//svdcmp(W, rows, rows, d, D);
		svd(W, d, D, W1);

		// W1 <- sW1$u %*% diag(1/sW1$d) %*% t(sW1$u) %*% W1
		//mat_transpose(W, rows, rows, TU);
		TU = W.t();
		//vect_apply_fx(d, rows, fx_inv, 0);
		for (int i = 0; i < d.size(); i++)
			d[i] = 1 / d[i];

		//mat_diag(d, rows, D);
		D = diagmat(d);
		//mat_mult(W, rows, rows, D, rows, rows, TMP);
		TMP = W * D;
		//mat_mult(TMP, rows, rows, TU, rows, rows, D);
		D = TMP * TU;
		//mat_mult(D, rows, rows, W1, rows, rows, W); // W1 = W
		W = D * W1;
		
		// lim[it + 1] <- max(Mod(Mod(diag(W1 %*% t(W))) - 1))
		//mat_transpose(Wd, rows, rows, TU);
		TU = Wd.t();
		//mat_mult(W, rows, rows, TU, rows, rows, TMP);
		TMP = W * TU;
		//lim[it+1] = fabs(mat_max_diag(TMP, rows, rows) - 1);
		lim[it+1] = fabs(max(diagvec(TMP)) - 1);

		// W <- W1
		//mat_copy(W, rows, rows, Wd);
		Wd = W;

		it++;
	}

	// clean up
	//mat_delete(TXp, cols, rows);
	//mat_delete(GWX, rows, cols);
	//mat_delete(W, rows, rows);
	//mat_delete(D, rows, rows);
	//mat_delete(TMP, rows, rows);
	//mat_delete(TU, rows, rows);
	//mat_delete(W1, rows, rows);
	//vect_delete(d);	

	return Wd;
}


//void ICA::fastica(int m, int n, int nc)
//{
//	emit progressChanged("Building data matrix...");
//	mat X(m, n);
//	for (int i = 0; i < m; i++) {
//		float *data_row = m_channels.at(i)->data();
//		for (int j = 0; j < n; j++)
//			X[i,j] = data_row[j];		
//	}
//	emit progressChanged("Computing ICA...");
//	mat XT, V, TU, D, X1, _A;
//	vec scale, d;
//
//	/*
//	 * CENTERING
//	 */
//	emit progressChanged("Centering data...");
//	scale = mat_center(X);
//	/*
//	 * WHITENING
//	 */
//	emit progressChanged("Whitening...");
//	// X <- t(X); V <- X %*% t(X)/rows 
//	XT = X.t();
//	X = X / X.n_rows;
//	V = XT * X;
//
//	// La.svd(V)
//	svd(V,d,D,V);
//
//	// D <- diag(c(1/sqrt(d))
//	for (int i = 0; i < d.size(); i++)
//		d[i] = 1 / sqrt(d[i]);
//	D = diagmat(d);
//
//	// K <- D %*% t(U)
////	TU = V.t();
////	V = D * TU;
////	V = D * V.t();
//	// X1 <- K %*% X
//	X1 = V * D * V.t();
////	X1 = V * XT;
//	
//	/*
//	 * FAST ICA
//	 */
//	_A = ICA_compute(X1, nc);
//
//	/*
//	 * OUTPUT
//	 */
//
//	// X <- t(x)
//	X = XT.t();
//	mat_decenter(X, scale);
//
////	// K
////	arma::mat K = V.t();
//	
//	// w <- a %*% K; S <- w %*% X
//	D = _A * V;
//	X1 = D * XT;
//
////	// S
////	arma::mat S = X1.t();
//
//	// A <- t(w) %*% solve(w * t(w))
//	TU = D.t();
//	V = D * TU;
//	D = pinv(V);
//	V = TU * D;
//
//	// A mixing
//	mat A = V.t();
//
//	// W unmixing
//	mat W = _A.t();
//
//	emit progressChanged("ICA done.");
////	m_unmixing = conv_to<fmat>::from(W);
////	m_mixing = conv_to<fmat>::from(A);
//	m_unmixing = W;
//	m_mixing = A;
//	saveToFile();	
//}