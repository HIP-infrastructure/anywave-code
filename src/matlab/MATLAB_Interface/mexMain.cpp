
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
	functions["send_message"] = AwRequest::SendMessage;
	functions["send_text"] = AwRequest::SendMessage;
	functions["get_props"] = AwRequest::GetProperties;
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
		error("anywave: Requires at least one argument");

	if (inputs[0].getType() != ArrayType::CHAR) 
		error("anywave: First input must be a string");

	// check for init command that should initialize port, pid and args from arguments passed to main() function.
	// used for compiled plugins
	// dispatch anywave requests based on the first input parameter that must a string
	std::string command = matlab::data::CharArray(inputs[0]).toAscii();

	if (command == "init") {
		// expect argument 1 to be a cell array
		if (inputs.size() != 2)
			error("anywave: init() requires one argument (varargin).");
		if (inputs[1].getType() != ArrayType::CELL)
			error("anywave: init() requires cell array argument.");
		CellArray cellArray = inputs[1];

		// check if empty cell array => do nothing
		if (cellArray.getDimensions()[1] == 0)
			return;
		if (cellArray.getDimensions()[1] != 3)
			error("anywave: init() cell array must contain three items: host, port, pid .");
		// cell array should contain :  host, port, pid (string)
		CharArray stringHost = cellArray[0][0];
		std::string host = stringHost.toAscii();
		
		CharArray stringPort = cellArray[0][1];
		std::string port = stringPort.toAscii();
		double portValue = std::stod(port);
		CharArray stringPid = cellArray[0][2];
		std::string pid = stringPid.toAscii();
		double pidValue = std::stod(pid);
	
		// this is for old mex files compatibility
		m_matlabPtr->setVariable(u"host", factory.createCharArray(stringHost.toAscii()), matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->setVariable(u"port", factory.createScalar<double>(portValue), matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->setVariable(u"pid", factory.createScalar<double>(pidValue), matlab::engine::WorkspaceType::BASE);

		MATLAB_Interface::port = static_cast<int>(portValue);
		MATLAB_Interface::pid = static_cast<int>(pidValue);

		// DONE !!!
		return;
	}

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
	case AwRequest::SendMessage:
		send_message(outputs, inputs);
		break;
	case AwRequest::GetProperties:
		get_properties(outputs, inputs);
		break;
	default:
		error("Unknown command");
	}
	
}

void MexFunction::getPidPort()
{
	// get port and pid!!!
	auto pid = m_matlabPtr->getVariable(u"pid", matlab::engine::WorkspaceType::BASE);
	if (!pid.isEmpty()) {
		TypedArray<double> p = pid;

		MATLAB_Interface::pid = static_cast<int>(p[0]);
	}
	auto port = m_matlabPtr->getVariable(u"port", matlab::engine::WorkspaceType::BASE);
	if (!port.isEmpty()) {
		TypedArray<double> p = port;
		MATLAB_Interface::port = static_cast<int>(p[0]);
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


void MexFunction::send_message(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	if (inputs.size() != 2) 
		error(std::string("send_message: requires 1 argument to be a string."));
	if (inputs[1].getType() != ArrayType::CHAR)
		error(std::string("send_message: argument must be a string."));
	CharArray ca = inputs[1];

	try {
		TCPRequest aw(AwRequest::SendMessage);
		aw.sendString(QString::fromStdString(ca.toAscii()));
	}
	catch (const QString& what) {
		std::string message = QString("send_markers: %1").arg(what).toStdString();
		error(message);
	}
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
	std::vector<std::string> expectedFields = { "label", "position", "duration", "color", "targets", "value" };
	StructArray S = std::move(inputs[1]);

	auto fields = S.getFieldNames();
	std::vector<std::string> fieldNames(fields.begin(), fields.end());

	std::map<std::string, int> map;
	map["label"] = -1;
	map["position"] = -1;
	map["duration"] = -1;
	map["targets"] = -1;
	map["value"] = -1;
	map["color"] = -1;

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
		map[fieldNames[i]] = i;
	} 
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
				if (map["color"] != -1) {
					CharArray c = S[counter]["color"];
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
			if (nMarkers == 0) {
				// finished sending markers =>inform anywave by sending 0 as number of markers
				data.clear();
				stream.device()->reset();
				stream << (int)0;
				aw.sendData(data);
				break;
			}
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
	// this is for old mex files compatibility
	m_matlabPtr->setVariable(u"host", factory.createCharArray("127.0.0.1"), matlab::engine::WorkspaceType::BASE);
	m_matlabPtr->setVariable(u"port", factory.createScalar<double>(MATLAB_Interface::port), matlab::engine::WorkspaceType::BASE);
	m_matlabPtr->setVariable(u"pid", factory.createScalar<double>(MATLAB_Interface::pid), matlab::engine::WorkspaceType::BASE);
}


void MexFunction::get_properties(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	using namespace matlab::data;
	using matlab::mex::ArgumentList;

	ArrayFactory factory;
	QString json;
	try {
		TCPRequest aw(AwRequest::GetProperties);
		aw.simpleRequest();
		aw.waitForResponse();
		QDataStream& response = aw.response();
		response >> json;
		auto res = m_matlabPtr->feval(u"jsondecode", 1, std::vector<Array>({ factory.createCharArray(json.toStdString()) }));
		outputs[0] = StructArray(res[0]);
	}
	catch (const QString& what) {
		std::string message = QString("get_prop: %1").arg(what).toStdString();
		error(message);
	}
}