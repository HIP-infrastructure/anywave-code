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
#include "common.h"
#include <AwMarker.h>
#include <AwProcess.h>
#include <QDataStream>

static void parse_markers(PyObject *);
static void send_markers(const AwMarkerList& markers);
extern PyObject *AnyWaveError;

void parse_markers(PyObject *list)
{
	Py_ssize_t i, size = PyList_Size(list);
	if (size == 0)
		return;

	AwMarkerList markers;
	for (i = 0; i < size; i++) {
		QString s_label;
        float position, duration;
        float value;
        QStringList channels;
		QString s_color;
		PyObject *item = PyList_GetItem(list, i);

		if (PyObject_HasAttrString(item, "label"))
			s_label = QString(PyString_AsString(PyObject_GetAttrString(item, "label")));
		if (PyObject_HasAttrString(item, "position"))
			position = PyFloat_AsDouble(PyObject_GetAttrString(item, "position"));
		if (PyObject_HasAttrString(item, "duration"))
			duration = PyFloat_AsDouble(PyObject_GetAttrString(item, "duration"));
		if (PyObject_HasAttrString(item, "value"))
			value = PyFloat_AsDouble(PyObject_GetAttrString(item, "value"));
		if (PyObject_HasAttrString(item, "channels")) {
			PyObject *pyChannels = PyObject_GetAttrString(item, "channels");
			Py_ssize_t i, size = PyList_Size(pyChannels);
			if (size) {
				for (i = 0; i < size; i++) {
					PyObject *pyChannel = PyList_GetItem(pyChannels, i);
					channels << QString(PyString_AsString(pyChannel));
				}
			}
		}
		if (PyObject_HasAttrString(item, "color"))
			s_color =  QString(PyString_AsString(PyObject_GetAttrString(item, "color")));

		// create marker
        AwMarker *m = new AwMarker;
        m->setLabel(s_label);
        m->setStart(position);
        m->setDuration(duration);
        m->setValue(value);
		if (!s_color.isEmpty())
			m->setColor(s_color);
        if (!channels.isEmpty()) {
            m->setTargetChannels(channels);
        }
        
        markers << m;
	}
	send_markers(markers);

	while (!markers.isEmpty())
		delete markers.takeFirst();
}

void send_markers(const AwMarkerList& markers)
{
	QTcpSocket *socket = connect();
	if (!socket) 
		return;
	int request = AwRequest::AddMarkers;
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_data.setVersion(QDataStream::Qt_4_4);
	
	stream_data << request;
	stream_data << markers.size();
    for (int i = 0; i < markers.size(); i++)    {
        AwMarker *marker = markers.at(i);
        // add marker
        stream_data << marker->label();
        stream_data << marker->start();
        stream_data << marker->duration();
        stream_data << marker->value();
        stream_data << marker->targetChannels();
		stream_data << marker->color();
    }
	sendRequest(socket, data);
}

PyObject *addMarkers(PyObject *self, PyObject *args)
{
	PyObject *cfg = args;
	
	if (!PyList_Check(cfg)) {
		PyErr_SetString(AnyWaveError, "argument must be a list.");
		return NULL;
	}

	parse_markers(cfg);
	return Py_None;
}