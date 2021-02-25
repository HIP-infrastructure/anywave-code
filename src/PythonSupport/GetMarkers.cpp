/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit dAix Marseille (AMU) - 
//                 Institut National de la Sant et de la Recherche Mdicale (INSERM)
//                 Copyright  2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet  Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include <AwProcess.h>
#include <QtMath>
#include <Python.h>
#include <QDataStream>

#include "Marker.h"
//static PyObject *parse_cfg(PyObject *cfg);
static PyObject *request_markers(const QString& file, const QString& extractTriggers, QVector<float>& values, QStringList& labels, QStringList& channels);
extern PyObject *AnyWaveError;

PyObject *request_markers(const QString& file, const QString& extractTriggers, QVector<float>& values, QStringList& labels, QStringList& channels)
{
	TCPRequest request(AwRequest::GetMarkers2);
	if (request.status() == TCPRequest::failed) {
		return NULL;
	}
	QDataStream& stream_data = *request.stream(); 
	QDataStream& in = *request.response();


	stream_data << file << extractTriggers << values << labels << channels;
	if (!request.sendSimpleRequest()) {
		return NULL;
	}
	if (!request.getResponse()) {
		return NULL;
	}

    int nMarkers;
    in >> nMarkers;

	if (nMarkers == 0)
		return Py_None;

	PyObject *pyMarkers = PyList_New(nMarkers);
	for (int i = 0; i < nMarkers; i++) {
		QString label;
		float start, duration;
		float value;
		QStringList channels;
		bool targetChannels;
		float *tmp = NULL;
		qint16 *tmpi = NULL;

		in >> label >> start >> duration >> value >> channels;
		PyObject *py_marker = PyObject_CallObject((PyObject *)&anywave_MarkerType, NULL);

		PyObject_SetAttrString(py_marker, "label", PyUnicode_FromString(label.toStdString().c_str()));
		PyObject_SetAttrString(py_marker, "position", PyFloat_FromDouble(start));
		PyObject_SetAttrString(py_marker, "duration", PyFloat_FromDouble(duration));
		PyObject_SetAttrString(py_marker, "value", PyFloat_FromDouble(value));
		if (!channels.isEmpty()) {
			Py_ssize_t count = 0;
			PyObject *list = PyList_New(channels.size());
			foreach (QString channel, channels) {
				PyList_SetItem(list, count++, PyUnicode_FromString(channel.toStdString().c_str()));
			}
			PyObject_SetAttrString(py_marker, "channels", list);
		}
		PyList_SetItem(pyMarkers, i, py_marker);
	}
	return pyMarkers;
}

PyObject *getMarkers(PyObject *self, PyObject *args)
{
	QString file, extractTriggers;
	QVector<float> values;
	QStringList labels, channels;
	PyObject *dict = args;

	if (!PyDict_Check(dict)) {
		PyErr_SetString(AnyWaveError, "incorrect argument.");
		return NULL;
	}

	// get keys
	PyObject *keys = PyDict_Keys(dict);
	Py_ssize_t size = PyList_Size(keys);
	QStringList keys_;
	QList<PyObject *> values_;

	// browse keys
	Py_ssize_t i;
	for (i = 0; i < size; i++) {
		PyObject *key_ = PyList_GetItem(keys, i);
		keys_ << Py3StringToQString(key_).toLower();
		values_ << PyDict_GetItem(dict, key_);
	}

	int index;

	// browse for file
	index = keys_.indexOf("file");
	if (index != -1) {
		PyObject *file_ = values_.at(index);
		if (!PyUnicode_Check(file_)) {
			PyErr_SetString(AnyWaveError, "Specify file as a string.");
			return NULL;
		}
		file = Py3StringToQString(file_);
	}
	
	index = keys_.indexOf("extracttriggers");
	if (index != -1) {
		PyObject *extract_ = values_.at(index);
		if (!PyUnicode_Check(extract_)) {
			PyErr_SetString(AnyWaveError, "Specify ExtractTriggers as a string.");
			return NULL;
		}
		extractTriggers = Py3StringToQString(extract_);
	}
	// browse for values in dict
	index = keys_.indexOf("values");
	if (index != -1) {
		// check for a PyList
		PyObject *list = values_.at(index);
		if (!PyList_Check(list)) {
			PyErr_SetString(AnyWaveError, "Specify values as a list of float.");
			return NULL;
		}
		Py_ssize_t i, nValues = PyList_Size(list);
		for (i = 0; i < nValues; i++) {
			PyObject *value = PyList_GetItem(list, i);
				values << (float)PyFloat_AsDouble(value);
		}
	}

	index = keys_.indexOf("labels");
	if (index != -1) {
		// check for a PyList
		PyObject *list = values_.at(index);
		if (!PyList_Check(list)) {
			PyErr_SetString(AnyWaveError, "Specify labels as a list of strings.");
			return NULL;
		}
		Py_ssize_t i, nValues = PyList_Size(list);
		for (i = 0; i < nValues; i++) {
			PyObject *value = PyList_GetItem(list, i);
			if (PyUnicode_Check(value))
				labels << Py3StringToQString(value);
		}
	}

	index = keys_.indexOf("channels");
	if (index != -1) {
		// check for a PyList
		PyObject *list = values_.at(index);
		if (!PyList_Check(list)) {
			PyErr_SetString(AnyWaveError, "Specify channels as a list of strings.");
			return NULL;
		}
		Py_ssize_t i, nValues = PyList_Size(list);
		for (i = 0; i < nValues; i++) {
			PyObject *value = PyList_GetItem(list, i);
			if (PyUnicode_Check(value))
				channels << Py3StringToQString(value);
		}
	}


	return request_markers(file, extractTriggers, values, labels, channels);
}