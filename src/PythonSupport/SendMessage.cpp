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
#include "common.h"
#include <process/AwTcpProcessRequest.h>
#include <Python.h>
//#define NO_IMPORT_ARRAY
//#define PY_ARRAY_UNIQUE_SYMBOL anywave_ARRAY_API
//#define NPY_NO_DEPRECATED_API NPY_1_11_API_VERSION
#include <QDataStream>

extern PyObject *AnyWaveError;

PyObject *sendMessage(PyObject *self, PyObject *args)
{
	PyObject *message = NULL;
	if (!PyArg_ParseTuple(args, "S", &message)) {
		PyErr_SetString(AnyWaveError, "argument must be a string.");
		return NULL;
	}

	QString s_message = QString(PyString_AsString(message));

	QTcpSocket *socket = connect();
	int request = AwRequest::SendMessage;
	QByteArray data;
	QDataStream stream_data(&data, QIODevice::WriteOnly);
	stream_data.setVersion(QDataStream::Qt_4_4);
	stream_data << request << s_message;
	sendRequest(socket, data);
	delete socket;
	return Py_None;
}