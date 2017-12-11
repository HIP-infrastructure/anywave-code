#include "ica.h"
#include <math/AwMath.h>
#include <time.h>
#include "pca.h"

int ICA::runica_matlab(int nc)
{
	mat data = AwMath::channelsToMat(m_channels);
	for (auto c : m_channels)
		c->clearData();
	uword chans = data.n_rows;
	uword urchans = chans;
	uword frames = data.n_cols;
	uword datalength = frames;
	if (chans < 2) {
		sendMessage("data size is too small.");
		return -1;
	}

	// defaults
	double MAX_WEIGHT = 1e8;
	double DEFAULT_STOP = 0.000001;
	double DEFAULT_ANNEALDEG = 60.;
	double DEFAULT_ANNEALSTEP = 0.9;
	double DEFAULT_EXTANNEAL = 0.98;
	int DEFAULT_MAXSTEPS = 512;
	double DEFAULT_MOMENTUM = 0.;
	double DEFAULT_EXTMOMENTUM = 0.5;
	double DEFAULT_BLOWUP = 1000000000.;
	double DEFAULT_BLOWUP_FAC = 0.8;
	double DEFAULT_RESTART_FAC = 0.9;
	double MIN_LRATE = 0.000001;
	double MAX_LRATE = 0.1;
	double DEFAULT_LRATE = 0.00065 / log(chans);
	int DEFAULT_BLOCK = (int)std::ceil(std::min(5 * log(frames), 0.3 * frames));
	int DEFAULT_EXTENDED = 0;
	int DEFAULT_EXTBLOCKS = 1;
	int DEFAULT_NSUB = 1;
	int MAX_KURTSIZE = 6000, MIN_KURTSIZE = 2000;
	int SIGNCOUNT_THRESHOLD = 25;
	int SIGNCOUNT_STEP = 2;
	bool DEFAULT_SPHEREFLAG = true;
	bool DEFAULT_INTERUPT = false;
	bool DEFAULT_PCAFLAG = false;
	bool DEFAULT_POSACTFLAG = false;
	int DEFAULT_BIASFLAG = 1;
	bool DEFAULT_RESETRANDOMSEED = true;

	double annealdeg = DEFAULT_ANNEALDEG;
	double annealstep = 0.;
	double nochange = NAN;
	bool posactflag = DEFAULT_POSACTFLAG;
	double momentum = DEFAULT_MOMENTUM;
	int maxsteps = DEFAULT_MAXSTEPS;
	int biasflag = DEFAULT_BIASFLAG;
	double signsbias = 0.02;
	bool interupt = DEFAULT_INTERUPT;
	int extended = DEFAULT_EXTENDED;
	int extblocks = DEFAULT_EXTBLOCKS;
	int kurtsize = MAX_KURTSIZE;
	double extmomentum = DEFAULT_EXTMOMENTUM;
	int nsub = DEFAULT_NSUB;
	int wts_blowup = 0;
	int wts_passed = 0;
	bool reset_randomseed = DEFAULT_RESETRANDOMSEED;
	bool pcaflag = DEFAULT_PCAFLAG;
	double lrate = DEFAULT_LRATE;
	int block = DEFAULT_BLOCK;

	if (nc < chans)
		pcaflag = true;

	if (extended)
		annealstep = DEFAULT_EXTANNEAL;
	annealdeg = DEFAULT_ANNEALDEG - momentum * 90;
	if (annealdeg < 0)
		annealdeg = 0.;

	if (nsub > nc) {
		sendMessage(QString("There can be at most %1 sub-Gaussian components.").arg(nc));
		return -1;
	}
	bool nochangeupdated;
	if (isnan(nochange)) {
		if (nc > 32) {
			nochange = 1e-7;
			nochangeupdated = true;
		}
		else {
			nochangeupdated = true;
			nochange = DEFAULT_STOP;
		}
	}
	else nochangeupdated = false;

	sendMessage(QString("Input data size [%1, %2] = %3 channels, %3 frames").arg(chans).arg(frames).arg(chans).arg(frames));
	if (pcaflag) 
		sendMessage("Using PCA for dimension reduction.");
	if (!extended)
		sendMessage("Finding components using logistic ICA.");
	else
		sendMessage("Finding components using extended ICA.");
	if (extblocks > 0) 
		sendMessage(QString("Kurtosis will be calculated initially every %1 blocks using %2 data points.").arg(extblocks).arg(kurtsize));
	else
		sendMessage(QString("Kurtosis will not be calculated. Exactly %1 sub-Gaussian assumeds.").arg(nsub));

	sendMessage(QString("Decomposing %1 frames per ICA weight ((%2)^2 = %3 weights, %4 frames").arg(std::floor(frames / (nc * nc))).arg(nc * nc).arg(frames));
	sendMessage(QString("Initial learning rate will be %1, block size %2").arg(lrate).arg(block));
	if (momentum)
		sendMessage(QString("Momentum will be %1").arg(momentum));
	sendMessage(QString("Learning rate will be multiplied by %1 whenever angledelat >= %2 deg.").arg(annealstep).arg(annealdeg));
	if (nochangeupdated)
		sendMessage("More than 32 channels : default stopping weight change to 1e-7");
	sendMessage(QString("Training will end when wchange < %1 or after %2 steps").arg(nochange).arg(maxsteps));
	if (biasflag)
		sendMessage("Online bias adjustment will be used.");
	else
		sendMessage("Online bias adjustment will not be used.");

	sendMessage("Removing mean of each channel...");
	data = data - mean(data.t()).t() * ones(1, frames);
	//mat rowmeans = arma::mean(data, 1);
	//for (uword iii = 0; iii < data.n_rows; iii++)
	//	data.row(iii) -= rowmeans(iii,0);
	sendMessage(QString("Final training data range: %1 to %2").arg(min(min(data))).arg(max(max(data))));
	mat eigenvectors, eigenvalues;
	mat sphere;
	if (pcaflag) {
		sendMessage(QString("Reducing the data to %1 principal dimensions...").arg(nc));
		//mat PCdat2 = data.t();
		//uword PCn = PCdat2.n_rows;
		//uword PCp = PCdat2.n_cols;
		//PCdat2 /= PCn;
		//mat PCout = data * PCdat2;
		//PCdat2.clear();
		//vec eigval;
		//mat eigvec;
		//if (eig_sym(eigval, eigvec, PCout) == false) {
		//	return -1;
		//}

		//vec PCeigenval = sort(eigval);
		//uvec PCindex = sort_index(eigval);
		//PCindex = arma::flipud(PCindex);
		//mat PCEigenvectors = eigvec.cols(PCindex);
		//data = PCEigenvectors.cols(span(0, nc - 1)).t() * data;
		// pca_fsvd(data, data, nc, 2);
		pca(data, nc, 1e8, data);
	}
	sendMessage("Computing the sphering matrix...");
	sphere = 2.0 * inv(sqrtmat_sympd(cov(data.t())));
	// use identity matrix for weights
	mat weigths(uword(nc), chans, fill::eye);
	sendMessage("Sphering the data...");
	data = sphere * data;

	// initialize training
	int lastt = (int)floor((datalength / block - 1) * block + 1);
	mat BI = block * eye(nc, nc);
	mat delta = zeros(1, chans * nc);
	mat olddelta = delta;
	double degconst = 180 / datum::pi;
	mat startweights = weigths;
	mat prevweights = startweights;
	mat oldweights = startweights;
	mat prevwtchange = zeros(chans, nc);
	mat oldwtchange = zeros(chans, nc);
	mat lrates = zeros(1, maxsteps);
	mat onesrow = ones(1, block);
	mat bias = zeros(nc, 1);
	vec vsigns = ones(nc);
	for (uword k = 0; k < nsub; k++)
		vsigns(k) = -1;
	if (extended && extblocks) {
		sendMessage("Fixed extented-ICA sign assignments:");
		for (uword k = 0; k < nc; k++)
			sendMessage(QString("%1").arg(vsigns(k)));
	}
	mat signs = diagmat(vsigns);
	mat oldsigns = zeros(size(signs));
	int signcount = 0;
	int urextblocks = extblocks;
	vec old_kk = zeros(nc);

	sendMessage("Beginning ICA training...");
	if (extended)
		sendMessage("first training step may be slow...");
	int step = 0, laststep = 0, blockno = 1;

	if (reset_randomseed) 
		arma_rng::set_seed(clock());
	
	double change, angledelta, oldchange, places;
	QVector<double> changes;
	// compute ICA Weights
	if (biasflag && !extended) {
		while (step < maxsteps) {
			uvec timeperm = shuffle(regspace<uvec>(0, datalength));
			for (int t = 0; t < lastt; t += block) {
				uvec range(block);
				int j = 0;
				for (int i = t; i < t + block; i++)
					range(j++) = timeperm(i);
				mat subdata = conv_to<mat>::from(data.cols(range));
				mat u = weigths * subdata + bias * onesrow;
				mat y = 1. / (1 + exp(-u));
				weigths = weigths + lrate * (BI + (1 - 2 * y) * u.t()) * weigths;
				bias = bias + lrate * sum((1 - 2 * y).t()).t();
				if (momentum > 0) {
					weigths = weigths + momentum * prevwtchange;
					prevwtchange = weigths - prevweights;
					prevweights = weigths;
				}

				if (max(max(abs(weigths))) > MAX_WEIGHT) {
					wts_blowup = 1;
					change = nochange;
				}
				blockno++;
				if (wts_blowup)
					break;
			} // end for t
			if (!wts_blowup) {
				oldwtchange = weigths - oldweights;
				step++;
				lrates(1, step) = lrate;
				angledelta = 0.;
				delta = reshape(oldwtchange, 1, chans * nc);
				mat toto = delta * delta.t();
				change = toto(1, 1);
			}

			// restart if weigths blow up
			if (wts_blowup || isnan(change) || isinf(change)) {
				step = 0;
				change = nochange;
				wts_blowup = 0;
				blockno = 1;
				lrate *= DEFAULT_LRATE;
				weigths = startweights;
				oldweights = startweights;
				oldwtchange = zeros(chans, nc);
				delta = zeros(1, chans * nc);
				olddelta = delta;
				extblocks = urextblocks;
				prevweights = startweights;
				prevwtchange = zeros(chans, nc);
				lrates = zeros(1, maxsteps);
				bias = zeros(nc, 1);
				if (lrate > MIN_LRATE) {
					uword r = rank(data);
					if (r < nc) {
						sendMessage(QString("Data has rank %1. Cannot compute %2 components.").arg(r).arg(nc));
						return -1;
					}
					else
						sendMessage(QString("Lowering learning rate to %1 and starting again.").arg(lrate));
				}
				else {
					sendMessage("QUITTING - weight matrix may not be invertible!");
					return -1;
				}
			}
			else { // if weights in bounds
				if (step > 2) {
					mat toto = delta * delta.t();
					angledelta = acos((toto(1,1) / sqrt(change * oldchange)));
				}
				places = -floor(log10(nochange));
				sendMessage(QString("step %1 - lrate %2, wchange %3, angledelta %4deg.").arg(step).arg(lrate).arg(change).arg(angledelta));
				changes.append(change);
				oldweights = weigths;

				// Anneal learning rate
				if (degconst * angledelta > annealdeg) {
					lrate *= annealstep;
					olddelta = delta;
					oldchange = change;
				}
				else if (step == 1) {
					olddelta = delta;
					oldchange = change;
				}
				if (step > 2 && change < nochange) {
					laststep = step;
					step = maxsteps;
				}
				else if (change > DEFAULT_BLOWUP) {
					lrate *= DEFAULT_BLOWUP_FAC;
				}
			}
		}
	}

	if (!laststep)
		laststep = step;

	lrates = lrates.cols(span(0, laststep - 1));

	//if (posactflag) {
	//	if (pcaflag) {
	//		mat sr = sphere * rowmeans.t();
	//		for (uword r = 0; r < nc; r++)
	//			data.row(r) = data.row(r) + sr(r);
	//		data = weigths * data;
	//	}
	//	else {
	//		mat ser = sphere * eigenvectors.cols(span(0, nc - 1)).t() * rowmeans.t();
	//		for (uword r = 0; r < nc; r++)
	//			data.row(r) = data.row(r) + ser(r);
	//		data = weigths * data;
	//	}
	//}
	if (pcaflag) {
		sendMessage("Composing the eigenvector, weights and sphere matrices...");
		sendMessage(QString(" into a signle rectangular weights matrix; sphere = eye(%1)").arg(chans));
		weigths = weigths * sphere * eigenvectors.cols(span(0, nc - 1)).t();
		sphere = eye(urchans, urchans);
	}

	sendMessage("Sorting components in descending order of mean projected variance...");
	mat winv;
	if (nc == urchans) 
		winv = inv(weigths * sphere);
	else {
		sendMessage("Using pseudo-inverse of weight matrix to rank order components projections");
		winv = pinv(weigths * sphere);
	}

	winv.for_each([](mat::elem_type& val) { val *= val; });
	mat dataT2 = data.t();
	dataT2.for_each([](mat::elem_type& val) { val *= val; });
	mat meanvar = (sum(winv) % sum(dataT2)) / (chans * frames - 1);
	mat sortvar = sort(meanvar);
	uvec windex = sort_index(meanvar);

	weigths = weigths.rows(windex);

	m_unmixing = weigths * sphere;

	if (nc < urchans)
		m_mixing = pinv(m_unmixing);
	else
		m_mixing = inv(m_unmixing);



	return 0;
}