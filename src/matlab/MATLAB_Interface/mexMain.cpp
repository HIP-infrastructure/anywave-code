
#include "mexMain.h"
#include <map>
#include <AwProcess.h>
#include "MATLAB_Interface.h"
#include "TCPRequest.h"
#include <QDataStream>

using namespace matlab::data;
using matlab::mex::ArgumentList;

std::map<std::string, int> functions;

MexFunction::MexFunction()
{
	functions["debug_connect"] = AwRequest::ConnectDebug;
	functions["get_data"] = AwRequest::GetDataEx;
}

void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
{
	// Get array factory
	ArrayFactory factory;
	std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
	
	if (inputs.empty()) {
		matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray("Requires at least one argument") }));
	}
	if (inputs[0].getType() != ArrayType::CHAR) {
		matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray("First input must be a string") }));
	}
	// dispatch anywave requests based on the first input parameter that must a string
	std::string command = matlab::data::CharArray(inputs[0]).toAscii();
	int request = functions[command];
	switch (request)
	{
	case AwRequest::ConnectDebug:
		debug_connect(outputs, inputs);
		break;
	case AwRequest::GetDataEx:
		get_data(outputs, inputs);
		break;
	default:
		matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray("Unknown command") }));
	}
	
}

void MexFunction::get_data(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	ArrayFactory factory;
	std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
	// input may be empty or contain one argument(the cfg structure)
	QString args;
	// don't forget that first argument is always the command string
	if (inputs.size() >= 2) {
		if (inputs[1].getType() == ArrayType::STRUCT) {
			
			// ask MATLAB to convert the struct to json
			auto res = matlabPtr->feval(u"jsonencode", 1, std::vector<Array>({ inputs[1] }));
			
			CharArray s = std::move(res[0]);
			std::string ss = s.toAscii();
			args = QString(ss.data());
		}
		else {
			matlabPtr->feval(u"error",
				0,
				std::vector<Array>({ factory.createCharArray("get_data: bad arguments. Expected struct.") }));
		}
	}
	int nChannels;
	try {
		TCPRequest aw(AwRequest::GetDataEx);
		aw.sendString(args);
		aw.waitForResponse();
		QDataStream& response = aw.response();
		response >> nChannels;
		if (nChannels == 0) {
			return;
		}
		StructArray S = factory.createStructArray({ 1, size_t(nChannels) }, { "name", "type", "ref", "sr", "hp", "lp", "notch", "data" });
		QString name, ref, type;
		float sr, hp, lp, notch;
		qint64 samples;
		for (auto i = 0; i < nChannels; i++) {
			aw.waitForResponse();
			response >> name >> type >> ref >> sr >> hp >> lp >> notch;
			std::string tmp = name.toStdString();
			S[i]["name"] = factory.createCharArray(tmp);
			tmp = type.toStdString();
			S[i]["type"] = factory.createCharArray(tmp);
			tmp = ref.toStdString();
			S[i]["ref"] = factory.createCharArray(tmp);
			S[i]["sr"] = factory.createScalar<double>(sr);
			S[i]["hp"] = factory.createScalar<double>(hp);
			S[i]["lp"] = factory.createScalar<double>(lp);
			S[i]["notch"] = factory.createScalar<double>(notch);
			response >> samples;
			qint64 samplesRead, chunkSize;;
			bool finished = false;
			if (samples) {
				auto data = factory.createArray<float>({ 1, size_t(samples) });
				//S[i]["data"] = factory.createArray<double>({ 1, size_t(samples) });

				finished = false;
				samplesRead = 0;
				chunkSize = 0;
				while (!finished) {
					aw.waitForResponse();
					response >> chunkSize;
					if (chunkSize == 0)
						finished = true;
					else {
						for (auto j = 0; j < chunkSize; j++) {
							response >> data[0][j + samplesRead];
						}
						samplesRead += chunkSize;
					}
				}
				S[i]["data"] = data;
			}
		}
		outputs[0] = S;
	}
	catch (const QString& what) {
		QString error = QString("get_data: %1").arg(what);
		std::string s = error.toStdString();
		matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray(s) }));
	}
}

void MexFunction::debug_connect(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	ArrayFactory factory;
	std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
	//matlabPtr->feval(u"disp", 0, std::vector<Array>{factory.createCharArray("debug connect")});

	// get input arguments #2 which could be the port number
	MATLAB_Interface::pid = -1;
	// don't forget that first argument is always the command string
	bool ok = inputs.size() == 2 && inputs[1].getType() == ArrayType::DOUBLE;
	if (!ok) {
		matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray("debug_connect: bad arguments. Expected port number.") }));
	}
	MATLAB_Interface::port = static_cast<int>(inputs[1][0]);
	int pid;
	QString args;
	try {
		TCPRequest aw(AwRequest::ConnectDebug);
		aw.simpleRequest();
		aw.waitForResponse();
		QDataStream& response = aw.response();

		response >> pid >> args;
	}
	catch (const QString& what) {
		QString error = QString("debug_connect: %1").arg(what);
		std::string s = error.toStdString();
		matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray(s) }));
	}
	MATLAB_Interface::pid = pid;
	if (!args.isEmpty()) {
		matlab::data::MATLABString s = args.toStdU16String();
		matlabPtr->setVariable(u"json_args", factory.createCharArray(s));
		matlabPtr->eval(u"args=jsondecode(json_args);");

	}
}
