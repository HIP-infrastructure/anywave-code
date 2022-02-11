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
#ifndef AWMATLABSCRIPTPLUGIN_H
#define AWMATLABSCRIPTPLUGIN_H
#include "AwScriptPlugin.h"
#include <AwMatlabInterface.h>
#include <QProcess>

/// AwMatlabScriptProcess
/// An AwProcess in charge of launching matlab scrip as a process plugin
class AwMatlabScriptProcess : public AwScriptProcess
{
	Q_OBJECT
public:
	AwMatlabScriptProcess() : AwScriptProcess() {}
	void run();
	void setSystemPath(const QString& path) { m_systemPath = path;   }
protected slots:
	void sendOutput();
protected:
	QString m_systemPath;	// used only to set the environment before launchine Compiled plugins
	QProcess *m_process;
};



class AwMatlabScriptPlugin : public AwScriptPlugin
{
	Q_OBJECT

public:
	AwMatlabScriptPlugin() : AwScriptPlugin() { m_backend = AwScriptPlugin::MATLAB; }

	AwMatlabScriptProcess* newInstance();
};
#endif // AWMATLABSCRIPTPLUGIN_H
