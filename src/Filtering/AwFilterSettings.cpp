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

AwFilterSettings::AwFilterSettings(bool noInit)
{
	m_ui = nullptr;
	m_uiDocked = false;
	if (!noInit) {
		// init default filter settings for every type of channels
		for (auto i : AwChannel::intTypes)
			m_filters.insert(i, QVector<float>{0, 0, 0, });
	}
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
		this->m_guiVisibleItems = other.m_guiVisibleItems;

		this->m_filters = other.m_filters;
	}
	return *this;
}

void AwFilterSettings::clear()
{
	m_limits.clear();
	m_hash.clear();
	m_filters.clear();
	for (auto i : AwChannel::intTypes)
		m_filters.insert(i, QVector<float>{0, 0, 0, });
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

void AwFilterSettings::apply(AwChannel *channel) const
{
	if (channel == nullptr)
		return;
	auto filters = m_filters.value(channel->type());
	channel->setHighFilter(filters[0]);
	channel->setLowFilter(filters[1]);
	channel->setNotch(filters[2]);
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
			m_guiVisibleItems << AwChannel::stringToType(t);
		}
	}

     if (m_guiVisibleItems.isEmpty()) {
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
						m_guiVisibleItems << AwChannel::EEG << AwChannel::SEEG;
					}
					else if (k == "MEG") {
						set(AwChannel::MEG, values);
						set(AwChannel::GRAD, values);
						m_guiVisibleItems << AwChannel::MEG << AwChannel::GRAD;
					}
					else if (k == "EMG") {
						set(AwChannel::EMG, values);
						set(AwChannel::ECG, values);
						m_guiVisibleItems << AwChannel::EMG << AwChannel::ECG;
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

void AwFilterSettings::setGuiVisibleItems(const AwChannelList& channels)
{
	m_guiVisibleItems.clear();
	for (auto chan : channels) {
		if (!m_guiVisibleItems.contains(chan->type()))
			m_guiVisibleItems << chan->type();
	}
	updateGUI();
}


void AwFilterSettings::setNewSettings(const AwFilterSettings& settings)
{
	m_hash = settings.m_hash;
	m_limits = settings.m_limits;
	m_filters = settings.m_filters;
	m_guiVisibleItems = settings.m_guiVisibleItems;
	emit settingsChanged(*this);
}