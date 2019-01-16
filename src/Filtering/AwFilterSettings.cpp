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
#include "AwFilterGUI.h"


AwFilterBounds::AwFilterBounds(int t, float hp, float lp)
{
	type = t;
	bounds[0] = hp;
	bounds[1] = lp;
}

AwFilterBounds& AwFilterBounds::operator=(const AwFilterBounds& other)
{
	if (this != &other) {
		this->type = other.type;
		this->bounds[0] = other.bounds[0];
		this->bounds[1] = other.bounds[1];
	}
	return *this;
}

//////////////////////////////////////////////////////////////////////////////////////////

AwFilterSettings::AwFilterSettings()
{
	m_ui = NULL;
}

AwFilterSettings::AwFilterSettings(const AwFilterSettings& settings)
{
	m_hash = settings.m_hash;
	m_bounds = settings.m_bounds;
	m_filterBounds = settings.m_filterBounds;
	m_ui = NULL;
}

AwFilterSettings::~AwFilterSettings()
{
	if (m_ui != NULL) {
		m_ui->close();
		delete m_ui;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
AwFilterSettings& AwFilterSettings::operator=(const AwFilterSettings& other)
{
	if (this != &other) {
		this->m_hash = other.m_hash;
		this->m_bounds = other.m_bounds;
		this->m_filterBounds = other.m_filterBounds;
	}
	return *this;
}

void AwFilterSettings::clear()
{
	m_bounds.clear();
	m_hash.clear();
	m_filterBounds.clear();
}

void AwFilterSettings::zero()
{
	for (auto k : m_hash.keys()) 
		set(k, 0., 0., 0.);
}


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

void AwFilterSettings::setBounds(int type, const QVector<float>& values)
{
	// check if type is already in hash table
	if (m_bounds.contains(type))
		m_bounds[type] = values;
	else
		m_bounds.insert(type, values);
}


void AwFilterSettings::setBounds(int type, float hp, float lp)
{
	// check if type is already in hash table
	QVector<float> tmp(2);
	tmp[0] = hp; tmp[1] = lp; 
	if (m_bounds.contains(type))
		m_bounds[type] = tmp;
	else
		m_bounds.insert(type, tmp);
}

void AwFilterSettings::setFilterBounds(int type, const AwFilterBounds& bounds)
{
	setBounds(bounds.type, bounds.bounds[0], bounds.bounds[1]);
	// build a unique key based on the virtual channel and its sources channels types.
	// for example if ICA was computed on MEG channels then generates the key: ICA-MEG
	QString key = QString("%1-%2").arg(AwChannel::typeToString(type)).arg(AwChannel::typeToString(bounds.type));

	if (m_filterBounds.contains(key))
		m_filterBounds.remove(key);
	m_filterBounds.insert(key, bounds);
	updateGUI();
	emit settingsChanged(*this);
}

///
/// returns a list of channels for which filter settings are beyond the bounds
/// returns an empty list if filters are in the bounds.
QList<int> AwFilterSettings::checkForBounds()
{
	QList<int> res;
	if (m_bounds.isEmpty())
		return res;
	for (auto k : m_bounds.keys()) {
		auto values = m_hash[k];
		auto bounds = m_bounds[k];
		// check only for HP and LP values.
		bool ok = values[0] >= bounds[0] && values[0] <= bounds[0] &&
			values[1] >= bounds[1] && values[1] <= bounds[1];
		if (!ok)
			res << k;
	}
	return res;
}


///
/// initWithFile()
/// try to load .flt file associated with the data file.
/// If the file does not exist or loading failed, returns false.
/// returns true if success.
bool AwFilterSettings::initWithFile(const QString& filePath)
{
	QString file = QString("%1.flt").arg(filePath);
	try {
		load(file);
	}
	catch (AwException &e)
	{
		emit log(QString("%1:%2").arg(e.origin()).arg(e.errorString()));
		return false;
	}
	return true;
}

void AwFilterSettings::initWithChannels(const AwChannelList& channels)
{
	// check for type of channels. Do not initiate filtering, just set the channels that might be filtered by the user afterward.

	// get type of channels
	auto types = AwChannel::getTypesAsInt(channels);
	// remove channels we don't want the user can filter (Trigger, Other)
	types.removeAll(AwChannel::Trigger);
	types.removeAll(AwChannel::Other);
	for (int i : types) {
		this->set(i, 0., 0., 0.);
	}
	updateGUI();
}

void AwFilterSettings::apply(AwChannel *channel) const
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

void AwFilterSettings::apply(const AwChannelList& channels) const
{
	for (auto c : channels)
		apply(c);
}

void AwFilterSettings::save(const QString& path)
{
	QFile file(path);
	QString origin("AwFilterSettings::save");
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		throw AwException(QString("Failed to open %1 for writing.").arg(path), origin);
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
	QString origin("AwFilterSettings::load");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		throw AwException(QString("Failed to open %1 for reading.").arg(path), origin);
		return;
	}
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
	file.close();

	if (doc.isNull() || doc.isEmpty() || error.error != QJsonParseError::NoError) {
		throw AwException(QString("Json error: %1.").arg(error.errorString()), origin);
		return;
	}
	m_hash.clear();
	// types that can be filtered:
	QStringList types = { "EEG", "SEEG", "EMG", "MEG", "GRAD", "ICA", "Source", "Reference", "ECG" };
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
	if (m_hash.isEmpty()) { // empty hash table means the json format is old/wrong
		//throw AwException(QString("json format is too old or incompatible."), origin);
		// read old format and convert it.
		QVector<float> values(3);
		values.fill(0.);
		QStringList keys = { "EEG", "MEG", "EMG" };
		for (auto k : keys) {
			if (root.contains(k)) {
				auto obj = root[k].toObject();
				if (!obj.isEmpty()) {
					values[0] = (float)obj["HP"].toDouble();
					values[1] = (float)obj["LP"].toDouble();
					values[2] = (float)obj["Notch"].toDouble();
					if (k == "EEG") {
						m_hash.insert(AwChannel::EEG, values);
						m_hash.insert(AwChannel::SEEG, values);
					}
					else if (k == "MEG") {
						m_hash.insert(AwChannel::MEG, values);
						m_hash.insert(AwChannel::GRAD, values);
					}
					else if (k == "EMG") {
						m_hash.insert(AwChannel::EMG, values);
						m_hash.insert(AwChannel::ECG, values);
					}
				}
			}
		}
		return;
	}
	updateGUI();
	emit settingsChanged(*this);
}

QWidget *AwFilterSettings::ui()
{
	if (m_ui == NULL) {
		m_ui = new AwFilterGUI(*this);
		connect(m_ui, &AwFilterGUI::applyClicked, this, &AwFilterSettings::setNewSettings);
	}
	return m_ui;
}


///
/// SLOTS
///
void AwFilterSettings::updateGUI()
{
	if (m_ui)
		m_ui->updateSettings(*this);
}


void AwFilterSettings::setNewSettings(const AwFilterSettings& settings)
{
	m_hash = settings.m_hash;
	m_bounds = settings.m_bounds;
	emit settingsChanged(*this);
}