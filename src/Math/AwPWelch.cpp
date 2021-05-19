// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <math/AwMath.h>
#include <math/FFT.h>
#include <iostream>
using std::cout;
using std::vector;
using namespace AwMath;

//AwPWelch::AwPWelch()
//{
//	m_segmentLength = 2048;
//	m_samplingInterval = 1.0;
//	m_calcCSD = true;
//	m_overlap = std::floor(2 * m_segmentLength / 3);
//}
//
//AwPWelch::AwPWelch(int segmentLength)
//{
//	m_segmentLength = segmentLength;
//	m_samplingInterval = 1.0;
//	m_calcCSD = true;
//	m_overlap = std::floor(2 * m_segmentLength / 3);
//}
//
//AwPWelch::AwPWelch(int segmentLength, int overlap)
//{
//	m_segmentLength = segmentLength;
//	m_samplingInterval = 1.0;
//	m_calcCSD = true;
//	m_overlap = overlap;
//}
//
//AwPWelch::AwPWelch(int segmentLength, int overlap, double sampling)
//{
//	m_segmentLength = segmentLength;
//	m_samplingInterval = sampling;
//	m_calcCSD = true;
//	m_overlap = overlap;
//}
//AwPWelch::AwPWelch(int segmentLength, int overlap, double sampling, bool calcCSD)
//{
//	m_segmentLength = segmentLength;
//	m_samplingInterval = sampling;
//	m_calcCSD = calcCSD;
//	m_overlap = overlap;
//}
//
//void AwPWelch::setSegmentLength(int length)
//{
//	m_segmentLength = length;
//}
//
//void AwPWelch::setSamplingInterval(double sampling)
//{
//	m_samplingInterval = sampling;
//}
//
//void AwPWelch::setOverlap(int overlap)
//{
//	m_overlap = overlap;
//}
//
//void AwPWelch::setCalculateCSD(bool calcCSD)
//{
//	m_calcCSD = calcCSD;
//}
//
//void AwPWelch::apply(const mat& in, mat& psd_out)
//{
//	unsigned int nrows = in.n_rows;
//	unsigned int ncols = in.n_cols;
//	unsigned int j = 0, row_pos = 0;
//
//	if (nrows == 0 || ncols == 0)
//		return;
//
//	if (ncols == 1)
//		m_calcCSD = false;
//
//	if (m_calcCSD)
//		m_CSD = zeros<cx_cube>(ncols, ncols, m_segmentLength / 2 + 1);
//
//	int step = m_segmentLength - m_overlap;
//	colvec hamming = AwMath::hamming(m_segmentLength);
//
//	vector<int> positions;
//	while ((row_pos + m_segmentLength) < nrows) {
//		positions.push_back(row_pos);
//		row_pos += step;
//	}
//
//	psd_out.reset();
//	psd_out = zeros<mat>(m_segmentLength / 2 + 1, ncols);
//
//	cx_mat tempPSD(m_segmentLength, ncols);
//
//	mat chunk(m_segmentLength, ncols);
//	AwMath::FFT fft(m_segmentLength);
//	vector<int>::iterator it;
//
//	for (it = positions.begin(); it != positions.end(); ++it) {
//		chunk = in.rows(*it, *it + m_segmentLength - 1);
//		AwMath::detrend(chunk);
//
//		for (j = 0; j < ncols; ++j) {
//			chunk.col(j) = chunk.col(j) % hamming;
//			fft.exec(chunk.colptr(j), tempPSD.colptr(j));
//		}
//
//		psd_out += arma::abs(tempPSD);
//
//		if (m_calcCSD) {
//			for (size_t col = 0; col < tempPSD.n_cols; ++col) {
//				for (size_t col2 = 0; col2 < tempPSD.n_cols; ++col2) {
//					for (size_t row = 0; row < tempPSD.n_rows; ++ row) 
//						m_CSD(col, col2, row) += tempPSD(row, col) * conj(tempPSD(row, col2));
//				}
//			}
//		}
//	}
//
//	int nshifts = (positions.end() - positions.begin());
//	// Scale factor
//	// scale = nffts * seg_len * Fs * win_meansq
//	// where win_meansq = (window.' * window) / seg_len;
//	double win_meansq = arma::accu(hamming % hamming) / hamming.n_elem;
//	double scale = win_meansq * nshifts * (double)m_segmentLength / (2.0 * m_samplingInterval);
//
//	if (m_calcCSD)
//		m_CSD /= scale;
//	psd_out /= scale;
//}
//
//cx_cube& AwPWelch::getCSD()
//{
//	return m_CSD;
//}