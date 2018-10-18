/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <Python.h>
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL	anywave_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"
#include "common.h"
#include <AwProcess.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

extern PyObject *AnyWaveError;
extern PyTypeObject channel_Type;

PyObject *getDataEx(PyObject *self, PyObject *args)
{
	TCPRequest request(AwRequest::GetDataEx);
	if (request.status() != TCPRequest::connected)
		return NULL;
	QDataStream *stream = request.stream();
	if (PyArg_ParseTuple(args, "")) { // no arguments => default parameters

	}	
	else {
		PyObject *dict = NULL;
		if (!PyArg_ParseTuple(args, "O", &dict)) {
			PyErr_SetString(AnyWaveError, "incorrect argument.");
			return NULL;
		}
		if (!PyDict_Check(dict)) {
			PyErr_SetString(AnyWaveError, "incorrect argument.");
			return NULL;
		}
		*stream << dictToJson(dict);
	}

	if (!request.sendRequest())
		return NULL;

	int dataSize = request.getResponse();
	if (dataSize < 0)
		return NULL;

	// Reading response..
	QDataStream in(request.socket());
	in.setVersion(QDataStream::Qt_4_4);
	int nChannels = 0;
	in >> nChannels;
	if (nChannels == 0) {
		return Py_None;
	}
	// create a list
	PyObject *list = PyList_New(nChannels);
	for (int i = 0; i < nChannels; i++) {
		if (request.getResponse() < 0) {
			PyErr_SetString(AnyWaveError, "Error receiving data.");
			Py_DECREF(list);
			return NULL;
		}
		QString name, ref, type;
		float samplingRate, hpf, lpf, notch;
		qint64 nSamples;
		int selected;
		in >> name >> type >> ref >> samplingRate >> hpf >> lpf >> notch >> nSamples >> selected;
		PyObject *py_channel = PyObject_CallObject((PyObject *)&channel_Type, NULL);
		PyObject_SetAttrString(py_channel, "label", PyString_FromString(name.toStdString().c_str()));
		PyObject_SetAttrString(py_channel, "ref", PyString_FromString(ref.toStdString().c_str()));
		PyObject_SetAttrString(py_channel, "type", PyString_FromString(type.toStdString().c_str()));
		PyObject_SetAttrString(py_channel, "sr", PyFloat_FromDouble(samplingRate));
		PyObject_SetAttrString(py_channel, "lpf", PyFloat_FromDouble(lpf));
		PyObject_SetAttrString(py_channel, "hpf", PyFloat_FromDouble(hpf));
		PyObject_SetAttrString(py_channel, "notch", PyFloat_FromDouble(notch));
		PyObject_SetAttrString(py_channel, "selected", PyInt_FromLong(selected));
		if (nSamples == 0)
			continue;
		else {
			// Create a brand new numpy array
			npy_intp dims[1] = { nSamples };
			float *c_array = new float[nSamples];

			// reading chuncks of data
			bool finished = false;
			qint64 nSamplesRead = 0;
			do {
				// waiting for chunk of data
				if (request.getResponse() < 0) {
					PyErr_SetString(AnyWaveError, "Error while receiving data.");
					Py_DECREF(list);
					return NULL;
				}
				qint64 chunkSize;
				in >> chunkSize;
				if (chunkSize == 0) { // finished receiving data
					finished = true;
				}
				else {
					float dumb;
					for (qint64 j = 0; j < chunkSize; j++) {
						in >> c_array[j + nSamplesRead];
					}
					nSamplesRead += chunkSize;
				}
			} // end do
			while (!finished);
			PyObject *array = PyArray_SimpleNewFromData(1, dims, NPY_FLOAT32, c_array);
			PyObject_SetAttrString(py_channel, "data", array);
			PyList_SetItem(list, i, py_channel);
		}
	}
	return list;
}