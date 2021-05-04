
#include "mexMain.h"
#include <map>
#include <string>
#include <AwProcess.h>
#include "MATLAB_Interface.h"
#include "TCPRequest.h"
#include <QDataStream>

using namespace matlab::data;
using matlab::mex::ArgumentList;

std::map<std::string, int> functions;


MexFunction::MexFunction()
{
	m_matlabPtr = getEngine();
	functions["debug_connect"] = AwRequest::ConnectDebug;
	functions["get_data"] = AwRequest::GetDataEx;
	functions["get_markers"] = AwRequest::GetMarkersEx;
	functions["send_markers"] = AwRequest::SendMarkers;
}

/// <summary>
/// mex file entry point
/// </summary>
/// <param name="outputs"></param>
/// <param name="inputs"></param>
void MexFunction::operator()(matlab::mex::ArgumentList outputs, matlab::mex::ArgumentList inputs)
{
	// Get array factory
	ArrayFactory factory;

	if (inputs.empty()) 
		error("Requires at least one argument");

	if (inputs[0].getType() != ArrayType::CHAR) 
		error("First input must be a string");
	
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
	case AwRequest::GetMarkersEx:
		get_markers(outputs, inputs);
		break;
	case AwRequest::SendMarkers:
		send_markers(outputs, inputs);
		break;
	default:
		error("Unknown command");
	}
	
}

void MexFunction::error(const std::string& message)
{
	ArrayFactory factory;
	m_matlabPtr->feval(u"error",
		0,
		std::vector<Array>({ factory.createCharArray(message) }));
}

void MexFunction::printf(std::ostringstream stream)
{
	ArrayFactory factory;
	m_matlabPtr->feval(u"fprintf", 0, std::vector<Array>
		({ factory.createScalar(stream.str()) }));
}

void MexFunction::printf(const std::string& message)
{
	ArrayFactory factory;
	m_matlabPtr->feval(u"fprintf", 0, std::vector<Array>
		({ factory.createScalar(message) }));
}

void MexFunction::send_markers(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	ArrayFactory factory;

	const int MARKERS_AT_ONCE = 1000;
	if (inputs.size() != 2)
		error(std::string("send_markers: requires 1 argument to be a struct array."));
	if (inputs[1].getType() != ArrayType::STRUCT)
		error(std::string("send_markers: argument must be a struct array."));

	// expected fields in struct
	std::vector<std::string> expectedFields = { "label", "position", "duration", "colour", "targets", "value" };
	StructArray S = std::move(inputs[1]);

	auto fields = S.getFieldNames();
	std::vector<std::string> fieldNames(fields.begin(), fields.end());

	std::map<std::string, int> map;
	map["label"] = -1;
	map["position"] = -1;
	map["duration"] = -1;
	map["targets"] = -1;
	map["value"] = -1;
	map["colour"] = -1;

	// fields must be:
	// - label => string
	// - positon => float
	// * duration => float
	// * value => float
	// * targets => cell array of strings
	// * colour => string
	if (fieldNames.size() < 2)
		error("send_markers: struct must contain at least two fields (label, position)");

	//std::ostringstream printf_stream;
	for (auto i = 0; i < fieldNames.size(); i++) {
		
		//std::string field = fieldNames[i];
		//printf_stream << "field: " << field << '\n' << "index is " << i << '\n';

		map[fieldNames[i]] = i;
	}
	//printf(std::move(printf_stream));

		 
	bool ok = map["label"] != -1 && map["position"] != -1;
	if (!ok)
		error("send_markers: struct must contain label and position fields.");

	const Array fieldLabel = S[0]["label"];
	if (fieldLabel.getType() != ArrayType::CHAR)
		error("send_markers: field  label must be a string.");
	const Array fieldPos = S[0]["position"];
	if (fieldPos.getType() != ArrayType::DOUBLE)
		error("send_markers: field position must be a double.");
	
	try {
		TCPRequest aw(AwRequest::SendMarkers);
		QByteArray data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream.setVersion(QDataStream::Qt_4_4);
		int nMarkers = S.getNumberOfElements();
		size_t counter = 0;
		QString label, colour;
		
		while (true) {
			int n = std::min(MARKERS_AT_ONCE, nMarkers);
			data.clear();
			stream.device()->reset();
			stream << n;
			for (auto i = 0; i < n; i++) {
				QStringList targets;
				float position = 0., duration = 0., value = 0.;

				CharArray field_label = S[i + counter]["label"];
				std::string s = field_label.toAscii();
				label = QString::fromStdString(s);
				stream << label;
				if (map["colour"] != -1) {
					CharArray c = S[counter]["colour"];
					colour = QString::fromStdString(c.toAscii());
				}
				stream << colour;
				TypedArray<double> pos = S[i + counter]["position"];
				position = static_cast<float>(pos[0]);
				stream << position;
				if (map["duration"] != -1) {
					const TypedArray<double> d = S[i + counter]["duration"];
					duration = static_cast<float>(d[0]);
				}
				stream << duration;
				if (map["value"] != -1) {
					TypedArray<double> v = S[i + counter]["value"];
					value = static_cast<float>(v[0]);
				}
				stream << value;
				if (map["targets"] != -1) {
					if (S[i + counter]["targets"].getType() == ArrayType::CELL) {
						CellArray ctargets = S[i + counter]["targets"];
						for (auto j = 0; j < ctargets.getNumberOfElements(); j++) {
							const CharArray t = ctargets[0][j];
							QString target = QString::fromStdString(t.toAscii());
							targets << target;
						}
					}
				}
				stream << targets;
			}
			aw.sendData(data);
			aw.waitForResponse();
			nMarkers -= n;
			counter += n;
			if (nMarkers == 0)
				break;
		}
	}
	catch (const QString& what) {
		std::string message = QString("send_markers: %1").arg(what).toStdString();
		error(message);
	}
	catch (matlab::data::TypeMismatchException& e) {
		error(e.what());
	}
}

void MexFunction::get_markers(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	ArrayFactory factory;

	QString args;
	// don't forget that first argument is always the command string
	if (inputs.size() >= 2) {
		if (inputs[1].getType() == ArrayType::STRUCT) {

			// ask MATLAB to convert the struct to json
			auto res = m_matlabPtr->feval(u"jsonencode", 1, std::vector<Array>({ inputs[1] }));

			CharArray s = std::move(res[0]);
			std::string ss = s.toAscii();
			args = QString(ss.data());
		}
		else 
			error(std::string("get_markers: bad arguments.Expected struct."));
	}
	int nMarkers;
	QString label;
	float position, duration, value;
	QStringList targets;
	try {
		TCPRequest aw(AwRequest::GetMarkersEx);
		aw.sendString(args);
		aw.waitForResponse();
		QDataStream& response = aw.response();
		response >> nMarkers;
		if (nMarkers == 0) {
			outputs[0] = factory.createEmptyArray();
			return;
		}
		StructArray S = factory.createStructArray({ 1, size_t(nMarkers) }, { "label", "position", "duration", "value", "targets" });
		aw.waitForResponse();
		for (auto i = 0; i < nMarkers; i++) {
			response >> label >> position >> duration >> value >> targets;
			std::string tmp = label.toStdString();
			S[i]["label"] = factory.createCharArray(tmp);
			S[i]["position"] = factory.createScalar<double>(position);
			S[i]["duration"] = factory.createScalar<double>(duration);
			S[i]["value"] = factory.createScalar<double>(value);
			// create cell array based on targets
			CellArray C = factory.createCellArray({ 1, size_t(targets.size()) });
			for (auto j = 0; j < targets.size(); j++) {
				tmp = targets.at(j).toStdString();
				C[0][j] = factory.createCharArray(tmp);
			}
			S[i]["targets"] = C;
		}
		outputs[0] = S;
	}
	catch (const QString& what) {
		std::string s = QString("get_markers: %1").arg(what).toStdString();
		error(s);
	}
}

void MexFunction::get_data(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	ArrayFactory factory;

	// input may be empty or contain one argument(the cfg structure)
	QString args;
	// don't forget that first argument is always the command string
	if (inputs.size() >= 2) {
		if (inputs[1].getType() == ArrayType::STRUCT) {
			
			// ask MATLAB to convert the struct to json
			auto res = m_matlabPtr->feval(u"jsonencode", 1, std::vector<Array>({ inputs[1] }));
			
			CharArray s = std::move(res[0]);
			std::string ss = s.toAscii();
			args = QString(ss.data());
		}
		else 
			error("get_data: bad arguments. Expected struct.");
	}
	int nChannels;
	try {
		TCPRequest aw(AwRequest::GetDataEx);
		aw.sendString(args);
		aw.waitForResponse();
		QDataStream& response = aw.response();
		response >> nChannels;
		if (nChannels == 0) {
			// return empty struct
			outputs[0] = factory.createEmptyArray();
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
		std::string s = QString("get_data: %1").arg(what).toStdString();
		error(s);
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
