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
#include "structmember.h"
#include "Marker.h"


static void Marker_dealloc(Marker *self)
{
	Py_XDECREF(self->label);
	Py_XDECREF(self->color);
	Py_XDECREF(self->channels);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *Marker_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Marker *self = (Marker *)type->tp_alloc(type, 0);

	if (self != NULL) {
		self->label = PyUnicode_FromString("");
		if (self->label == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->color = PyUnicode_FromString("");
		if (self->color == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->value = 0.;
		self->position = 0.;
		self->duration = 0.;
	}

	return (PyObject *)self;
}

static int Marker_init(Marker *self, PyObject *args, PyObject *kwds)
{
	PyObject *label = NULL, *channels = NULL, *color = NULL, *tmp;

	static char *kwlist[] = { "label", "color", "channels", "value", "position", "duration", NULL };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|SSOfff", kwlist,
		&label, &color, &channels, &self->value, &self->position, &self->duration))
		return -1;

	if (label) {
		tmp = self->label;
		Py_INCREF(label);
		self->label = label;
		Py_XDECREF(tmp);
	}
	if (color) {
		tmp = self->color;
		Py_INCREF(color);
		self->color = color;
		Py_XDECREF(tmp);
	}
	if (channels) {
		tmp = self->channels;
		Py_INCREF(channels);
		self->channels = channels;
		Py_XDECREF(tmp);
	}
	return 0;
}

static PyMemberDef Marker_members[] = {
    {"label", T_OBJECT_EX, offsetof(Marker, label), 0,
     "marker's label"},
    {"value", T_FLOAT, offsetof(Marker, value), 0,
     "marker's value"},
    {"position", T_FLOAT, offsetof(Marker, position), 0,
     "marker's position"},
    {"duration", T_FLOAT, offsetof(Marker, duration), 0,
     "marker's duration"},
	{"color", T_OBJECT_EX, offsetof(Marker, color), 0,
     "marker's color"},
    {"channels", T_OBJECT_EX, offsetof(Marker, channels), 0,
     "targeted channels"},
    {NULL}  /* Sentinel */
};

static PyObject *marker_getcolor(Marker *self, void *closure)
{
	Py_INCREF(self->color);
	return self->color;
}

static int marker_setcolor(Marker *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the color");
		return -1;
	}
	if (!PyUnicode_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "color must be a string");
		return -1;
	}
	Py_DECREF(self->color);
	Py_INCREF(value);
	self->color = value;
	return 0;
}


static PyObject *marker_getlabel(Marker *self, void *closure)
{
	Py_INCREF(self->label);
	return self->label;
}

static int marker_setlabel(Marker *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the label");
		return -1;
	}
	if (!PyUnicode_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "label must be a string");
		return -1;
	}
	Py_DECREF(self->label);
	Py_INCREF(value);
	self->label = value;
	return 0;
}

static int marker_setvalue(Marker *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the value");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "value must be a float");
		return -1;
	}
	self->value = (float)PyFloat_AsDouble(value);
	return 0;
}

static PyObject *marker_getvalue(Marker *self, void *closure)
{
	return PyFloat_FromDouble(self->value);
}

static int marker_setpos(Marker *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the position");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "position must be a real value");
		return -1;
	}
	self->position = (float)PyFloat_AsDouble(value);
	return 0;
}

static PyObject *marker_getpos(Marker *self, void *closure)
{
	return PyFloat_FromDouble(self->position);
}

static int marker_setdur(Marker *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the duration");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "value must be a real value");
		return -1;
	}
	self->duration = (float)PyFloat_AsDouble(value);
	return 0;
}

static PyObject *marker_getdur(Marker *self, void *closure)
{
	return PyFloat_FromDouble(self->duration);
}

static int marker_setchannels(Marker *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the duration");
		return -1;
	}
	if (!PyList_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "channels must be a list of strings");
		return -1;
	}
	Py_XDECREF(self->channels);
	Py_INCREF(value);
	self->channels = value;
	return 0;
}

static PyObject *marker_getchannels(Marker *self, void *closure)
{
	Py_INCREF(self->channels);
	return self->channels;
}

static PyGetSetDef Marker_getseters[] = {
	{"label", (getter)marker_getlabel, (setter)marker_setlabel,	"label", NULL},
	{"value", (getter)marker_getvalue, (setter)marker_setvalue,  "value", NULL},
	{"position", (getter)marker_getpos, (setter)marker_setpos, "position", NULL},
	{"duration", (getter)marker_getdur, (setter)marker_setdur,	"duration", NULL},
	{"channels", (getter)marker_getchannels, (setter)marker_setchannels,	"channels", NULL},
	{"color", (getter)marker_getcolor, (setter)marker_setcolor,	"color", NULL},
	{NULL}  /* Sentinel */
};


static PyMethodDef Marker_methods[] = {
    {NULL}  /* Sentinel */
};


// do not make it static as it will be declared as extern to be used in other .cpp files.

PyTypeObject anywave_MarkerType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"anywave.Marker",				// tp_name
	sizeof(Marker),  				// tp_basicsize
	0,								// tp_itemsize
	(destructor)Marker_dealloc,		// tp_dealloc
	0,								// tp_print
	0,								// tp_getattr
	0,								// tp_setattr
	0,								// tp_reserved
	0,								// tp_repr
	0,								// tp_as_number
	0,								// tp_as_sequence
	0,								// tp_as_mapping
	0,								// tp_hash
	0,								// tp_call
	0,								// tp_str
	0,								// tp_getattro
	0,								// tp_setattro
	0,								// tp_as_buffer
	Py_TPFLAGS_DEFAULT |
		Py_TPFLAGS_BASETYPE,		// tp_flags
	"anywave Marker Object",		// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	Marker_methods,					// tp_methods
	Marker_members,					// tp_members
	Marker_getseters,				// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)Marker_init,			// tp_init
	0,								// tp_alloc
	Marker_new,						// tp_new
};

