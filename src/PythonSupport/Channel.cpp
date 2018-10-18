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
#include "structmember.h"
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL	anywave_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_12_API_VERSION
#include "numpy/arrayobject.h"

typedef struct {
	PyObject_HEAD
	PyObject *label;		// string
	PyObject *ref;			// string
	PyObject *type;			// string
	float sr;
	float lpf, hpf, notch;
	int selected;
	PyObject *data;			// numpy array
} channel;

static void channel_dealloc(channel *self)
{
	Py_XDECREF(self->label);
	Py_XDECREF(self->ref);
	Py_XDECREF(self->type);
	Py_XDECREF(self->data);
	self->ob_type->tp_free((PyObject*)self);
}

static PyObject *channel_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	channel *self = (channel *)type->tp_alloc(type, 0);

	if (self != NULL) {
		self->label = PyString_FromString("");
		if (self->label == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->type = PyString_FromString("");
		if (self->type == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->ref = PyString_FromString("");
		if (self->ref == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->data = NULL;
		self->sr = 0;
		self->lpf = 0.;
		self->hpf = 0.;
		self->notch = 0.;
	}

	return (PyObject *)self;
}

static int channel_init(channel *self, PyObject *args, PyObject *kwds)
{
	PyObject *label = NULL, *ref = NULL, *type = NULL, *data = NULL, *selected = NULL, *tmp;

	static char *kwlist[] = { "label", "ref", "type", "sr", "lpf", "hpf", "notch", "selected", "data", NULL };
	if (!PyArg_ParseTupleAndKeywords(args, kwds, "|SSSffffiO", kwlist,
		&label, &ref, &type, &self->sr, &self->lpf, &self->hpf, &self->notch, &self->selected, &data))
		return -1;

	if (label) {
		tmp = self->label;
		Py_INCREF(label);
		self->label = label;
		Py_XDECREF(tmp);
	}
	if (ref) {
		tmp = self->ref;
		Py_INCREF(ref);
		self->ref = ref;
		Py_XDECREF(tmp);
	}
	if (type) {
		tmp = self->type;
		Py_INCREF(type);
		self->type = type;
		Py_XDECREF(tmp);
	}
	if (data) {
		tmp = self->data;
		Py_INCREF(data);
		self->data = data;
		Py_XDECREF(tmp);
	}
	return 0;
}

static PyMemberDef channel_members[] = {
    {"label", T_OBJECT_EX, offsetof(channel, label), 0,
     "channel's"},
    {"ref", T_OBJECT_EX, offsetof(channel, ref), 0,
     "reference"},
    {"type", T_OBJECT_EX, offsetof(channel, type), 0,
     "channel's type"},
	{"data", T_OBJECT_EX, offsetof(channel, data), 0,
     "channel data"},
    {"sr", T_FLOAT, offsetof(channel, sr), 0,
     "sampling rate"},
	{"lpf", T_FLOAT, offsetof(channel, lpf), 0,
     "low pass filter"},
    {"hpf", T_FLOAT, offsetof(channel, hpf), 0,
     "high pass filter"},
	{"notch", T_FLOAT, offsetof(channel, notch), 0,
	"notch filter"},
	{"selected", T_INT, offsetof(channel, selected), 0,
	"flag for selection"},
    {NULL}  /* Sentinel */
};

static PyObject *channel_getlabel(channel *self, void *closure)
{
	Py_INCREF(self->label);
	return self->label;
}

static int channel_setlabel(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the label");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "label must be a string");
		return -1;
	}
	Py_DECREF(self->label);
	Py_INCREF(value);
	self->label = value;
	return 0;
}

static PyObject *channel_getdata(channel *self, void *closure)
{
	Py_INCREF(self->data);
	return self->data;
}

static int channel_setdata(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		Py_XDECREF(self->data);
		self->data = NULL;
	}

	if (!PyArray_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "data must be a numpy array");
		return -1;
	}
	Py_XDECREF(self->data);
	Py_INCREF(value);
	self->data = value;
	return 0;
}

static PyObject *channel_gettype(channel *self, void *closure)
{
	Py_INCREF(self->type);
	return self->type;
}

static int channel_settype(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the type");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "type must be a string");
		return -1;
	}
	Py_DECREF(self->type);
	Py_INCREF(value);
	self->type = value;
	return 0;
}

static int channel_setref(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the ref");
		return -1;
	}
	if (!PyString_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "ref must be a string");
		return -1;
	}
	Py_DECREF(self->ref);
	Py_INCREF(value);
	self->ref = value;
	return 0;
}

static PyObject *channel_getref(channel *self, void *closure)
{
	Py_INCREF(self->ref);
	return self->ref;
}

static int channel_setsr(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the sampling rate");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "sampling rate must be a real value");
		return -1;
	}
	self->sr = (float)PyFloat_AsDouble(value);
	return 0;
}

static int channel_setselected(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete selected");
		return -1;
	}
	if (!PyInt_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "selected must be an int");
		return -1;
	}
	self->selected = (int)PyInt_AsLong(value);
	return 0;
}

static PyObject *channel_getselected(channel *self, void *closure)
{
	return PyInt_FromLong(self->selected);
}

static PyObject *channel_getsr(channel *self, void *closure)
{
	return PyFloat_FromDouble(self->sr);
}

static int channel_setlpf(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the low pass filter");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "the low pass filter must be a real value");
		return -1;
	}
	self->lpf = (float)PyFloat_AsDouble(value);
	return 0;
}

static PyObject *channel_getlpf(channel *self, void *closure)
{
	return PyFloat_FromDouble(self->lpf);
}

static int channel_sethpf(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the high pass filter");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "the high pass filter must be a real value");
		return -1;
	}
	self->hpf = (float)PyFloat_AsDouble(value);
	return 0;
}

static PyObject *channel_gethpf(channel *self, void *closure)
{
	return PyFloat_FromDouble(self->hpf);
}

static int channel_setnotch(channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the notch filter");
		return -1;
	}
	if (!PyFloat_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "the notch filter must be a real value");
		return -1;
	}
	self->notch = (float)PyFloat_AsDouble(value);
	return 0;
}

static PyObject *channel_getnotch(channel *self, void *closure)
{
	return PyFloat_FromDouble(self->notch);
}

static PyGetSetDef channel_getseters[] = {
	{"label", (getter)channel_getlabel, (setter)channel_setlabel,	"label", NULL},
	{"ref", (getter)channel_getref, (setter)channel_setref,  "reference", NULL},
	{"type", (getter)channel_gettype, (setter)channel_settype, "type", NULL},
	{"data", (getter)channel_getdata, (setter)channel_setdata, "data", NULL},
	{"lpf", (getter)channel_getlpf, (setter)channel_setlpf,	"low pass filter", NULL},
	{"sr", (getter)channel_getsr, (setter)channel_setsr,	"sampling rate", NULL },
	{"hpf", (getter)channel_gethpf, (setter)channel_sethpf, "high pass filter", NULL},
	{"notch", (getter)channel_getnotch, (setter)channel_setnotch,	"notch filter", NULL},
	{NULL}  /* Sentinel */
};

PyTypeObject channel_Type = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "channel",             /*tp_name*/
    sizeof(channel),             /*tp_basicsize*/
    0,                         /*tp_itemsize*/
    (destructor)channel_dealloc, /*tp_dealloc*/
    0,                         /*tp_print*/
    0,                         /*tp_getattr*/
    0,                         /*tp_setattr*/
    0,                         /*tp_compare*/
    0,                         /*tp_repr*/
    0,                         /*tp_as_number*/
    0,                         /*tp_as_sequence*/
    0,                         /*tp_as_mapping*/
    0,                         /*tp_hash */
    0,                         /*tp_call*/
    0,                         /*tp_str*/
    0,                         /*tp_getattro*/
    0,                         /*tp_setattro*/
    0,                         /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /*tp_flags*/
    "Channel objects",           /* tp_doc */
    0,		               /* tp_traverse */
    0,		               /* tp_clear */
    0,		               /* tp_richcompare */
    0,		               /* tp_weaklistoffset */
    0,		               /* tp_iter */
    0,		               /* tp_iternext */
    0,             /* tp_methods */
    channel_members,             /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)channel_init,      /* tp_init */
    0,                         /* tp_alloc */
    channel_new,                 /* tp_new */
};

static PyMethodDef channel_methods[] = {
    {NULL}  /* Sentinel */
};

