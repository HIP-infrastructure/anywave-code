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

#define WAIT_TIME_OUT   3000000   // 300s socket time out

static QJsonObject dictToJsonObject(PyObject *dict);

QString Py3StringToQString(PyObject *str)
{
	if (str)
		return QString(PyBytes_AsString(PyUnicode_AsUTF8String(str)));
	return QString();
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
       if (!socket->waitForReadyRead(WAIT_TIME_OUT))
			return -1;

    QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_4);
    int status, size;
    in >> status;
	if (status == -1) {  // if status is incorrect ends here.
		PyErr_SetString(AnyWaveError, "Bad status returned by AnyWave.");
		return status;
	}
    // getting size of data (in bytes)
    while (socket->bytesAvailable() < sizeof(int))
		if (!socket->waitForReadyRead()) {
			PyErr_SetString(AnyWaveError, "Nothing to read from socket.");
			return -1;
		}

    in >> size;

	// update 3.7.2014   Wait for all data to be available
	while (socket->bytesAvailable() < size)
        if (!socket->waitForReadyRead(WAIT_TIME_OUT))
			return -1;

    return size;
}

void TCPRequest::connect()
{
	// get module's dict
	PyObject *dict = PyModule_GetDict(m_module);
	QString host = Py3StringToQString(PyDict_GetItemString(dict, "host"));
	QString pid = Py3StringToQString(PyDict_GetItemString(dict, "pid"));
	QString port = Py3StringToQString(PyDict_GetItemString(dict, "server_port"));
	m_pidValue = pid.toInt();
	m_socket.connectToHost(host, (quint16)port.toInt());
	if (!m_socket.waitForConnected()) {
		m_status = TCPRequest::failed;
		QString error = QString("Unable to connect to AnyWave: %1").arg(m_socket.errorString());
		PyErr_SetString(AnyWaveError, error.toStdString().c_str());
	}
	m_status = TCPRequest::connected;
}

TCPRequest::TCPRequest(int requestID)
{
	m_status = TCPRequest::undefined;
	connect();
	m_pid = m_pidValue;
	m_requestID = requestID;
	m_streamSize = new QDataStream(&m_size, QIODevice::WriteOnly);
	m_streamSize->setVersion(QDataStream::Qt_4_4);
	m_streamData = new QDataStream(&m_data, QIODevice::WriteOnly);
	m_streamData->setVersion(QDataStream::Qt_4_4);
	m_streamResponse = new QDataStream(&m_socket);
	m_streamResponse->setVersion(QDataStream::Qt_4_4);
}

TCPRequest::~TCPRequest()
{
	delete m_streamSize;
	delete m_streamData;
	delete m_streamResponse;
}

void TCPRequest::clear()
{
	m_size.clear();
	m_data.clear();
}

bool TCPRequest::sendRequest(QString& jsonString)
{
	int dataSize = jsonString.size() + sizeof(int); // data size + request ID size
	// always send the pid first then size and data.
	*m_streamSize << m_pid;
	*m_streamSize << dataSize << m_requestID;
	m_socket.write(m_size);
	*m_streamData << jsonString;
	m_socket.write(m_data);
	if (!m_socket.waitForBytesWritten()) {
		PyErr_SetString(AnyWaveError, "Error while sending request to AnyWave.");
		return false;
	}
	return true;
}

int TCPRequest::waitForResponse()
{
	while (m_socket.bytesAvailable() < sizeof(int))
		if (!m_socket.waitForReadyRead(WAIT_TIME_OUT))
			return -1;

	QDataStream in(&m_socket);
	in.setVersion(QDataStream::Qt_4_4);
	int status, size;
	in >> status;
	if (status == -1) {  // if status is incorrect ends here.
		PyErr_SetString(AnyWaveError, "Bad status returned by AnyWave.");
		return status;
	}
	// getting size of data (in bytes)
	while (m_socket.bytesAvailable() < sizeof(int))
		if (!m_socket.waitForReadyRead()) {
			PyErr_SetString(AnyWaveError, "Nothing to read from socket.");
			return -1;
		}

	in >> size;

	// update 3.7.2014   Wait for all data to be available
	while (m_socket.bytesAvailable() < size)
		if (!m_socket.waitForReadyRead(WAIT_TIME_OUT))
			return -1;

	return size;
}

bool TCPRequest::getResponse()
{
	return waitForResponse() >= 0;
}
