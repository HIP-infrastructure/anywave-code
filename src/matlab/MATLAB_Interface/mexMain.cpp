
#include "mexMain.h"
#include <map>
#include <string>
#include <AwProcess.h>
#include "MATLAB_Interface.h"
#include "TCPRequest.h"
#include <QDataStream>
#ifdef Q_OS_WIN
#include <format>   // only msvc supports C++20 format feature....
#endif

std::map<std::string, int> functions;
using namespace matlab::data;
using namespace matlab::engine;

MexFunction::MexFunction()
{
	//m_matlabPtr = getEngine();
	m_matlabPtr = getEngine();
	functions["debug_connect"] = AwRequest::ConnectDebug;
	functions["get_data"] = AwRequest::GetDataEx;
	functions["get_markers"] = AwRequest::GetMarkersEx;
	functions["send_markers"] = AwRequest::SendMarkers;
	functions["send_message"] = AwRequest::SendMessage;
	functions["send_text"] = AwRequest::SendMessage;
	functions["get_props"] = AwRequest::GetProperties;
	functions["run"] = AwRequest::Run;
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
	typedef std::basic_stringbuf<char16_t> StringBuf;
	std::shared_ptr<StringBuf> output = std::make_shared<StringBuf>();
	std::shared_ptr<StringBuf> output_error = std::make_shared<StringBuf>();

	if (command == "init") {
		// expect argument 1 to be a cell array
		if (inputs.size() != 2)
			error("anywave: init() requires one argument (varargin).");
		if (inputs[1].getType() != ArrayType::CELL)
			error("anywave: init() requires cell array argument.");
		CellArray cellArray = inputs[1];

		// check if empty cell array => do nothing
		if (cellArray.getDimensions()[1] == 0) {
			// try to get variables already set by the MATLAB support lib (if we are running a matlab script inside a MATLAB engine)
			getPidPort();	
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
			return;
		}
		auto vararginSize = cellArray.getDimensions()[1];
#ifdef Q_OS_WIN
		if (vararginSize < 3)
			error(std::format("anywave: init() cell array must contain at least three items: host, port, pid\n. array dimensions are {},{}", 
				cellArray.getDimensions()[0], cellArray.getDimensions()[1]));
#else
		if (vararginSize < 3) {
			std::string message = "anywave: init() cell array myst contain at least three items: host, port, pid\n array dimensions are ";
			message += std::to_string(cellArray.getDimensions()[0]) + "," + std::to_string(cellArray.getDimensions()[1]);
			error(message);
		}
#endif

		// cell array should contain :  host, port, pid (string)
		CharArray stringHost = cellArray[0][0];
		std::string host = stringHost.toAscii();
		
		CharArray stringPort = cellArray[0][1];
		std::string port = stringPort.toAscii();
		double portValue = std::stod(port);
		CharArray stringPid = cellArray[0][2];
		std::string pid = stringPid.toAscii();
		double pidValue = std::stod(pid);

		if (vararginSize > 3) { // get args
			CharArray args = cellArray[0][3];
			//m_matlabPtr->setVariable(u"args", args, matlab::engine::WorkspaceType::BASE);
			//m_matlabPtr->eval(u"args=jsondecode(args);", output, output_error);
			try {
				auto res = m_matlabPtr->feval(u"jsondecode", 1, std::vector<Array>({ args }));
				outputs[0] = StructArray(res[0]);
			}
			catch (const matlab::engine::MATLABExecutionException& e)
			{
				error(e.what());
			}
		}
	
		// this is for old mex files compatibility
		m_matlabPtr->setVariable(u"host", factory.createCharArray(stringHost.toAscii()), matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->setVariable(u"port", factory.createScalar<double>(portValue), matlab::engine::WorkspaceType::BASE);
		m_matlabPtr->setVariable(u"pid", factory.createScalar<double>(pidValue), matlab::engine::WorkspaceType::BASE);


		MATLAB_Interface::port = static_cast<int>(portValue);
		MATLAB_Interface::pid = static_cast<int>(pidValue);

		if (vararginSize == 3) {  // this is the case where only server port and pid are provided
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
	case AwRequest::Run:
		run(outputs, inputs);
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
		error(what.toStdString());
	}
}

void MexFunction::send_markers(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
{
	ArrayFactory factory;

	//const int MARKERS_AT_ONCE = 1000;
	if (inputs.size() != 2)
		error(std::string("send_markers: requires 1 argument to be a struct array."));
	if (inputs[1].getType() != ArrayType::STRUCT)
		error(std::string("send_markers: argument must be a struct array."));

	// expected fields in struct
	std::vector<std::string> expectedFields = { "label", "position", "duration", "color", "channels", "value" };
	StructArray S = std::move(inputs[1]);

	auto fields = S.getFieldNames();
	std::vector<std::string> fieldNames(fields.begin(), fields.end());

	std::map<std::string, int> map;
	map["label"] = -1;
	map["position"] = -1;
	map["duration"] = -1;
	map["channels"] = -1;
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
		// Here we send ALL the markers at once in the same QByteArray
		// My attempt to send markers by chunks is not working as everytime we send data to AnyWave
		// if will trigger the dataReceived() SLOT and then initiate a new 'command'.
		// TODO: reimplement the way we can communicate with AnyWave using chunks in ONE command
		TCPRequest aw(AwRequest::SendMarkers);
		QByteArray data;
		QDataStream stream(&data, QIODevice::WriteOnly);
		stream.setVersion(QDataStream::Qt_4_4);
		int nMarkers = S.getNumberOfElements();
		size_t counter = 0;
		QString label, colour;

		stream << nMarkers;
		for (auto i = 0; i < nMarkers; i++) {
			QStringList targets;
			float position = 0., duration = 0., value = 0.;

			CharArray field_label = S[i + counter]["label"];
			std::string s = field_label.toAscii();
			label = QString::fromStdString(s);
			stream << label;
			if (map["color"] != -1) {
				CharArray c = S[i + counter]["color"];
				std::string s = c.toAscii();
				colour = QString::fromStdString(s);
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
			if (map["channels"] != -1) {
				if (S[i + counter]["channels"].getType() == ArrayType::CELL) {
					CellArray ctargets = S[i + counter]["channels"];
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
	}
	catch (const QString& what) {
		error(what.toStdString());
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
		StructArray S = factory.createStructArray({ 1, size_t(nMarkers) }, { "label", "position", "duration", "value", "channels" });
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
			S[i]["channels"] = C;
		}
		outputs[0] = S;
	}
	catch (const QString& what) {
		error(what.toStdString());
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
		error(what.toStdString());
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
		m_matlabPtr->feval(u"error",
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
		m_matlabPtr->feval(u"error",
			0,
			std::vector<Array>({ factory.createCharArray(s) }));
	}
	MATLAB_Interface::pid = pid;
	if (!args.isEmpty()) {
		auto res = m_matlabPtr->feval(u"jsondecode", 1, std::vector<Array>({ factory.createCharArray(args.toStdString()) }));
		outputs[0] = StructArray(res[0]);
	}
	// this is for old mex files compatibility
	matlabPtr->setVariable(u"host", factory.createCharArray("127.0.0.1"), matlab::engine::WorkspaceType::BASE);
	matlabPtr->setVariable(u"port", factory.createScalar<double>(MATLAB_Interface::port), matlab::engine::WorkspaceType::BASE);
	matlabPtr->setVariable(u"pid", factory.createScalar<double>(MATLAB_Interface::pid), matlab::engine::WorkspaceType::BASE);
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
		error(what.toStdString());
	}
}

void MexFunction::run(matlab::mex::ArgumentList& outputs, matlab::mex::ArgumentList& inputs)
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
			error("run: bad arguments. Expected struct.");
	}
	int status;
	try {
		QString outputArgs;
		TCPRequest aw(AwRequest::Run);
		aw.sendString(args);
		aw.waitForResponse();
		QDataStream& response = aw.response();
		response >> outputArgs;
		if (outputArgs.isEmpty())
			return;
		auto res = m_matlabPtr->feval(u"jsondecode", 1, std::vector<Array>({ factory.createCharArray(outputArgs.toStdString()) }));
		outputs[0] = StructArray(res[0]);
	}
	catch (const QString& what) {
		error(what.toStdString());
	}
}