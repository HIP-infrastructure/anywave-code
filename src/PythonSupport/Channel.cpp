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
#define PY_SSIZE_T_CLEAN
#include "common.h"
#include "structmember.h"
#define NO_IMPORT_ARRAY
#define PY_ARRAY_UNIQUE_SYMBOL	anywave_ARRAY_API
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include "numpy/arrayobject.h"
#include "Channel.h"



static void Channel_dealloc(Channel *self)
{
	Py_XDECREF(self->label);
	Py_XDECREF(self->ref);
	Py_XDECREF(self->type);
	Py_XDECREF(self->data);
	Py_TYPE(self)->tp_free((PyObject*)self);
}

static PyObject *Channel_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
	Channel *self = (Channel *)type->tp_alloc(type, 0);

	if (self != NULL) {
		self->label = PyUnicode_FromString("");
		if (self->label == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->type = PyUnicode_FromString("");
		if (self->type == NULL) {
			Py_DECREF(self);
			return NULL;
		}
		self->ref = PyUnicode_FromString("");
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

static int Channel_init(Channel *self, PyObject *args, PyObject *kwds)
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

static PyMemberDef Channel_members[] = {
    {"label", T_OBJECT_EX, offsetof(Channel, label), 0,
     "channel's"},
    {"ref", T_OBJECT_EX, offsetof(Channel, ref), 0,
     "reference"},
    {"type", T_OBJECT_EX, offsetof(Channel, type), 0,
     "channel's type"},
	{"data", T_OBJECT_EX, offsetof(Channel, data), 0,
     "channel data"},
    {"sr", T_FLOAT, offsetof(Channel, sr), 0,
     "sampling rate"},
	{"lpf", T_FLOAT, offsetof(Channel, lpf), 0,
     "low pass filter"},
    {"hpf", T_FLOAT, offsetof(Channel, hpf), 0,
     "high pass filter"},
	{"notch", T_FLOAT, offsetof(Channel, notch), 0,
	"notch filter"},
	{"selected", T_INT, offsetof(Channel, selected), 0,
	"flag for selection"},
    {NULL}  /* Sentinel */
};

static PyObject *channel_getlabel(Channel *self, void *closure)
{
	Py_INCREF(self->label);
	return self->label;
}

static int channel_setlabel(Channel *self, PyObject *value, void *closure)
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

static PyObject *channel_getdata(Channel *self, void *closure)
{
	Py_INCREF(self->data);
	return self->data;
}

static int channel_setdata(Channel *self, PyObject *value, void *closure)
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

static PyObject *channel_gettype(Channel *self, void *closure)
{
	Py_INCREF(self->type);
	return self->type;
}

static int channel_settype(Channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the type");
		return -1;
	}
	if (!PyUnicode_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "type must be a string");
		return -1;
	}
	Py_DECREF(self->type);
	Py_INCREF(value);
	self->type = value;
	return 0;
}

static int channel_setref(Channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete the ref");
		return -1;
	}
	if (!PyUnicode_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "ref must be a string");
		return -1;
	}
	Py_DECREF(self->ref);
	Py_INCREF(value);
	self->ref = value;
	return 0;
}

static PyObject *channel_getref(Channel *self, void *closure)
{
	Py_INCREF(self->ref);
	return self->ref;
}

static int channel_setsr(Channel *self, PyObject *value, void *closure)
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

static int channel_setselected(Channel *self, PyObject *value, void *closure)
{
	if (value == NULL) {
		PyErr_SetString(PyExc_TypeError, "Cannot delete selected");
		return -1;
	}
	if (!PyLong_Check(value)) {
		PyErr_SetString(PyExc_TypeError, "selected must be an int");
		return -1;
	}
	self->selected = (int)PyLong_AsLong(value);
	return 0;
}

static PyObject *channel_getselected(Channel *self, void *closure)
{
	return PyLong_FromLong(self->selected);
}

static PyObject *channel_getsr(Channel *self, void *closure)
{
	return PyFloat_FromDouble(self->sr);
}

static int channel_setlpf(Channel *self, PyObject *value, void *closure)
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

static PyObject *channel_getlpf(Channel *self, void *closure)
{
	return PyFloat_FromDouble(self->lpf);
}

static int channel_sethpf(Channel *self, PyObject *value, void *closure)
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

static PyObject *channel_gethpf(Channel *self, void *closure)
{
	return PyFloat_FromDouble(self->hpf);
}

static int channel_setnotch(Channel *self, PyObject *value, void *closure)
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

static PyObject *channel_getnotch(Channel *self, void *closure)
{
	return PyFloat_FromDouble(self->notch);
}

static PyGetSetDef Channel_getseters[] = {
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



static PyMethodDef Channel_methods[] = {
    {NULL}  /* Sentinel */
};



PyTypeObject anywave_ChannelType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"anywave.Channel",				// tp_name
	sizeof(Channel),				// tp_basicsize
	0,								// tp_itemsize
	(destructor)Channel_dealloc,	// tp_dealloc
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
	"anywave Channel Object",		// tp_doc
	0,								// tp_traverse
	0,								// tp_clear
	0,								// tp_richcompare
	0,								// tp_weaklistoffset
	0,								// tp_iter
	0,								// tp_iternext
	Channel_methods,				// tp_methods
	Channel_members,				// tp_members
	Channel_getseters,				// tp_getset
	0,								// tp_base
	0,								// tp_dict
	0,								// tp_descr_get
	0,								// tp_descr_set
	0,								// tp_dictoffset
	(initproc)Channel_init,			// tp_init
	0,								// tp_alloc
	Channel_new,					// tp_new
};

