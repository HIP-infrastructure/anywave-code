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
//    Authors: Bruno Colombet, Antoine Ducorps – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//    cenir - antoine.ducorps@umpc.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include <filter/AwFilterSettings.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <AwException.h>
#include <qfile.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
void  AwFilterSettings::set(const QString& type, const QVector<float>& values)
{
	set(AwChannel::stringToType(type), values);
}

void AwFilterSettings::set(int type, const QVector<float>& values)
{
	// check if type is already in hash table
	if (m_hash.contains(type))
		m_hash[type] = values;
	else
		m_hash.insert(type, values);
}

void AwFilterSettings::set(int type, float hp, float lp, float notch)
{
	// check if type is already in hash table
	QVector<float> tmp(3);
	tmp[0] = hp; tmp[1] = lp; tmp[2] = notch;
	if (m_hash.contains(type))
		m_hash[type] = tmp;
	else
		m_hash.insert(type, tmp);
}


void AwFilterSettings::apply(AwChannel *channel)
{
	if (!m_hash.contains(channel->type()))
		return;

	// Check for ICA/Source if the channel is real or virtual: DO NOT FILTER ICA/Source Virtual Channels.
	if (channel->isICA() || channel->isSource())
		if (channel->isVirtual())
			return;

	QVector<float> tmp = m_hash[channel->type()];
	channel->setHighFilter(tmp[0]);
	channel->setLowFilter(tmp[1]);
	channel->setNotch(tmp[2]);
}

void AwFilterSettings::apply(const AwChannelList& channels)
{
	for (auto c : channels)
		apply(c);
}

void AwFilterSettings::save(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		throw AwException(QString("Failed to open %1 for writing.").arg(path), QString("aw::filter::Settings::save"));
		return;
	}
	QJsonObject root;

	for (auto k : m_hash.keys()) {
		QJsonArray array;
		for (auto v : m_hash[k])
			array.append(v);
		root[AwChannel::typeToString(k)] = array;
	}
	QJsonDocument doc(root);
	file.write(doc.toJson());
	file.close();
}

void AwFilterSettings::load(const QString& path)
{
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw AwException(QString("Failed to open %1 for reading.").arg(path), QString("aw::filter::Settings::load"));
		return;
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
	file.close();

	if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
		throw AwException(QString("Json error: %1.").arg(error.errorString()), QString("aw::filter::Settings::load"));
		return;
	}
	m_hash.clear();
	// types that can be filtered:
	QStringList types = { "EEG", "SEEG", "EMG", "MEG", "GRAD", "ICA", "Source" };
	QJsonObject root = doc.object();
	for (auto t : types) {
		if (root.contains(t) && root[t].isArray()) {
			QJsonArray array = root[t].toArray();
			// array must contains three values or less ?
			if (array.size() > 3)
				continue;
			QVector<float> values(3);
			values.fill(0.);
			for (auto i = 0; i < array.size(); i++)
				values[i] = (float)array[i].toDouble();
			m_hash.insert(AwChannel::stringToType(t), values);
		}
	}
}

