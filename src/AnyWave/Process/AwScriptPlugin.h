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
#ifndef AWSCRIPTPLUGIN_H
#define AWSCRIPTPLUGIN_H

#include <AwProcessInterface.h>

class AwScriptProcess : public AwProcess
{
	Q_OBJECT
public:
	AwScriptProcess() : AwProcess() { m_isCompiled = false;  m_pid = -1; }
	void setScriptPath(const QString& path) { m_path = path; }
	inline void setPid(int pid) { m_pid = pid; }
	inline int pid() const { return m_pid; }
	void setCompiled(bool flag = true) { m_isCompiled = flag; }
	/** calls to runFromCommandLine redirected to run() : command line run options. **/
	void runFromCommandLine() override { run(); }

	// Map to hold int values associated with text flags name
	static QMap<QString, int> pluginFlags;
protected:
	QString m_path;	// path to plugin executable file (optional)
	bool m_isCompiled; // used for MATLAB compiled plugin
	int m_pid;	
};

class AwScriptPlugin : public AwProcessPlugin
{
	Q_OBJECT
public:
	AwScriptPlugin() : AwProcessPlugin() { type = AwProcessPlugin::Background; m_isCompiled = false;}
	enum Backends { Python, MATLAB};
	void init(const QMap<QString, QString>& map);
	void setScriptPath(const QString& path) { m_path = path; }
	void setPluginDir(const QString& dir) { m_pluginDir = dir; }
	void setPluginBackend(int backend) { m_backend = backend; }
	void initProcess(AwScriptProcess *process);
	inline bool isCompiled() { return m_isCompiled; }
	inline void setAsCompiled(bool f) { m_isCompiled = f; }
	void setInputFlags(int flags) { m_inputFlags = flags; }
protected:
	bool m_isCompiled;
	int m_inputFlags;	// input flags to set when instantiating the process.
	QString m_path;			// path to script or executable file
	QString m_pluginDir;	// path to the directory where the plugin is installed.
	int m_backend;	// either Python or MATLAB
};

#endif // AWSCRIPTPLUGIN_H
