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
#pragma once

#include <AwGlobal.h>
#include <QObject>
#include <AwVersion.h>

/// Base classes for all plugins
///

/// AwPluginInstanceBase is the base class to inherit when designing the instance object of a plugin.
/// The idea is that a plugin object is instantiated when loaded by AnyWave but then it will instantiate the real plugin instance through the newInstance() method.

class AwPluginBase;

class AwPluginInstanceBase : public QObject
{
public:
	explicit AwPluginInstanceBase() { m_plugin = nullptr; }

	void setPlugin(AwPluginBase *plugin) { m_plugin = plugin; }
	inline AwPluginBase *plugin() { return m_plugin; }
	inline QString& errorString() { return m_errorString; }

protected:
	AwPluginBase *m_plugin;
	QString m_errorString;		// may contains a usefull message in case of error
};

class AwPluginBase : public QObject
{
public:
	explicit AwPluginBase() { minorVersion = AW_MINOR_VERSION; majorVersion = AW_MAJOR_VERSION; }

// name must be unique and short.
// description must contains a brief description of the plugin
// version is not mandatory 
// category is optional : used to inset the plugin launcher into a specific part of the AnyWave GUI
	QString name, description, version, category;
	int minorVersion, majorVersion;	// set up when building
	inline QString& helpUrl() { return m_helpUrl; }
	QString pluginDir;	// full path to the plugin
protected:
	QString m_helpUrl;  // optional : fill it with a url location to get help about the plugin.
// Category can be:
// File: My menu description => for plugin that only acts on files (conversion or export)
// View: My menu description => for plugin that only display graphics 
// Process:Category: My menu description => for classic process plugin that need to be categorized (for example : Filtering, Correlation, etc.)

};

//#define AW_INSTANTIATE_PLUGIN(P) P* newInstance() { auto r = new P; r->setPlugin(this); return r; }