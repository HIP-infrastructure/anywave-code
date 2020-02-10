/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include <AwProcess.h>
extern PyObject *m_module;			// the module


PyObject *openNewFile(PyObject *self, PyObject *args)
{
	if (!PyDict_Check(args)) {
		PyErr_SetString(AnyWaveError, "parameter must be a dict.");
		return NULL;
	}

	TCPRequest request(AwRequest::OpenNewFile);
	if (request.status() == TCPRequest::failed) {
		PyErr_SetString(AnyWaveError, "Connection to AnyWave failed.");
		return NULL;
	}
	if (!request.sendRequest(dictToJson(args))) {
		PyErr_SetString(AnyWaveError, "Failed to send request to AnyWave.");
		return NULL;
	}
	if (!request.getResponse()) {
		PyErr_SetString(AnyWaveError, "No response received from AnyWave.");
		return NULL;
	}
	QDataStream& out = *request.response();
	// AnyWave returns status and new server_port
	int  status,  port;
	out >> status >> port;
	if (status == -1) { // failed : the file could not be open
		PyErr_SetString(AnyWaveError, "AnyWave could not open the file.");
		return NULL;
	}
	// set the  module global server_port with new value
	if (PyDict_SetItemString(PyModule_GetDict(m_module), "server_port", PyLong_FromLong(port))) {
		PyErr_SetString(AnyWaveError, "Module error: could not set global named 'server_port'.");
		return NULL;
	}
	return Py_None;
}