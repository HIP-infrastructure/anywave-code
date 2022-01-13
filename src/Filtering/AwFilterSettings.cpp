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
#include <filter/AwFilterSettings.h>
#include <qjsonobject.h>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <AwException.h>
#include <qfile.h>
#include "AwFilterGUI.h"

//////////////////////////////////////////////////////////////////////////////////////////

AwFilterSettings::AwFilterSettings()
{
	m_ui = nullptr;
	m_uiDocked = false;
}

AwFilterSettings::AwFilterSettings(const AwFilterSettings& settings)
{
	m_hash = settings.m_hash;
	m_limits = settings.m_limits;
	m_filters = settings.m_filters;
	m_ui = nullptr;
}

AwFilterSettings::~AwFilterSettings()
{
	if (m_ui != nullptr && !m_uiDocked) {
		m_ui->close();
		delete m_ui;
		m_ui = nullptr;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
AwFilterSettings& AwFilterSettings::operator=(const AwFilterSettings& other)
{
	if (this != &other) {
		this->m_hash = other.m_hash;
		this->m_limits = other.m_limits;
		this->m_registeredChannelTypes = other.m_registeredChannelTypes;
		this->m_filters = other.m_filters;
	}
	return *this;
}

void AwFilterSettings::clear()
{
	m_limits.clear();
	m_hash.clear();
//	m_filterBounds.clear();
}

void AwFilterSettings::zero()
{
	for (auto k : m_filters.keys()) 
		set(k, 0., 0., 0.);
}


void  AwFilterSettings::set(const QString& type, const QVector<float>& values)
{
	set(AwChannel::stringToType(type), values);
}

void AwFilterSettings::set(int type, const QVector<float>& values)
{
	// check if type is already in hash table
	if (m_filters.contains(type))
		m_filters.remove(type);
	m_filters.insert(type, values);
	registerChannelType(type, AwChannel::typeToString(type));
}

void AwFilterSettings::set(int type, float hp, float lp, float notch)
{
	set(type, { hp, lp, notch });
}

void AwFilterSettings::setLimits(int type, const QVector<float>& values)
{
	// check if type is already in hash table
	if (m_limits.contains(type))
		m_limits.remove(type);
	m_limits.insert(type, values);
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
	if (channel == nullptr)
		return;
	if (!m_filters.contains(channel->type()))
		return;

	// ALLOW FILTERING OF ICA VIRTUAL CHANNELS (January 2022)

	//// Check for ICA/Source if the channel is real or virtual: DO NOT FILTER ICA/Source Virtual Channels.
	//if (channel->isICA() || channel->isSource())
	//	if (channel->isVirtual())
	//		return;

	// ALLOW FILTERING OF ICA VIRTUAL CHANNELS (January 2022) Uncomment previous lines to disable filtering again


	QVector<float> tmp = m_filters.value(channel->type());
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
		emit log(QString("Failed to write %1.").arg(path));
		return;
	}
	QJsonObject root;

	for (auto k : m_filters.keys()) {
		QJsonArray array;
		for (const auto& v : m_filters.value(k))
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
	m_filters.clear();
	// types that can be filtered:
	// all types of channels can be filtered
	auto types = AwChannel::types;
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
			set(t, values);
		}
	}
	if (m_filters.isEmpty()) { // empty hash table means the json format is old/wrong
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
						set(AwChannel::EEG, values);
						set(AwChannel::SEEG, values);
					}
					else if (k == "MEG") {
						set(AwChannel::MEG, values);
						set(AwChannel::GRAD, values);
					}
					else if (k == "EMG") {
						set(AwChannel::EMG, values);
						set(AwChannel::ECG, values);
					}
				}
			}
		}
		return;
	}
	updateGUI();
	emit settingsChanged(*this);
}

void AwFilterSettings::registerChannelType(int type, const QString& name)
{
	if (m_registeredChannelTypes.contains(type))
		m_registeredChannelTypes.remove(type);
	m_registeredChannelTypes.insert(type, name);
}

QWidget *AwFilterSettings::ui()
{
	if (m_ui ==  nullptr) {
		m_ui = new AwFilterGUI(*this);
		m_ui->updateSettings(*this);
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
	m_limits = settings.m_limits;
	m_filters = settings.m_filters;
	m_registeredChannelTypes = settings.m_registeredChannelTypes;
	emit settingsChanged(*this);
}