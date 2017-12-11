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
#include <QTcpSocket>
#include <QDataStream>
//#define NO_IMPORT_ARRAY
//#define PY_ARRAY_UNIQUE_SYMBOL anywave_ARRAY_API
//#define NPY_NO_DEPRECATED_API NPY_1_11_API_VERSION

extern PyObject *m_module;
extern PyObject  *m_pid;
extern int m_pidValue;
extern PyObject *AnyWaveError;

void sendRequest(QTcpSocket *client, const QByteArray& data)
{
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    // always send the pid first then size and data.
    stream_size << m_pidValue;
    stream_size << data.size();
	client->write(size);
	client->write(data);
	client->waitForBytesWritten();   
}

int waitForResponse(QTcpSocket *socket)
{
    while (socket->bytesAvailable() < sizeof(int))
       if (!socket->waitForReadyRead())
			return -1;

    QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_4);
    int status, size;
    in >> status;
    if (status == -1)   // if status is incorrect ends here.
        return status;
    // getting size of data (in bytes)
    while (socket->bytesAvailable() < sizeof(int))
		if (!socket->waitForReadyRead())
			return -1;

    in >> size;

	// update 3.7.2014   Wait for all data to be available
	while (socket->bytesAvailable() < size)
        if (!socket->waitForReadyRead())
			return -1;

    return size;
}

QTcpSocket *connect()
{
	// get module's dict
	PyObject *dict = PyModule_GetDict(m_module);
	char *s_host = "";
	QString host;
	PyObject *pyHost = PyDict_GetItemString(dict, "host");
	if (PyString_Check(pyHost))
		s_host = PyString_AS_STRING(pyHost);
	else {
		PyErr_SetString(AnyWaveError, "host is not a string.");
		return NULL;
	}

	PyObject *pyPid = PyDict_GetItemString(dict, "pid");
	if (pyPid == NULL) {
		PyErr_SetString(AnyWaveError, "no pid in dict.");
		return NULL;
	}
	char *s_pid;
	if (PyString_Check(pyPid))
		s_pid = PyString_AS_STRING(pyPid);

	m_pidValue = QString(s_pid).toInt();

	QTcpSocket *socket = new QTcpSocket();

	host = QString(s_host);
	socket->connectToHost(host, 50222);
	if (!socket->waitForConnected()) {
		QString error = QString("Unable to connect to AnyWave: %1").arg(socket->errorString());
		PyErr_SetString(AnyWaveError, error.toStdString().c_str());
		delete socket;
		return NULL;
	}
	return socket;
}