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
#include "IO/BIDS/AwBIDSItem.h"

class AwBatchItem
{
public:
	explicit AwBatchItem(AwProcessPlugin *plugin);
	explicit AwBatchItem(AwBatchItem *copy);
	enum Inputs { Directory, Files, None };

	void copy(AwBatchItem *source);
	inline QString& pluginName() { return m_plugin->name; }
	QVariantHash& params() { return m_params; }
	QVariantHash& pluginBatchSettings() { return m_plugin->batchHash(); }
	void setParams(const QVariantHash& args) { m_params = args; }
	AwProcessPlugin *plugin() { return m_plugin; }
//	inline bool isEmpty() { return m_settings.isEmpty(); }
	bool checkPluginParams();
	QString getFileForInput(const QString& key, int index);
	QStringList getFilesForInput(const QString& key);
	QMap<QString, QStringList>& inputsMap() { return m_inputFilesMap; }
	void addFiles(const QString& key, const QStringList& files) { m_inputFilesMap[key] = files; }
	void addFilesFromBIDS(AwBIDSItems& items);
	AwBIDSItem *getBIDSFileItem(const AwBIDSItems& item, int type);
	QStringList getFiles(const QString& key) { return m_inputFilesMap.value(key); }
protected:
	QString m_pluginName;		// name of the plugin
	QVariantHash m_params;	    // current parameters
	AwProcessPlugin *m_plugin;
	QMap<QString, QStringList> m_inputFilesMap;	// hold a list of files for a specific key.
};

using AwBatchItems = QList<AwBatchItem *>;