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
#include "AwBatchItem.h"
#include <utils/json.h>
#include <AwKeys.h>

AwBatchItem::AwBatchItem(AwProcessPlugin *plugin)
{
	m_plugin = plugin; 
	auto batchSettings = plugin->batchHash();
	auto hash = m_plugin->batchHash();
	if (hash.contains("inputs")) {
		auto inputs = hash.value("inputs").toHash();
		for (auto key : inputs.keys())
			m_inputFilesMap.insert(key, QStringList());
	}
	else {
		m_inputFilesMap.insert("input_file", QStringList());
	}
	// now fill params with defaults values
	auto defaults = batchSettings.value(cl::batch_defaults).toHash();
	auto ui = batchSettings.value(cl::batch_ui).toHash();
	for (auto key : ui.keys()) {
		auto keyValue = ui.value(key).toStringList().last().toLower();
		if (keyValue == "string" || keyValue == "stringlist")
			m_params[key] = defaults.value(key).toString();
		else if (keyValue == "double")
			m_params[key] = defaults.value(key).toDouble();
		else if (keyValue == "int")
			m_params[key] = defaults.value(key).toInt();
		else if (keyValue == "boolean")
			m_params[key] = defaults.value(key).toBool();
		else if (keyValue == "list")
			m_params[key] = defaults.value(key).toStringList().first();
	}
	m_locked = false;
}

AwBatchItem::AwBatchItem(AwBatchItem *copy)
{
	m_plugin = copy->m_plugin;
	m_pluginName = copy->pluginName();
	m_params = copy->m_params;
	m_inputFilesMap = copy->m_inputFilesMap;
	m_locked = copy->m_locked;
}

void AwBatchItem::copy(AwBatchItem *source)
{
	m_plugin = source->m_plugin;
	m_pluginName = source->pluginName();
	m_params = source->m_params;
	m_inputFilesMap = source->m_inputFilesMap;
	m_locked = source->m_locked;
}

//AwBIDSItem *AwBatchItem::getBIDSFileItem(const AwBIDSItems& items, int type)
//{
//	if (items.isEmpty())
//		return nullptr;
//	if (type == -1)
//		return items.first();
//
//	for (auto item : items) {
//		auto itemType = item->data(AwBIDSItem::DataTypeRole).toInt();
//		if (itemType == type)
//			return item;
//	}
//	return nullptr;
//}

//void AwBatchItem::addFilesFromBIDS(AwBIDSItems& items)
//{
//	auto batchSettings = m_plugin->batchHash();
//	auto inputs = batchSettings.value(cl::batch_inputs).toHash();
//	for (auto k : inputs.keys()) {
//		auto value = inputs.value(k).toString();
//		AwBIDSItem *item;
//		int fileItemType = -1;
//		if (value == "eeg file")
//			fileItemType = AwBIDSItem::eeg;
//		else if (value == "meg file")
//			fileItemType = AwBIDSItem::eeg;
//		else if (value == "ieeg file")
//			fileItemType = AwBIDSItem::ieeg;
//		item = getBIDSFileItem(items, fileItemType);
//		if (item) {
//			auto files = m_inputFilesMap.value(k);
//			files << item->data(AwBIDSItem::PathRole).toString();
//			m_inputFilesMap[k] = files;
//			items.removeAll(item);
//		}
//	}
//
//}

QString AwBatchItem::getFileForInput(const QString& key, int index)
{
	auto list = m_inputFilesMap.value(key);
	if (list.isEmpty() || index >= list.size() || index < 0)
		return QString();
	return list.at(index);
}

QStringList AwBatchItem::getFilesForInput(const QString& key)
{
	return m_inputFilesMap.value(key);
}

bool AwBatchItem::checkPluginParams()
{
	auto instance = m_plugin->newInstance();
	QString error;
	auto res = instance->batchParameterCheck(this->m_params);
	m_plugin->deleteInstance(instance);
	return res;
}