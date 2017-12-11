#include "ica.h"

#define 	FICA_APPROACH_DEFL   2
#define 	FICA_APPROACH_SYMM   1
#define 	FICA_NONLIN_POW3   10
#define 	FICA_NONLIN_TANH   20
#define 	FICA_NONLIN_GAUSS   30
#define 	FICA_NONLIN_SKEW   40
#define 	FICA_INIT_RAND   0
#define 	FICA_INIT_GUESS   1
#define 	FICA_TOL   1e-9

inline int mod(int k, int n)
{
	return (n == 0) ? k : (k - n * (int)floor(static_cast<double>(k) / n));
}

int pcamat(mat& vectors, int numOfIc, int firstEig, int lastEig, mat& Es, vec& Ds);
void selcol(const mat& oldMatrix, const vec& maskVector, mat& newMatrix);
void remmean(mat& inVectors, mat& outVectors, vec& meanValue);
void whitenv(const mat& vectors, const mat& E, const mat& D, mat& newVectors, mat& whiteningMatrix, mat& dewhiteningMatrix);
mat orth(const mat& A);
bool fpica(const mat& X, const mat& whiteningMatrix, const mat& dewhiteningMatrix, const int approach, const int numOfIc,
	const int g, const int finetune, const double a1, const double a2, double myy,
	const int stabilization, const double epsilon, const int maxNumIterations, const int maxFineTune, const int initState,
	mat& guess, double sampleSize, mat& A, mat& W);
mat mpower(const mat& A, const double y);
vec sumcol(const mat& A);
uvec getSamples(const int max, const double percentage);

uvec getSamples(const int max, const double percentage)
{
	vec rd = randu(max);
	QVector<int> sV;
	int sZ = 0;

	for (int i = 0; i < max; i++)
		if (rd(i) < percentage)
			sV.append(sZ++);

	uvec out(sV.size());
	for (int i = 0; i < sV.size(); i++)
		out(i) = (uword)sV.at(i);

	return out;
}

vec sumcol(const mat& A)
{
	vec out = zeros(A.n_cols);
	for (arma::uword i = 0; i < A.n_cols; i++)
		out(i) = sum(A.col(i));
	return out;
}

mat mpower(const mat& A, const double y)
{
	mat T = zeros(A.n_rows, A.n_cols);
	mat dd = zeros(A.n_rows, A.n_cols);
	vec d = zeros(A.n_rows);
	vec dOut = zeros(A.n_rows);
	
	eig_sym(d, T, A);
	dOut = pow(d, y);
	dd = diagmat(dOut);
	for (arma::uword i = 0; i < T.n_cols; i++)
		T.col(i) = T.col(i) / norm(T.col(i));

	return T * dd * T.t();
}

bool fpica(const mat& X, const mat& whiteningMatrix, const mat& dewhiteningMatrix, const int approach, const int numOfIc,
	const int g, const int finetune, const double a1, const double a2, double myy,
	const int stabilization, const double epsilon, const int maxNumIterations, const int maxFineTune, const int initState,
	mat& guess, double sampleSize, mat& A, mat& W) 
{
	arma::uword vectorSize = X.n_rows;
	arma::uword numSamples = X.n_cols;
	int gOrig = g;
	int gFine = finetune + 1;
	double myyOrig = myy;
	double myyK = 0.01;
	int failureLimit = 5;
	int usedNlinearity = 0;
	double stroke = 0.;
	int notFine = 1;
	int loong = 0;
	int initialStateMode = initState;
	double minAbsCos = 0., minAbsCos2 = 0.;
	if (sampleSize * numSamples < 1000)
		sampleSize = (1000 / (double)numSamples) < 1.0 ? 1000 / (double)numSamples : 1.0;
	if (sampleSize != 1.0)
		gOrig += 2;
	if (myy != 1.0)
		gOrig += 1;
	int fineTuningEnabled = 1;
	if (!finetune) {
		if (myy != 1.0)
			gFine = gOrig;
		else
			gFine = gOrig + 1;
		fineTuningEnabled = 0;
	}

	int stabilizationEnabled = stabilization;
	if (!stabilization && myy != 1.0)
		stabilizationEnabled = true;
	usedNlinearity = gOrig;
	if (initState == FICA_INIT_GUESS && guess.n_rows != whiteningMatrix.n_cols) {
		initialStateMode = 0;
	}
	else if (guess.n_cols < numOfIc) {
		mat guess2 = randu(guess.n_rows, numOfIc - guess.n_cols) - 0.5;
		guess = join_rows(guess, guess2);
	}
	else if (guess.n_cols > numOfIc)
		guess = guess.submat(0, guess.n_rows - 1, 0, numOfIc - 1);

	if (approach == FICA_APPROACH_SYMM) {
		usedNlinearity = gOrig;
		stroke = 0.;
		notFine = 1;
		loong = 0;
		A = zeros(vectorSize, numOfIc);
		mat B = zeros(vectorSize, numOfIc);

		if (initialStateMode == 0)
			B = orth(randu(vectorSize, numOfIc) - 0.5);
		else
			B = whiteningMatrix * guess;
		mat BOld = zeros(B.n_rows, B.n_cols);
		mat BOld2 = zeros(B.n_rows, B.n_cols);
		for (int round = 0; round < maxNumIterations; round++) {
			if (round == maxNumIterations - 1) {
				A = dewhiteningMatrix * B;
				W = B.t() * whiteningMatrix;
				return false;
			}

			B = B * mpower(B.t() * B, -0.5);

			minAbsCos = abs(diagmat(B.t() * BOld)).min();
			minAbsCos2 = abs(diagmat(B.t() * BOld2)).min();

			if (1 - minAbsCos < epsilon) {
				if (fineTuningEnabled && notFine) {
					notFine = 0;
					usedNlinearity = gFine;
					myy = myyK * myyOrig;
					BOld = zeros(B.n_rows, B.n_cols);
					BOld2 = zeros(B.n_rows, B.n_cols);
				}
				else {
					A = dewhiteningMatrix * B;
					break;
				}
			}
			else if (stabilizationEnabled) {
				if (!stroke && (1 - minAbsCos2 < epsilon)) {
					stroke = myy;
					myy /= 2;
					if (mod(usedNlinearity, 2) == 0)
						usedNlinearity += 1;
				}
				else if (stroke) {
					myy = stroke;
					stroke = 0;
					if (myy == 1 && mod(usedNlinearity, 2) != 0)
						usedNlinearity -= 1;
				}
				else if (!loong && (round > maxNumIterations / 2)) {
					loong = 1;
					myy /= 2;
					if (mod(usedNlinearity, 2) == 0)
						usedNlinearity += 1;
				}
			} // stabilization enabled
			BOld2 = BOld;
			BOld = B;

			switch (usedNlinearity) {
			case FICA_NONLIN_POW3: {
				B = (X * pow(X.t() * B, 3)) / numSamples - 3 * B;
				break;
			}
			case FICA_NONLIN_POW3 + 1: {
				mat Y = X.t() * B;
				mat Gpow3 = pow(Y, 3);
				vec Beta = sumcol(pow(Y, 4));
				mat D = diagmat(pow(Beta - 3 * numSamples, -1));
				B = B + myy * B * (Y.t() * Gpow3 - diagmat(Beta)) * D;
				break;
			}
			case FICA_NONLIN_POW3 + 2: {
				mat Xsub = X.cols(getSamples(numSamples, sampleSize));
				B = (Xsub * pow(Xsub.t() * B, 3)) / Xsub.cols(span::all) - 3 * B;
				break;
			}
			case FICA_NONLIN_POW3 + 3: {
				mat Ysub = X.cols(getSamples(numSamples, sampleSize)).t() * B;
				mat Gpow3 = pow(Ysub, 3);
				vec Beta = sumcol(pow(Ysub, 4));
				mat D = diagmat(pow(Beta - 3 * Ysub.rows(span::all), -1));
				B = B + myy * B * (Ysub.t() * Gpow3 - diagmat(Beta)) * D;
				break;
			}
			} // end switch
		} // for max iterations

		W = B.t() * whiteningMatrix;
	} // IF FICA_SYMM APPROACH
	else { // DEFLATION
		A = zeros(whiteningMatrix.n_cols, numOfIc);
		mat B = zeros(vectorSize, numOfIc);
		W = B.t() * whiteningMatrix;
		int round = 1;
		int numFailures = 0;
		while (round <= numOfIc) {
			myy = myyOrig;
			usedNlinearity = gOrig;
			stroke = 0;
			notFine = 1;
			loong = 0;
			int endFineTuning = 0;
			vec w = zeros(vectorSize);
			if (initialStateMode == 0)
				w = randu(vectorSize) - 0.5;
			else
				w = whiteningMatrix * guess.col(round);
			w = w - B * B.t() * w;
			w /= norm(w);
			vec wOld = zeros(vectorSize);
			vec wOld2 = zeros(vectorSize);
			int i = 1;
			int gabba = 1;
			while (i <= maxNumIterations + gabba) {
				w = w - B * B.t() * w;
				w /= norm(w);
				if (notFine) {
					if (i == maxNumIterations + 1) {
						round--;
						numFailures++;
						if (numFailures > failureLimit) {
							if (round == 0) {
								A = dewhiteningMatrix * B;
								W = B.t() * whiteningMatrix;
							} // if round
							return false;
						} // if numFailures > failureLimit
						break;
					} // if i == maxNumIterations + 1
				} // if notFine
				else if (i >= endFineTuning)
					wOld = w;
				if (norm(w - wOld) < epsilon || norm(w + wOld) < epsilon) {
					if (fineTuningEnabled && notFine) {
						notFine = 0;
						gabba = maxFineTune;
						wOld = zeros(vectorSize);
						wOld2 = zeros(vectorSize);
						usedNlinearity = gFine;
						myy = myyK * myyOrig;
						endFineTuning = maxFineTune + 1;
					} // if fineTuningEnabled
					else {
						numFailures = 0;
						B.col(round - 1) = w;
						A.col(round - 1) = dewhiteningMatrix * w;
						W.col(round - 1) = whiteningMatrix.t() * w;
						break;
					} // else fineTuning
				} // if epsilon
				else if (stabilizationEnabled) {
					if (stroke == 0.0 && (norm(w - wOld2) < epsilon || norm(w + wOld2) < epsilon)) {
						stroke = myy;
						myy /= 2.0;
						if (mod(usedNlinearity, 2) == 0) {
							usedNlinearity++;
						} // if mod
					} // if stroke
					else if (stroke != 0.0) {
						myy = stroke;
						stroke = 0.0;
						if (myy == 1 && mod(usedNlinearity, 2) != 0) {
							usedNlinearity--;
						}
					} // if stroke
					else if (notFine && !loong && i > maxNumIterations / 2) {
						loong = 1;
						myy /= 2.0;
						if (mod(usedNlinearity, 2) == 0) {
							usedNlinearity++;
						} // if mod
					} // if not fine
				} // if stabilization
				wOld2 = wOld;
				wOld = w;

				switch (usedNlinearity) {
				case FICA_NONLIN_POW3: {
					w = (X * pow(X.t() * w, 3)) / numSamples - 3 * w;
					break;
				}
				case FICA_NONLIN_POW3 + 1: {
					vec Y = X.t() * w;
					vec Gpow3 = X * pow(Y, 3) / numSamples;
					double Beta = dot(w, Gpow3);
					w = w - myy * (Gpow3 - Beta * w) / (3 - Beta);
					break;
				}
				case FICA_NONLIN_POW3 + 2: {
					mat Xsub = X.cols(getSamples(numSamples, sampleSize));
					w = (Xsub * pow(Xsub.t() * w, 3)) / Xsub.cols(span::all) - 3 * w;
					break;
				}
				case FICA_NONLIN_POW3 + 3: {
					mat Xsub = X.cols(getSamples(numSamples, sampleSize));
					vec Gpow3 = Xsub * pow(Xsub.t() *w, 3) / Xsub.cols(span::all);
					double Beta = dot(w, Gpow3);
					w = w - myy * (Gpow3 - Beta * w) / (3 - Beta);
					break;
				}
				} // switch usedNlinearity
				w /= norm(w);
				i++;
			} // while i <= maxNumIterations + gabba
			round++;
		} // while round <= numOfIc
	} // else deflation
	return true;
}

mat orth(const mat& A)
{
	mat Q, U, V;
	vec S;
	double eps = 2.2e-16;
	double tol = 0.;
	int mmax = 0, r = 0;
	svd(U, S, V, A);
	if (A.n_rows > A.n_cols) {
		U = U.submat(0, U.n_rows - 1, 0, A.n_cols - 1);
		S = S.subvec(0, A.n_cols - 1);
	}
	mmax = A.n_rows > A.n_cols ? A.n_rows : A.n_cols;
	tol = mmax * eps * max(S);
	for (arma::uword i = 0; i < S.n_elem; i++)
		if (S(i) > tol)
			r++;

	Q = U.submat(0, U.n_rows - 1, 0, r - 1);
	return Q;
}


void whitenv(const mat& vectors, const mat& E, const mat& D, mat& newVectors, mat& whiteningMatrix, mat& dewhiteningMatrix)
{
	whiteningMatrix = zeros(E.n_cols, E.n_rows);
	dewhiteningMatrix = zeros(E.n_rows, E.n_cols);
	for (arma::uword i = 0; i < D.n_cols; i++) {
		whiteningMatrix.row(i) = std::pow(std::sqrt(D(i, i)), -1) * E.col(i).t();
		dewhiteningMatrix.col(i) = std::sqrt(D(i, i)) * E.col(i);
	}
	newVectors = whiteningMatrix * vectors;
}

void remmean(mat& inVectors, mat& outVectors, vec& meanValue)
{
	outVectors = zeros(inVectors.n_rows, inVectors.n_cols);
	meanValue = zeros(inVectors.n_rows);
	for (arma::uword i = 0; i < inVectors.n_rows; i++) {
		meanValue(i) = mean(inVectors.row(i));
		for (arma::uword j = 0; j < inVectors.n_cols; j++)
			outVectors(i, j) = inVectors(i, j) - meanValue(i);
	}
}

void selcol(const mat& oldMatrix, const vec& maskVector, mat& newMatrix)
{
	int numTaken = 0;
	for (arma::uword i = 0; i < maskVector.n_elem; i++)
		if (maskVector(i) == 1)
			numTaken++;
	newMatrix = zeros(oldMatrix.n_rows, numTaken);
	numTaken = 0;
	for (arma::uword i = 0; i < maskVector.n_elem; i++)
		if (maskVector(i) == 1) {
			newMatrix.col(numTaken) = oldMatrix.col(i);
			numTaken++;
		}
}

int pcamat(mat& vectors, int numOfIc, int firstEig, int lastEig, mat& Es, vec& Ds)
{
	mat Et;
	vec Dt;
	cx_mat Ec;
	cx_vec Dc;
	double lowerLimitValue = 0., higherLimitValue = 0.;
	arma::uword oldDimension = vectors.n_rows;
	mat covarianceMatrix = cov(vectors.t());
	eig_sym(Dt, Et, covarianceMatrix);
	int maxLastEig = 0;

	// compute rank
	for (arma::uword i = 0; i < Dt.n_elem; i++) {
		double v = Dt(i);
		if (Dt(i) > FICA_TOL)
			maxLastEig++;
	}
	if (maxLastEig < 1)
		return 0;

	// force numOfIc components
	if (maxLastEig > numOfIc)
		maxLastEig = numOfIc;
	
	vec eigenvalues = zeros(Dt.n_elem);
	vec eigenvalues2 = zeros(Dt.n_elem);
	eigenvalues2 = Dt;
	sort(eigenvalues2);
	vec lowerColumns = zeros(Dt.n_elem);

	for (arma::uword i = 0; i < Dt.n_elem; i++)
		eigenvalues(i) = eigenvalues2(Dt.n_elem - i - 1);
	if (lastEig > maxLastEig) 
		lastEig = maxLastEig;
	if (lastEig < oldDimension)
		lowerLimitValue = (eigenvalues(lastEig - 1) + eigenvalues(lastEig)) / 2;
	else
		lowerLimitValue = eigenvalues(oldDimension - 1) - 1;
	for (arma::uword i = 0; i < Dt.n_elem; i++) {
		if (Dt(i) > lowerLimitValue)
			lowerColumns(i) = 1;
	}
	if (firstEig > 1)
		higherLimitValue = (eigenvalues(firstEig - 2) + eigenvalues(firstEig - 1)) / 2;
	else
		higherLimitValue = eigenvalues(0) + 1;

	vec higherColumns = zeros(Dt.n_elem);
	for (arma::uword i = 0; i < Dt.n_elem; i++) {
		if (Dt(i) < higherLimitValue)
			higherColumns(i) = 1;
	}
	vec selectedColumns = zeros(Dt.n_elem);
	for (arma::uword i = 0; i < Dt.n_elem; i++) {
		selectedColumns(i) = (lowerColumns(i) == 1 && higherColumns(i) == 1) ? 1 : 0;
	}
	selcol(Et, selectedColumns, Es);
	int numTaken = 0;
	for (arma::uword i = 0; i < selectedColumns.n_elem; i++) {
		if (selectedColumns(i) == 1)
			numTaken++;
	}
	Ds = zeros(numTaken);
	numTaken = 0;
	for (arma::uword i = 0; i < Dt.n_elem; i++) {
		if (selectedColumns(i) == 1) {
			Ds(numTaken) = Dt(i);
			numTaken++;
		}
	}
	return lastEig;
}

int ICA::fica(int ncomp)
{
	int approach = FICA_APPROACH_SYMM;
	int g = FICA_NONLIN_POW3;
	bool finetune = true;
	double a1 = 1., a2 = 1., mu = 1.;
	double epsilon = 0.0001;
	double sampleSize = 1.0;
	int stabilization = false;
	int maxNumIterations = 100000;
	int maxFineTune = 100;
	int firstEig = 1;
	
	sendMessage("fica: converting channels to matrix...");
	qint64 dataLength = m_channels.first()->dataSize();
	mat X(m_channels.size(), dataLength);
	for (qint64 r = 0; r < m_channels.size(); r++)
		for (qint64 c = 0; c < dataLength; c++)
			X(r, c) = m_channels.at(r)->data()[c];

//	if (m_modality == AwChannel::MEG)
//		X *= 1E12;
	int lastEig = X.n_rows;
	int initState = FICA_INIT_RAND;

	mat A, W, icasig;
	mat initGuess;
	mat whiteningMatrix, dewhiteningMatrix;
	mat whitesig;
	mat E, VecPr;
	vec D;
	arma::uword Dim = ncomp, numOfIc = ncomp;

	mat guess = zeros(ncomp, ncomp);
	VecPr = zeros(X.n_rows, ncomp);
	mat Xc;
	icasig = zeros(ncomp, X.n_cols);
	vec mixedMean;
	remmean(X, Xc, mixedMean);
	if (pcamat(Xc, ncomp, firstEig, lastEig, E, D) < 1) {
		sendMessage("No principal components could be found (e.g. all-zero data). Finished.");
		return -1;
	}
	whitenv(Xc, E, diagmat(D), whitesig, whiteningMatrix, dewhiteningMatrix);
	Dim = whitesig.n_rows;
	if (numOfIc > Dim)
		numOfIc = Dim;

	ivec NcFirst = conv_to<ivec>::from(zeros(numOfIc));
	vec NcVp = D;
	for (arma::uword i = 0; i < NcFirst.n_elem; i++) {
		NcFirst(i) = NcVp.index_max();
		NcVp(NcFirst(i)) = 0.;
		VecPr.col(i) = dewhiteningMatrix.col(i);
	}
	bool result = fpica(whitesig, whiteningMatrix, dewhiteningMatrix, approach, numOfIc, g, finetune, a1, a2, mu,
		stabilization, epsilon, maxNumIterations, maxFineTune, initState, guess, sampleSize, A, W);

	if (result) {
		m_unmixing = W;
		if (numOfIc < X.n_rows)
			m_mixing = pinv(m_unmixing);
		else
			m_mixing = inv(m_unmixing);
	}
	else {
		sendMessage("Failed to compute fica.");
		return -1;
	}
	return 0;
}