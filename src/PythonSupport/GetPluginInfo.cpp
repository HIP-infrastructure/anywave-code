#include "common.h"
#include <AwProcess.h>
#include <Python.h>
#include <QDataStream>
#include <AwChannel.h>
extern PyObject *AnyWaveError;

PyObject *getPluginInfo(PyObject *sef, PyObject *args)
{
	TCPRequest request(AwRequest::GetPluginInfo);
	if (request.status() == TCPRequest::failed) 
		return NULL;
	if (!request.sendRequest()) 
		return NULL;
	if (!request.getResponse())
		return NULL;

	// get the data stream
	QDataStream& response = *request.response();
	QStringList labels, refs, rejected_ics;
	float max_sr, total_dur;
	QString temp_dir, plugin_dir, file, icaFile;

	response >> file >> labels >> refs >> max_sr >> total_dur >> temp_dir >> plugin_dir >> icaFile >> rejected_ics;

	PyObject *out = PyDict_New(); // create a new dict.
	if (out == NULL) {
		PyErr_SetString(AnyWaveError, "Unable to create dict. as output.");
		return NULL;
	}
	
	// FILE
	PyObject *pyFile = PyUnicode_FromString(file.toStdString().c_str());
	PyDict_SetItemString(out, "file", pyFile);

	// ICA FILE
	PyObject *pyIcaFile = PyUnicode_FromString(icaFile.toStdString().c_str());
	PyDict_SetItemString(out, "ica_file", pyIcaFile);
	
	// LABELS
	if (!labels.isEmpty()) {
		PyObject *list_labels = PyList_New((Py_ssize_t)labels.size());
		for (int i = 0; i < labels.size(); i++) {
			PyObject *s = PyUnicode_FromString(labels.at(i).toStdString().c_str());
			PyList_SetItem(list_labels, i, s);
		}
		PyDict_SetItemString(out, "labels", list_labels);
	}

	// REJECTED_ICS
	if (!rejected_ics.isEmpty()) {
		PyObject *list_labels = PyList_New((Py_ssize_t)rejected_ics.size());
		for (int i = 0; i < rejected_ics.size(); i++) {
			PyObject *s = PyUnicode_FromString(rejected_ics.at(i).toStdString().c_str());
			PyList_SetItem(list_labels, i, s);
		}
		PyDict_SetItemString(out, "rejected_ics", list_labels);
	}

	// REFS
	if (!refs.isEmpty()) {
		PyObject *list_refs = PyList_New((Py_ssize_t)refs.size());
		for (int i = 0; i < refs.size(); i++) {
			PyObject *s = PyUnicode_FromString(refs.at(i).toStdString().c_str());
			PyList_SetItem(list_refs, i, s);
		}
		PyDict_SetItemString(out, "refs", list_refs);
	}

	// max sr
	PyObject *sr = PyFloat_FromDouble(max_sr);
	PyDict_SetItemString(out, "max_sr", sr);
	// total dur
	PyObject *dur = PyFloat_FromDouble(total_dur);
	PyDict_SetItemString(out, "total_dur", dur);

	// temp dir
	PyObject *temp = PyUnicode_FromString(temp_dir.toStdString().c_str());
	PyDict_SetItemString(out, "temp_dir", temp);

	// temp dir
	PyObject *plugin = PyUnicode_FromString(plugin_dir.toStdString().c_str());
	PyDict_SetItemString(out, "plugin_dir", plugin);

	return out;
}