#include <math/AwHilbertTransform.h>
#include <math/AwFFT.h>




void AwHilbertTransform::FHT(vec& data, int direction)
{
	auto N = data.n_elem;

	// forward
	if (direction == AwHilbertTransform::Forward) {

	}
	//	//vec z(N, arma::fill::zeros);
	//	//cx_vec cdata(data, z);
	//	// perform fft
	//	AwMathMutex.lock();
	//	fftw_plan plan = fftw_plan_dft_r2c_1d(n, in, fftx, FFTW_ESTIMATE);
	//	TFMutex.unlock();
	//	auto plan = fftw_plan_r2r()
	//	auto res = arma::fft(data);
	//	for (auto i = 1; i < (N / 2); i++)
	//		res(span(1, (N / 2)) *= 2.0;
	//}
}