//#include "UtilsCpp.h"
#include "common.h"
#include <AwProcess.h>
//#include "mex.hpp"
//#include "mexAdapter.hpp"
#include <QDataStream>

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	TCPRequest request(AwRequest::ConnectDebug);
	if (request.status() != TCPRequest::connected) {
		mexErrMsgTxt("Failed to connect to AnyWave.");
		return;
	}
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
	int pid;
	QString args;
	in >> pid >> args;
	mxArray* mPid = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxArray* mArgs = mxCreateString(args.toStdString().c_str());
	int* d = (int*)mxGetData(mPid);
	d[0] = pid;
	plhs[0] = mPid;
	plhs[1] = mArgs;
}

//using namespace matlab::data;
//using matlab::mex::ArgumentList;
//class MexFunction : public matlab::mex::Function {
//public:
//    void operator()(ArgumentList outputs, ArgumentList inputs) {
//        TCPRequest request(AwRequest::ConnectDebug);
//        if (request.status() != TCPRequest::connected) {
//           // mexErrMsgTxt("Failed to connect to AnyWave.");
//            return;
//        }
//        if (!request.sendRequest()) {
//           // mexErrMsgTxt("sending request failed.");
//            return;
//        }
//        int dataSize = request.getResponse();
//        if (dataSize < 0) {
//           // mexErrMsgTxt("AnyWave did not respond to the request.");
//            return;
//        }
//        QDataStream in(request.socket());
//        in.setVersion(QDataStream::Qt_4_4);
//        int pid;
//        QString args;
//        in >> pid >> args;
//        // Get pointer to engine
//        std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
//        ArrayFactory factory;
//        matlab::data::Array data = factory.createArray<int>({1});
//        data[0] = pid;
//        outputs[0] = data;
//        auto json = factory.createCharArray(args.toStdString());
//        outputs[1] = json;
//    }

//};


