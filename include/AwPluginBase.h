/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2020 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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
// Category can be:
// File: My menu description => for plugin that only acts on files (conversion or export)
// View: My menu description => for plugin that only display graphics 
// Process:Category: My menu description => for classic process plugin that need to be categorized (for example : Filtering, Correlation, etc.)

};

//#define AW_INSTANTIATE_PLUGIN(P) P* newInstance() { auto r = new P; r->setPlugin(this); return r; }