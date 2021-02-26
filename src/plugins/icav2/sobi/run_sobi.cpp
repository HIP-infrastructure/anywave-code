#include "../ica.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "sobi_coder.h"

void ICA::run_sobi(int m, int n)
{
	coder::array<double, 2U> data;
	coder::array<double, 2U> mixing;
	coder::array<double, 2U> unmixing;

	data.set_size(m, n);
	// Loop over the array to initialize each element.
	for (coder::SizeType idx0 = 0; idx0 < data.size(0); idx0++) {
		for (coder::SizeType idx1 = 0; idx1 < data.size(1); idx1++) {
			data[idx0 + data.size(0) * idx1] = m_channels.at(idx0)->data()[idx1];
		}
	}
	// saving memory by clearing loaded data
	for (AwChannel* c : m_channels)
		c->clearData();
	sobi_coder(data, mixing, unmixing);
	m_mixing = arma::mat(mixing.data(), mixing.size(0), mixing.size(1));
	m_unmixing = arma::mat(unmixing.data(), unmixing.size(0), unmixing.size(1));
}