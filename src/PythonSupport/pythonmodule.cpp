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
#define PY_ARRAY_UNIQUE_SYMBOL anywave_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"


// globals for the module
PyObject *AnyWaveError;		// error handling object
PyObject *m_host;			// host to connect to
PyObject *m_pid;			// process ID
PyObject *m_server_port;		// server port number
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
	{NULL, NULL, 0, NULL}
};

// Channel and Marker types
extern PyTypeObject channel_Type;
extern PyTypeObject marker_Type;

PyMODINIT_FUNC
initanywave(void)
{
	if (PyType_Ready(&marker_Type) < 0)
		 return;

	if (PyType_Ready(&channel_Type) < 0)
		 return;

	m_module = Py_InitModule("anywave", AnyWaveMethods);
	if (m_module == NULL)
		return;
	import_array();
	Py_INCREF(m_module);
	AnyWaveError = PyErr_NewException("anywave.error", NULL, NULL);
	Py_INCREF(AnyWaveError);
	PyModule_AddObject(m_module, "error", AnyWaveError);
	m_host = Py_BuildValue("s", "127.0.0.1");
	m_pid = Py_BuildValue("i", 0);
	m_server_port = Py_BuildValue("i", 0);
	Py_INCREF(m_host);
	Py_INCREF(m_pid);
	Py_INCREF(m_server_port);
	PyModule_AddObject(m_module, "host", m_host);
	PyModule_AddObject(m_module, "pid", m_pid);
	PyModule_AddObject(m_module, "server_port", m_server_port);
	PyModule_AddObject(m_module, "marker", (PyObject *)&marker_Type);
	PyModule_AddObject(m_module, "channel", (PyObject *)&channel_Type);
}


