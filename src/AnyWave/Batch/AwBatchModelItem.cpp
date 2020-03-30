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
#include "AwBatchModelItem.h"
#include <utils/json.h>

AwBatchModelItem::AwBatchModelItem(AwProcessPlugin *plugin)
{
	m_plugin = plugin; 
	QString error;
	m_jsonUi = AwUtilities::json::mapFromJsonString(plugin->settings().value(processio::json_ui).toString(), error);
	m_jsonDefaults = AwUtilities::json::hashFromJsonString(plugin->settings().value(processio::json_defaults).toString(), error);
	auto inputs = m_jsonUi.value("input_keys").toStringList();
	for (auto input : inputs) 
		m_inputFilesMap.insert(input, QStringList());	
	// default arguments are json defaults
	m_args = m_jsonDefaults;
}

AwBatchModelItem::AwBatchModelItem(AwBatchModelItem *copy)
{
	m_plugin = copy->m_plugin;
	m_pluginName = copy->pluginName();
	m_args = copy->m_args;
	m_inputFilesMap = copy->m_inputFilesMap;
}

void AwBatchModelItem::copy(AwBatchModelItem *source)
{
	m_plugin = source->m_plugin;
	m_pluginName = source->pluginName();
	m_args = source->m_args;
	m_inputFilesMap = source->m_inputFilesMap;
}

QString AwBatchModelItem::getFileForInput(const QString& key, int index)
{
	auto list = m_inputFilesMap.value(key);
	if (list.isEmpty() || index >= list.size() || index < 0)
		return QString();
	return list.at(index);
}

QStringList AwBatchModelItem::getFilesForInput(const QString& key)
{
	return m_inputFilesMap.value(key);
}

bool AwBatchModelItem::checkPluginParams()
{
	auto instance = m_plugin->newInstance();
	QString error;
	auto res = instance->batchParameterCheck(this->m_args);
	m_plugin->deleteInstance(instance);
	return res;
}