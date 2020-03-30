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
	explicit AwBatchModelItem(AwProcessPlugin *plugin); 
	explicit AwBatchModelItem(AwBatchModelItem *copy);
	enum Inputs { Directory, Files, None };

	void copy(AwBatchModelItem *source);
	inline QString& pluginName() { return m_plugin->name; }
	QVariantHash& jsonParameters() { return m_args; }
	void setJsonParameters(const QVariantHash& args) { m_args = args; }
	AwProcessPlugin *plugin() { return m_plugin; }
	inline bool isEmpty() { return m_args.isEmpty(); }
	bool checkPluginParams();
	QVariantMap& jsonUi() { return m_jsonUi; }
	QString getFileForInput(const QString& key, int index);
	QStringList getFilesForInput(const QString& key);
	QMap<QString, QStringList>& inputsMap() { return m_inputFilesMap; }
	QVariantHash& jsonDefaults() { return m_jsonDefaults; }
	void addFiles(const QString& key, const QStringList& files) { m_inputFilesMap[key] = files; }
protected:
	QString m_pluginName;		// name of the plugin
	QVariantHash m_args;
	AwProcessPlugin *m_plugin;
	QVariantMap m_jsonUi;
	QVariantHash m_jsonDefaults;
	QMap<QString, QStringList> m_inputFilesMap;	// hold a list of files for a specific key.
};

using AwBatchItems = QList<AwBatchModelItem *>;