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
#include "common.h"
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qjsondocument.h>

extern PyObject *m_module;
extern PyObject  *m_pid;
extern int m_pidValue;
extern PyObject *AnyWaveError;

static QJsonObject dictToJsonObject(PyObject *dict);

QString Py3StringToQString(PyObject *str)
{
	return QString(PyBytes_AsString(PyUnicode_AsUTF8String(str)));
}

QJsonObject dictToJsonObject(PyObject *dict)
{
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
	QJsonObject json;
	for (int index = 0; index < keys_.size(); index++) {
		auto k = keys_.at(index);
		Py_ssize_t i = 0, nValues = 0;
		PyObject *value = values_.at(index);
		// check for object type.
		if (PyUnicode_Check(value))
			json[k] = Py3StringToQString(value);
		else if (PyBool_Check(value))
			json[k] = bool(PyObject_IsTrue(value));
		else if (PyList_Check(value)) {
			nValues = PyList_Size(value);
			QJsonArray array;
			for (auto i = 0; i < nValues; i++) {
				PyObject *item = PyList_GetItem(value, i);
				if (PyUnicode_Check(item))
					array.append(QJsonValue(Py3StringToQString(item)));
				else if (PyBool_Check(item))
					array.append(QJsonValue(bool(PyObject_IsTrue(item))));
				else if (PyLong_Check(item))
					array.append(QJsonValue(PyLong_AsLong(item)));
				else if (PyFloat_Check(item))
					array.append(QJsonValue(PyFloat_AsDouble(item)));
			}
			json[k] = array;
		}
		else if (PyLong_Check(value))
			json[k] = PyLong_AsLong(value);
		else if (PyFloat_Check(value))
			json[k] = PyFloat_AsDouble(value);
		else if (PyDict_Check(value))
			json[k] = dictToJsonObject(value);
	}
	return json;
}

// parse a whole dictionnary and convert it to JSON format.
QString dictToJson(PyObject *dict)
{
	if (!PyDict_Check(dict)) {
		PyErr_SetString(AnyWaveError, "the argument must be a dictionnary.");
		return QString();
	}
	// convert all keys to json
	QJsonObject json = dictToJsonObject(dict);
	return QString(QJsonDocument(json).toJson());

}

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
	//char *s_host = "";
	QString host = Py3StringToQString(PyDict_GetItemString(dict, "host"));
	//QString pid = Py3StringToQString(PyDict_GetItemString(dict, "pid"));
	QString port = Py3StringToQString(PyDict_GetItemString(dict, "server_port"));
	//if (PyString_Check(pyHost))
	//	s_host = PyString_AS_STRING(pyHost);
	//else {
	//	PyErr_SetString(AnyWaveError, "host is not a string.");
	//	return NULL;
	//}

	//PyObject *pyPid = PyDict_GetItemString(dict, "pid");
	//if (pyPid == NULL) {
	//	PyErr_SetString(AnyWaveError, "no pid in dict.");
	//	return NULL;
	//}
	//PyObject *pyServerPort = PyDict_GetItemString(dict, "server_port");
	//if (pyServerPort == NULL) {
	//	PyErr_SetString(AnyWaveError, "no server port defined in dict.");
	//	return NULL;
	//}
	//char *s_pid, *s_serverPort;
	//if (PyString_Check(pyPid))
	//	s_pid = PyString_AS_STRING(pyPid);
	//if (PyString_Check(pyServerPort))
	//	s_serverPort = PyString_AS_STRING(pyServerPort);

	//m_pidValue = QString(s_pid).toInt();

	QTcpSocket *socket = new QTcpSocket();

	//host = QString(s_host);
	//quint16 port = QString(s_serverPort).toInt();

	//socket->connectToHost(host, 50222);
	socket->connectToHost(host, (quint16)port.toInt());
	if (!socket->waitForConnected()) {
		QString error = QString("Unable to connect to AnyWave: %1").arg(socket->errorString());
		PyErr_SetString(AnyWaveError, error.toStdString().c_str());
		delete socket;
		return NULL;
	}
	return socket;
}

TCPRequest::TCPRequest(int requestID)
{
	m_status = TCPRequest::undefined;
	m_socket = connect();
	m_pid = m_pidValue;
	m_requestID = requestID;
	if (m_socket == NULL) {
		m_status = TCPRequest::failed;
	}
	else {
		m_status = TCPRequest::connected;
	}
	m_streamSize = new QDataStream(&m_size, QIODevice::WriteOnly);
	m_streamSize->setVersion(QDataStream::Qt_4_4);
	m_streamData = new QDataStream(&m_data, QIODevice::WriteOnly);
	m_streamData->setVersion(QDataStream::Qt_4_4);
}

TCPRequest::~TCPRequest()
{
	if (m_socket)
		delete m_socket;
	delete m_streamSize;
	delete m_streamData;
}

void TCPRequest::clear()
{
	m_size.clear();
	m_data.clear();
}

bool TCPRequest::sendRequest()
{
	int dataSize = m_data.size() + sizeof(int); // data size + request ID size
	// always send the pid first then size and data.
	*m_streamSize << m_pid;
	*m_streamSize << dataSize << m_requestID;
	m_socket->write(m_size);
	m_socket->write(m_data);
	return m_socket->waitForBytesWritten();
}

int TCPRequest::getResponse()
{
	return waitForResponse(m_socket);
}
