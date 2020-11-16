#include "common.h"
#include <AwProcess.h>


void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	TCPRequest request(AwRequest::GetProperties);
	if (request.status() != TCPRequest::connected) {
		mexErrMsgTxt("Failed to connect to AnyWave.");
		return;
	}

	QDataStream* stream = request.stream();

	if (!request.sendRequest()) {
		mexErrMsgTxt("sending request failed.");
		return;
	}

	int dataSize = request.getResponse();
	if (dataSize < 0) {
		mexErrMsgTxt("AnyWave did not respond to the request.");
		return;
	}
	QDataStream in(request.socket());
	in.setVersion(QDataStream::Qt_4_4);
	QString json;
	in >> json;
	mxArray* input[1], * output[1];
	input[0] = mxCreateString(json.toStdString().c_str());
	mexCallMATLAB(1, output, 1, input, "jsondecode");
	plhs[0] = output[0];
}