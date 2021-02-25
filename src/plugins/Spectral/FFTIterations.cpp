#include "FFTIterations.h"

void FFTIterations::appendIteration(const arma::vec& iter)
{
	m_iterations.col(m_iterationIndex++) = iter;
}

vec& FFTIterations::computePxx()
{
	m_pxx = arma::mean(m_iterations, 1); // mean for each cols
	return m_pxx;
}
