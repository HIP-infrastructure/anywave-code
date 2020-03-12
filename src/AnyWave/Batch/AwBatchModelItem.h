/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2020 AMU, INSERM
// 
//  This software is free software; you can redistribute it and/or
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
#pragma once

#include <QStringList>
#include <AwProcessInterface.h>

class AwBatchModelItem
{
public:
	explicit AwBatchModelItem(AwProcessPlugin *plugin) { m_plugin = plugin; }

	inline QString& pluginName() { return m_plugin->name; }
	inline QStringList& files() { return m_files; }
	void setFiles(const QStringList& files) { m_files = files; }
	inline QString& jsonParameters() { return m_jsonArgs; }
	void setJsonParameters(const QString& args) { m_jsonArgs = args; }
	AwProcessPlugin *plugin() { return m_plugin; }
	inline bool isEmpty() { return m_jsonArgs.isEmpty(); }
	bool checkPluginParams();
protected:
	QString m_pluginName;		// name of the plugin
	QString m_jsonArgs;			// json string used as arguments to run the process
	QStringList m_files;		// list of file to use as input.
	AwProcessPlugin *m_plugin;
};