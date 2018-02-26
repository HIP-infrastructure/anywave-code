#ifndef FICA_H
#define FICA_H

#include <AwChannel.h>
#include <itpp/base/mat.h>
using namespace itpp;

namespace FICAAlgo {
	bool FICA(const AwChannelList& channels, int modality, int m, int n, int nc);
	double *getW();
}

#endif