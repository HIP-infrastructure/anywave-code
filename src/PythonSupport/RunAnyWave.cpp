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
#include <AwProcess.h>
#include <QProcess>
#include <QTcpServer>


extern PyObject* AnyWaveError;

PyObject *launchAnyWave(PyObject *self, PyObject *args)
{
	if (!PyDict_Check(args)) {
		PyErr_SetString(AnyWaveError, "parameter must be a dict.");
		return NULL;
	}

	// spawn a new instance of AnyWave with arguments:
	// 
	QString path = "AnyWave.exe";
	PyObject *p = PyDict_GetItemString(args, "path");
	if (p) 
		path = Py3StringToQString(p);
	QString file = Py3StringToQString(PyDict_GetItemString(args, "file"));
	QString serverPort;

	QTcpServer server;
	if (server.listen(QHostAddress::Any, 0))
		serverPort = QString("--server_port %1").arg(server.serverPort());


	QStringList arguments = { "--server", serverPort, QString("--input_file %1").arg(file) };

	QProcess process;
	process.setProgram(path);
	process.setArguments(arguments);
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
	process.startDetached(path);
#else
	process.startDetached();
#endif

	return Py_None;
}