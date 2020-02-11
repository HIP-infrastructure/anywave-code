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
#include "pythonmodule.h"
#include "Channel.h"
#include "Marker.h"
#define PY_ARRAY_UNIQUE_SYMBOL anywave_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"


// globals for the module
PyObject *AnyWaveError;		// error handling object
PyObject *m_host;			// host to connect to
PyObject *m_pid;			// process ID
PyObject *m_server_port;	// server port number
PyObject *m_module;			// the module

// global variables (not included in Python module)
int m_pidValue = 0;

// method prototypes
extern PyObject *getPluginInfo(PyObject *self, PyObject *args);
extern PyObject *getData(PyObject *self, PyObject *args);
extern PyObject *getDataEx(PyObject *self, PyObject *args);
extern PyObject *getMarkers(PyObject *self, PyObject *args);
extern PyObject *addMarkers(PyObject *self, PyObject *args);
extern PyObject *sendMessage(PyObject *self, PyObject *args);
extern PyObject *openNewFile(PyObject *self, PyObject *args);
extern PyObject *launchAnyWave(PyObject *self, PyObject *args);

static PyMethodDef AnyWaveMethods[] = {
	{"get_data", (PyCFunction)getData,  METH_O,
	"Request data from AnyWave"},
	{"get_dataex", (PyCFunction)getDataEx,  METH_O,
	"Request data from AnyWave" },
	{"add_markers", (PyCFunction)addMarkers, METH_O,
	"Add markers to AnyWave"},
	{"get_markers", (PyCFunction )getMarkers, METH_O,
	"Get markers from AnyWave"},
	{"send_message", (PyCFunction)sendMessage, METH_O,
	"Send a message to AnyWave"},
	{"get_plugininfo", (PyCFunction)getPluginInfo, METH_NOARGS,
	"Get information about the plugin's inputs"},
	{"open_new_file", (PyCFunction)openNewFile, METH_O,
	"Request AnyWave to load a file and connect to the newly loaded file."},
	{"run", (PyCFunction)launchAnyWave, METH_O,
	"Launch a new AnyWave instance with a file and connect to it."},
	{NULL, NULL, 0, NULL}
};

// Python 3 Module defintion
static struct PyModuleDef module = {
	PyModuleDef_HEAD_INIT,
	"anywave",
	"anywave API module",
	-1,
	AnyWaveMethods
};


// Python 3 way of life:
PyMODINIT_FUNC PyInit_anywave(void)
{
	// init Channel and Marker types
	anywave_ChannelType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&anywave_ChannelType) < 0)
		return NULL;
	anywave_MarkerType.tp_new = PyType_GenericNew;
	if (PyType_Ready(&anywave_MarkerType) < 0)
		return NULL;

	m_module = PyModule_Create(&module);
	if (m_module == NULL)
		return NULL;
	import_array();
	
	//Py_INCREF(m_module);
	// add error object to module
	AnyWaveError = PyErr_NewException("anywave.error", NULL, NULL);
	Py_INCREF(AnyWaveError);
	PyModule_AddObject(m_module, "error", AnyWaveError);
	m_host = Py_BuildValue("s", "127.0.0.1");
	m_pid = Py_BuildValue("i", 0);
	m_server_port = Py_BuildValue("i", 0);
	// add host object
	Py_INCREF(m_host);
	PyModule_AddObject(m_module, "host", m_host);
	// add pid object
	Py_INCREF(m_pid);
	PyModule_AddObject(m_module, "pid", m_pid);
	// add server_port object
	Py_INCREF(m_server_port);
	PyModule_AddObject(m_module, "server_port", m_server_port);
	// add channel and marker type objects
	Py_INCREF(&anywave_ChannelType);
	PyModule_AddObject(m_module, "Channel", (PyObject *)&anywave_ChannelType);
	PyModule_AddObject(m_module, "Marker", (PyObject *)&anywave_MarkerType);
	return m_module;
}



