#include "ICASobi.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include "sobi_coder.h"

ICASobi::ICASobi(ICA* process, QObject *parent) : ICAAlgorithm(process, parent)
{
	m_name = "sobi";
	m_flags = ICAAlgorithm::CanDoPCA;
}

void ICASobi::run()
{
	coder::array<double, 2U> data;
	coder::array<double, 2U> mixing;
	coder::array<double, 2U> unmixing;
	coder::SizeType m = m_channels.size();
	coder::SizeType n = m_channels.first()->dataSize();

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
	emit progressChanged("Starting SOBI computation...");
	sobi_coder(data, m_nComp, mixing, unmixing);
	emit progressChanged("Done.");
	m_mixing = arma::mat(mixing.data(), mixing.size(0), mixing.size(1));
	m_unmixing = arma::mat(unmixing.data(), unmixing.size(0), unmixing.size(1));
}