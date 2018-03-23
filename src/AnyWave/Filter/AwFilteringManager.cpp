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
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////

#include "AwFilteringManager.h"
#include <AwChannel.h>
#include <QFile>
#include <QTextStream>
#include <widget/AwMessageBox.h>

// statics
AwFilteringManager *AwFilteringManager::m_instance = NULL;
AwFilteringManager *AwFilteringManager::instance()
{
	if (!m_instance)
		m_instance = new AwFilteringManager;
	return m_instance;
}


AwFilteringManager::AwFilteringManager(QObject *parent)
: QObject(parent)
{
	reset();
}

void AwFilteringManager::reset()
{
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		m_savedLP[i] = m_hp[i] = m_lp[i] = m_savedHP[i] = -1;
		m_notch[i] = -1;
	}
	m_flags = 0;
	emit filtersChanged();
}

AwFilteringOptions AwFilteringManager::filteringOptions()
{
	AwFilteringOptions fo;
	fo.eegHP = m_hp[AwChannel::EEG];
	fo.eegLP = m_lp[AwChannel::EEG];
	fo.eegNotch = m_notch[AwChannel::EEG];
	fo.megHP = m_hp[AwChannel::MEG];
	fo.megLP = m_lp[AwChannel::MEG];
	fo.megNotch = m_notch[AwChannel::MEG];
	fo.emgHP = m_hp[AwChannel::EMG];
	fo.emgLP = m_lp[AwChannel::EMG];
	fo.emgNotch = m_notch[AwChannel::EMG];
	return fo;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SLOTS

void AwFilteringManager::switchFilters(bool On)
{
	if (m_flags == 0)   // no active filters => do nothing
		return;

	if (On) { // ON
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
			m_lp[i] = m_savedLP[i];
			m_hp[i] = m_savedHP[i];
		}
	}
	else { // filters are off
		for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
			m_lp[i] = m_hp[i] = -1;
		}
	}
	emit filtersChanged();
}

///
/// checkForActiveFilters
/// returns true if any filter if active
/// otherwise returns false
bool AwFilteringManager::checkForActiveFilters()
{
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (m_lp[i] > 0. || m_hp[i] > 0. || m_notch[i] > 0.) 
			return true;
	}
	return false;
}

void AwFilteringManager::setNotch(int type, float notch)
{
	m_notch[type] = notch;
	m_savedNotch[type] = notch;
}

void AwFilteringManager::setSourceSettings(int type, float lp, float hp)
{
	int t;
	if (type == AwChannel::MEG)
		t = 0;
	else if (type == AwChannel::EEG)
		t = 1;
	else
		t = 2;
	m_SourceComputationSettings[t] = QPair<float, float>(lp, hp);
	emit newSourceSettings(type, lp, hp);
}


void AwFilteringManager::setICASettings(int type, float lp, float hp)
{
	int t;
	if (type == AwChannel::MEG)
		t = 0;
	else if (type == AwChannel::EEG)
		t = 1;
	else
		t = 2;
	m_ICAComputationSettings[t] = QPair<float, float>(lp, hp);
	emit newICASettings(type, lp, hp);
}

void AwFilteringManager::setFilter(int type, float LP, float HP)
{
	m_savedLP[type] = m_lp[type] = LP;
	m_savedHP[type] = m_hp[type] = HP;

	if (!checkForActiveFilters())
		emit noActiveFilters();
}

void AwFilteringManager::quit()
{
	closeFile();
}

void AwFilteringManager::closeFile()
{
	if (!m_filePath.isEmpty())
		save();
	m_filePath = "";
	emit filtersReset();
}

void AwFilteringManager::load()
{
	if (m_filePath.isEmpty())
		return;

	QFile file(m_filePath);
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		return;

	QTextStream stream(&file);
	QString line;
	float value;

	m_flags = 0;  // reset flags
	
	// EEG HP
	line = stream.readLine();
	if (!line.isEmpty()) {
		value = (float)line.toDouble();
		// EEG value is also SEEG
		m_hp[AwChannel::EEG] = m_hp[AwChannel::SEEG] = m_savedHP[AwChannel::EEG] = m_savedHP[AwChannel::SEEG] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::EEG_HP;
	}

	// EEG LP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		// EEG value is also SEEG
		m_lp[AwChannel::EEG] = m_lp[AwChannel::SEEG] = m_savedLP[AwChannel::EEG] = m_savedLP[AwChannel::SEEG] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::EEG_LP;
	}

	// EMG HP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		// EMG value is also ECG value
		m_hp[AwChannel::EMG] = m_hp[AwChannel::ECG] = m_savedHP[AwChannel::EMG] = m_savedHP[AwChannel::ECG] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::EMG_HP;
	}

	// EMG LP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		// EMG value is also ECG value
		m_lp[AwChannel::EMG] = m_lp[AwChannel::ECG] = m_savedLP[AwChannel::EMG] = m_savedLP[AwChannel::ECG] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::EMG_LP;
	}

	// MEG HP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		m_hp[AwChannel::MEG] = m_savedHP[AwChannel::MEG] = m_hp[AwChannel::GRAD] = m_savedHP[AwChannel::GRAD] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::MEG_HP;
	}

	// MEG LP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		m_lp[AwChannel::MEG] = m_savedLP[AwChannel::MEG] = m_lp[AwChannel::GRAD] = m_savedLP[AwChannel::GRAD]= value;
		if (value > 0)
			m_flags |= AwFilteringManager::MEG_LP;
	}

	// ICA HP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		m_hp[AwChannel::ICA] = m_savedHP[AwChannel::ICA] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::ICA_HP;
	}

	// ICA LP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		m_lp[AwChannel::ICA] = m_savedLP[AwChannel::ICA] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::ICA_LP;
	}

	// Source HP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		m_hp[AwChannel::Source] = m_savedHP[AwChannel::Source] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::Source_HP;
	}

	// Source LP
	line = stream.readLine();
	if (!line.isEmpty()) {	
		value = (float)line.toDouble();
		m_lp[AwChannel::Source] = m_savedLP[AwChannel::Source] = value;
		if (value > 0)
			m_flags |= AwFilteringManager::Source_LP;
	}

	// update 
	file.close();
	emit filtersChanged();

	if (m_flags == 0)
		emit noActiveFilters();
}

void AwFilteringManager::save()
{
	if (m_filePath.isEmpty())
		return;

	QFile file(m_filePath);
	if (!file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Truncate))
		return;

	QTextStream stream(&file);
	stream << QString::number(m_hp[AwChannel::EEG]) << endl;
	stream << QString::number(m_lp[AwChannel::EEG]) << endl;
	stream << QString::number(m_hp[AwChannel::EMG]) << endl;
	stream << QString::number(m_lp[AwChannel::EMG]) << endl;
	stream << QString::number(m_hp[AwChannel::MEG]) << endl;
	stream << QString::number(m_lp[AwChannel::MEG]) << endl;
	stream << QString::number(m_hp[AwChannel::ICA]) << endl;
	stream << QString::number(m_lp[AwChannel::ICA]) << endl;
	stream << QString::number(m_hp[AwChannel::Source]) << endl;
	stream << QString::number(m_lp[AwChannel::Source]) << endl;
	file.close();
}


void AwFilteringManager::setFilename(const QString& path)
{
	m_filePath = path + ".flt";
	if (QFile::exists(m_filePath))
		load();
	else 
		reset();
}
