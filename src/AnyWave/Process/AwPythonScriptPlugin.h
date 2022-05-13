// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#ifndef AWPYTHONSCRIPTPLUGIN_H
#define AWPYTHONSCRIPTPLUGIN_H
#include "AwScriptPlugin.h"
#include <QProcess>

namespace aw {
	namespace python_plugin
	{
		constexpr auto venv = "venv";
	}
}

/// AwPythonScriptProcess
/// An AwProcess in charge of launching a Python script as a process plugin
class AwPythonScriptProcess : public AwScriptProcess
{
	Q_OBJECT
public:
	AwPythonScriptProcess();
	~AwPythonScriptProcess();

	void run() override;
public slots:
	void pythonOutput();
	void pythonError();
	void error(QProcess::ProcessError error);
protected:
	QProcess *m_python;
};


class AwPythonScriptPlugin : public AwScriptPlugin
{
	Q_OBJECT

public:
	AwPythonScriptPlugin() : AwScriptPlugin() { m_backend = AwScriptPlugin::Python; }
	AwPythonScriptProcess *newInstance();
};


#endif // AWPYTHONSCRIPTPLUGIN_H
