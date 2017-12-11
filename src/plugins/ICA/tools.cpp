#include "tools.h"

namespace aw_matlab {
void srqtm(mat& X, double& alpha, mat& A) {
	uword n = max(size(A));
	mat Q, T;
	schur(Q, T, A);
	mat diagT = diagmat(T);
	if (approx_equal(T,diagmat(diagT), "both")) {

	}
}
}
