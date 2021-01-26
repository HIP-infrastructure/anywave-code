#pragma once
#include <AwChannel.h>
#include <aw_armadillo.h>
// data structure to hold all fft iterations for a channel
class FFTIterations
{
public:
	FFTIterations(AwChannel* chan) { m_channel = chan; m_iterationIndex = 0; }
	void setNumberOfIterations(uword count, uword timeWindowPts) { m_iterations = arma::zeros(timeWindowPts, count); }
	void appendIteration(const arma::vec& iteration);
	vec& computePxx();

	inline mat& results() { return m_iterations; }
	inline vec& pxx() { return m_pxx; }
	AwChannel* channel() { return m_channel; }
protected:
	AwChannel* m_channel;
	arma::mat m_iterations;	// each cols of iterations are ONE fft iteration
	arma::vec m_pxx;
	uword m_iterationIndex;
};

using FFTs = QList<FFTIterations*>;