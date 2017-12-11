#include <mlpack/core.hpp>
#include "ica.h"

void ICA::fastica()
{
	double lmbinit = 0.995;

	sendMessage("FastICA: converting channels to matrix...");
	qint64 dataLength = m_channels.first()->dataSize();
	int rows = m_channels.size();
	mat X(m_channels.size(), dataLength);
	for (qint64 r = 0; r < rows; r++)
		for (qint64 c = 0; c < dataLength; c++)
			X(r, c) = m_channels.at(r)->data()[c];
	sendMessage("FastICA: done.");
	mat W(rows, rows);
	for (qint64 i = 0; i < W.n_elem; i++)
		W.memptr()[i] = mlpack::math::Random(-1, 1);
	qint64 iterate_num = 0;
	sendMessage("FastICA: computing...");
	for (qint64 i = 0; i < dataLength; i++) {
		iterate_num++;
		mat Y = W * X.col(i);
		mat tanh1 = exp(Y);
		mat tanh2 = exp(-1 * Y);
		mat nonlin = (tanh1 - tanh2) / (tanh1 + tanh2);
		mat gn = W.t() * nonlin;
		double lmd = lmbinit / pow((double)iterate_num, 0.7);

		W = W + (lmd / (1 - lmd)) * (W - ((Y * gn.t()) / (1 + lmd * as_scalar((nonlin.t() * Y) - 1))));
	}
	sendMessage("FastICA: done.");
	m_unmixing = W;
	m_mixing = inv(W);
}