/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit dAix Marseille (AMU) - 
//                 Institut National de la Sant et de la Recherche Mdicale (INSERM)
//                 Copyright  2013 AMU, INSERM
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
//    Author: Bruno Colombet  Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include <AwMarker.h>
#include <AwProcess.h>
#include <QDataStream>

static PyObject* parse_markers(PyObject *);
static PyObject* send_markers(const AwMarkerList& markers);
extern PyObject *AnyWaveError;

PyObject* parse_markers(PyObject *list)
{
	Py_ssize_t i, size = PyList_Size(list);
	if (size == 0)
		return Py_None;

	AwMarkerList markers;
	for (i = 0; i < size; i++) {
		QString s_label;
        float position, duration;
        float value;
        QStringList channels;
		QString s_color;
		PyObject *item = PyList_GetItem(list, i);

		if (PyObject_HasAttrString(item, "label"))
			s_label = QString(PyBytes_AsString(PyObject_GetAttrString(item, "label")));
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
					channels << QString(PyBytes_AsString(pyChannel));
				}
			}
		}
		if (PyObject_HasAttrString(item, "color"))
			s_color =  QString(PyBytes_AsString(PyObject_GetAttrString(item, "color")));

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
	auto res = send_markers(markers);
	while (!markers.isEmpty())
		delete markers.takeFirst();
	if (res == NULL)
		return NULL;
	return Py_None;
}

PyObject *send_markers(const AwMarkerList& markers)
{
	TCPRequest request(AwRequest::AddMarkers);
	if (request.status() == TCPRequest::failed) 
		return NULL;
	QDataStream& stream = *request.stream();
	stream << markers.size();
    for (int i = 0; i < markers.size(); i++)    {
        AwMarker *marker = markers.at(i);
        // add marker
        stream << marker->label();
        stream << marker->start();
        stream << marker->duration();
        stream << marker->value();
        stream << marker->targetChannels();
		stream << marker->color();
    }
	if (!request.sendRequest())
	   return NULL;
	return Py_None;
}

PyObject *addMarkers(PyObject *self, PyObject *args)
{
	PyObject *cfg = args;
	
	if (!PyList_Check(cfg)) {
		PyErr_SetString(AnyWaveError, "argument must be a list.");
		return NULL;
	}

	return parse_markers(cfg);
}