#include "common.h"
#include <AwProcess.h>


extern PyObject *AnyWaveError;

PyObject *getProperties(PyObject *sef, PyObject *args)
{
	TCPRequest request(AwRequest::GetProperties);
	if (request.status() == TCPRequest::failed)
		return NULL;

	PyObject *keys = nullptr;

	// args can be a string or a list of strings
	if (PyUnicode_Check(args)) {
		keys = PyTuple_New(1);
		Py_INCREF(args); // PyTuple_SetItem steals the refence, so increment it before calling PyTuple_SetItem
		PyTuple_SetItem(keys, 0, args);
	}
	else if (PyTuple_Check(args))
		keys = args;
	else {
		PyErr_SetString(AnyWaveError, "get_prop() : arguments must be a tuple of strings or a string.");
		return NULL;
	}

	QDataStream& toAw = *request.stream();
	QDataStream& fromAw = *request.response();
	QStringList list;
	QString jsonString;
	for (auto i = 0; i < PyTuple_Size(keys); i++) {
		auto item = PyTuple_GetItem(keys, i);
		if (PyUnicode_Check(item))
			list << Py3StringToQString(item);
	}
	toAw << list;
	if (!request.sendSimpleRequest())
		return NULL;
	if (!request.getResponse())
		return NULL;
	// response should come as a json string
	fromAw >> jsonString;

	// try to import json module and use it
	auto json = PyImport_ImportModule("json");
	if (json == NULL) {
		PyErr_SetString(AnyWaveError, "get_prop() : failed to import json module.");
		return NULL;
	}
	auto loads = PyDict_GetItemString(PyModule_GetDict(json), "loads");
	if (loads == NULL) {
		PyErr_SetString(AnyWaveError, "get_prop() : json module has no method called 'loads'.");
		return NULL;
	}
	Py_INCREF(loads);
	/// PyOBject_CallOBject always want a tuple as arguments!!!!
	auto tuple = PyTuple_New(1);
	PyTuple_SetItem(tuple, 0, QStringToPy3String(jsonString));
	auto result = PyObject_CallObject(loads, tuple);
	Py_DECREF(tuple);
	if (result == NULL) {
		QString message = QString("get_prop() : json.loads() failed. json string was: %1.").arg(jsonString);
		PyErr_SetString(AnyWaveError, message.toStdString().c_str());
		return NULL;
	}
	return result;
}