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
#include <AwProcess.h>
#include "common.h"
#include <QDataStream>
#include <Python.h>
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL	anywave_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"


extern PyObject *AnyWaveError;
extern PyTypeObject channel_Type;

struct filtering {
	float eegLP, eegHP;
	float megLP, megHP;
	float emgLP, emgHP;
	filtering() { eegLP = eegHP = megLP = megHP = emgLP = emgHP = 0.; }
};

//static PyObject *parse_cfg(PyObject *cfg);
static PyObject *request_data(const QString&, const QString&, float, float, const QStringList&, const QStringList&, 
							  int, int, filtering *);

PyObject *request_data(const QString& file, const QString& montage,  float start, float duration, const QStringList& labels, const QStringList& types, 
					   int decimate, int filters, filtering *fo)
{
	 QTcpSocket *socket = connect();
	 if (!socket) 
		 return NULL;

	int request = AwRequest::GetData3;
	QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
	stream_data.setVersion(QDataStream::Qt_4_4);
	stream_data << request;
	stream_data << file;
	stream_data << montage;
	stream_data << start;
	stream_data << duration;  
    stream_data << decimate;   // decimation factor for downsampling of data (if value is 1 => no downsampling)
    stream_data << labels;
	stream_data << types;
	stream_data << filters;		// filtering flag

    if (fo)	{
        stream_data << fo->eegHP;
        stream_data << fo->eegLP;
        stream_data << fo->megHP;
        stream_data << fo->megLP;
        stream_data << fo->emgHP;
        stream_data << fo->emgLP;
	}
	sendRequest(socket, data);
	 // waiting for response
	int dataSize = waitForResponse(socket);  
   	if (dataSize == -1)	{
		PyErr_SetString(AnyWaveError, "Bad status received from AnyWave.");
		delete socket;
		return NULL;
	}  

    // Get response
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_4);
    int nChannels = 0;

	// get response from AnyWave
    in >> nChannels; 

    if (nChannels == 0) { // no channels returned (means that the requested labels did not match existing channels in AnyWave.
        delete socket;
        return Py_None;
    }
   
	// create a list
	PyObject *list = PyList_New(nChannels);
	for (int i = 0; i < nChannels; i++)   {
		dataSize = waitForResponse(socket);
        if (dataSize == -1) {
            PyErr_SetString(AnyWaveError, "Bad status received from AnyWave.");
            delete socket;
            return NULL;
        }

        QString name, ref, type;
        float samplingRate, hpf, lpf;
        qint64 nSamples;
		int selected;
        
        in >> name >> type >> ref >> samplingRate >> hpf >> lpf >> nSamples >> selected;
		PyObject *py_channel = PyObject_CallObject((PyObject *)&channel_Type, NULL);
		PyObject_SetAttrString(py_channel, "label", PyString_FromString(name.toStdString().c_str()));
		PyObject_SetAttrString(py_channel, "ref", PyString_FromString(ref.toStdString().c_str()));
		PyObject_SetAttrString(py_channel, "type", PyString_FromString(type.toStdString().c_str()));
		PyObject_SetAttrString(py_channel, "sr", PyFloat_FromDouble(samplingRate));
		PyObject_SetAttrString(py_channel, "lpf", PyFloat_FromDouble(lpf));
		PyObject_SetAttrString(py_channel, "hpf", PyFloat_FromDouble(hpf));
		
		if (nSamples == 0) { // no data for the channel
			continue;
		}
		else {
			// Create a brand new numpy array
			npy_intp dims[1] = { nSamples };
			float *c_array = new float[nSamples];

			// reading chuncks of data
			bool finished = false;
			qint64 nSamplesRead = 0;
			do {
				// waiting for chunk of data
				dataSize = waitForResponse(socket);
				if (dataSize == -1) {
					//PyErr_SetString(AnyWaveError, "Error while receiving data.");
					delete socket;
					//Py_DECREF(array);
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
			
		} // end else
	} // end for
	return list;
}

PyObject *getData(PyObject *self, PyObject *args)
{
	// default parameters
	float start = 0, duration = -1; // get ALL the data (0 to -1)
	int decimate = 1, filters = 0; // no decimation, no filtering
	filtering *fo = NULL;
	QStringList labels, types; // no labels, no types
	QString file, montage; // no file, no montage

//	if (PyArg_ParseTuple(args, "")) // no arguments => default parameters
//		return  request_data(file, montage, start, duration, labels, types, decimate, filters, fo);

	PyObject *dict = args;
	//if (!PyArg_ParseTuple(args, "O", &dict)) {
	//	PyErr_SetString(AnyWaveError, "incorrect argument.");
	//	return NULL;
	//}
	//if (dict == NULL)
	//	return request_data(file, montage, start, duration, labels, types, decimate, filters, fo);

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
		keys_ << QString(PyString_AS_STRING(key_)).toLower();
		values_ << PyDict_GetItem(dict, key_);
	}
	int index;


	// browse for file
	index = keys_.indexOf("file");
	if (index != -1) {
		PyObject *file_ = values_.at(index);
		if (!PyString_Check(file_)) {
			PyErr_SetString(AnyWaveError, "Specify file as a string.");
			return NULL;
		}
		file = PyString_AS_STRING(file_);
	}

	// browse for montage
	index = keys_.indexOf("montage");
	if (index != -1) {
		PyObject *montage_ = values_.at(index);
		if (!PyString_Check(montage_)) {
			PyErr_SetString(AnyWaveError, "Specify file as a string.");
			return NULL;
		}
		montage = PyString_AS_STRING(montage_);
	}
	// browse for start
	index = keys_.indexOf("start");
	if (index != -1) {
		PyObject *start_ = values_.at(index);
		if (!PyFloat_Check(start_)) {
			PyErr_SetString(AnyWaveError, "Specify start in seconds. (float)");
			return NULL;
		}
		start = (float)PyFloat_AsDouble(start_);
	}
	// browse for duration
	index = keys_.indexOf("duration");
	if (index != -1) {
		PyObject *dur_ = values_.at(index);
		if (!PyFloat_Check(dur_)) {
			PyErr_SetString(AnyWaveError, "Specify duration in seconds. (float)");
			return NULL;
		}
		duration = (float)PyFloat_AsDouble(dur_);
		if (duration <= 0)
			duration = -1;
	}
	// browse for labels
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
			if (PyString_Check(value))
				labels << QString(PyString_AS_STRING(value));
		}
	}
	// browse for types
	index = keys_.indexOf("types");
	if (index != -1) {
		// check for a PyList
		PyObject *list = values_.at(index);
		if (!PyList_Check(list)) {
			PyErr_SetString(AnyWaveError, "Specify types as a list of strings.( ['EEG', 'SEEG'] )");
			return NULL;
		}
		Py_ssize_t i, nValues = PyList_Size(list);
		for (i = 0; i < nValues; i++) {
			PyObject *value = PyList_GetItem(list, i);
			if (PyString_Check(value))
				types << QString(PyString_AS_STRING(value));
		}
	}

	// browse for filtering
	index = keys_.indexOf("filtering");
	bool filtersRequired = false;
	if (index != -1) {
		PyObject *filtering_ = values_.at(index);
		if (!PyString_Check(filtering_)) {
			PyErr_SetString(AnyWaveError, "Specify 'no' or 'yes' for filtering parameter.");
			return NULL;
		}
		QString filter = QString(PyString_AS_STRING(filtering_)).toLower();
		if (filter == "no")
			fo = new filtering;
		else if (filter == "yes") {
			fo = new filtering;
			filtersRequired = true;
		}
		else {
			PyErr_SetString(AnyWaveError, "Specify 'no' or 'yes' for filtering parameter.");
			return NULL;
		}
	}

	// check for filters parameters (in case filtering = 'yes')
	if (filtersRequired) {
		bool filterSet = false;
		index = keys_.indexOf("eeg_lp");
		if (index != -1) {
			PyObject *val_ = values_.at(index);
			if (!PyFloat_Check(val_)) {
				PyErr_SetString(AnyWaveError, "eeg_lp must be a value. (float)");
				return NULL;
			}
			fo->eegLP = (float)PyFloat_AsDouble(val_);
			filterSet = true;
		}
		index = keys_.indexOf("eeg_hp");
		if (index != -1) {
			PyObject *val_ = values_.at(index);
			if (!PyFloat_Check(val_)) {
				PyErr_SetString(AnyWaveError, "eeg_hp must be a value. (float)");
				return NULL;
			}
			fo->eegHP = (float)PyFloat_AsDouble(val_);
			filterSet = true;
		}
		index = keys_.indexOf("meg_lp");
		if (index != -1) {
			PyObject *val_ = values_.at(index);
			if (!PyFloat_Check(val_)) {
				PyErr_SetString(AnyWaveError, "meg_lp must be a value. (float)");
				return NULL;
			}
			fo->megLP = (float)PyFloat_AsDouble(val_);
			filterSet = true;
		}
		index = keys_.indexOf("meg_hp");
		if (index != -1) {
			PyObject *val_ = values_.at(index);
			if (!PyFloat_Check(val_)) {
				PyErr_SetString(AnyWaveError, "meg_hp must be a value. (float)");
				return NULL;
			}
			fo->megHP = (float)PyFloat_AsDouble(val_);
			filterSet = true;
		}

		if (!filterSet) {
			PyErr_SetString(AnyWaveError, "filtering was set to 'yes' but not filters were specified.");
			return NULL;
		}
	}

	PyObject *out = request_data(file, montage, start, duration, labels, types, decimate, filters, fo);
	if (fo)
		delete fo;

	return out;
}