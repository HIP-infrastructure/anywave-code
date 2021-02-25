/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with This software; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
	//void setNameAndDesc(const QString& name, const QString& description);
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
