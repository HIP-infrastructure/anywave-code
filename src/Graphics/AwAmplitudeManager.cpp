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
#include <AwAmplitudeManager.h>
#include <QSettings>
#include <QTextCodec>
#include <QStringList>
#include <AwChannel.h>
#include <QFile>
#include <QTextStream>


AwAmplitudeManager *AwAmplitudeManager::m_instance = NULL;
AwAmplitudeManager *AwAmplitudeManager::instance()
{
	if (!m_instance)
		m_instance = new AwAmplitudeManager();
	return m_instance;
}

AwAmplitudeManager::AwAmplitudeManager(QObject *parent)
	: QObject(parent)
{
	defaults();
	for (int i = 0; i < AW_CHANNEL_TYPES; i++)
		m_units[i] = QString("%1/cm").arg(AwChannel::unitForType(i));
}


void AwAmplitudeManager::defaults()
{
	// default amplitudes
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		m_amplitudes[i].clear();
		m_amplitude[i] = AwChannel::defaultAmplitudeForType(i);
	}

	float v;
	// The MEG scale goes from 0.1 to 500 pt/cm with step of 0.1 and 1.
	for (v = 0.1; v < 1; v += 0.1) {
		m_amplitudes[AwChannel::MEG] << v;
		m_amplitudes[AwChannel::Reference] << v;
		m_amplitudes[AwChannel::GRAD] << v;
	}
	for (v = 1.0; v < 500; v+= 2.) {
		m_amplitudes[AwChannel::MEG] << v;
		m_amplitudes[AwChannel::Reference] << v;
		m_amplitudes[AwChannel::GRAD] << v;
	}
	m_amplitudes[AwChannel::MEG] << 500;
	m_amplitudes[AwChannel::Reference] << 500;
	m_amplitudes[AwChannel::GRAD] << 500;

	// The other scales go from 0.0001 to 1 with a step of 0.001
	for (v = 0.0001; v < 1.; v += 0.01) {
		m_amplitudes[AwChannel::EEG] << v;
		m_amplitudes[AwChannel::ECoG] << v;
		m_amplitudes[AwChannel::SEEG] << v;
		m_amplitudes[AwChannel::EMG] << v;
		m_amplitudes[AwChannel::ECG] << v;
		m_amplitudes[AwChannel::Other] << v;
		m_amplitudes[AwChannel::Trigger] << v;
		m_amplitudes[AwChannel::ICA] << v;
		m_amplitudes[AwChannel::Source] << v;
	}
	// The other scales continue from 1 to 10, with a step of 1.
	for (v = 1.; v < 10; v += 2.) {
		m_amplitudes[AwChannel::EEG] << v;
		m_amplitudes[AwChannel::ECoG] << v;
		m_amplitudes[AwChannel::SEEG] << v;
		m_amplitudes[AwChannel::EMG] << v;
		m_amplitudes[AwChannel::ECG] << v;
		m_amplitudes[AwChannel::Other] << v;
		m_amplitudes[AwChannel::Trigger] << v;
		m_amplitudes[AwChannel::ICA] << v;
		m_amplitudes[AwChannel::Source] << v;
	}
	//  The other scales continue from 10 to 500, with a step of 10.
	for (v = 10.; v < 500; v += 10) {
		m_amplitudes[AwChannel::EEG] << v;
		m_amplitudes[AwChannel::ECoG] << v;
		m_amplitudes[AwChannel::SEEG] << v;
		m_amplitudes[AwChannel::EMG] << v;
		m_amplitudes[AwChannel::ECG] << v;
		m_amplitudes[AwChannel::Other] << v;
		m_amplitudes[AwChannel::Trigger] << v;
		m_amplitudes[AwChannel::ICA] << v;
		m_amplitudes[AwChannel::Source] << v;
	}
	//  The other scales continue from 500 to 5000, with a step of 100.
	for (v = 500.; v < 5000; v += 100) {
		m_amplitudes[AwChannel::EEG] << v;
		m_amplitudes[AwChannel::ECoG] << v;
		m_amplitudes[AwChannel::SEEG] << v;
		m_amplitudes[AwChannel::EMG] << v;
		m_amplitudes[AwChannel::ECG] << v;
		m_amplitudes[AwChannel::Other] << v;
		m_amplitudes[AwChannel::Trigger] << v;
		m_amplitudes[AwChannel::ICA] << v;
		m_amplitudes[AwChannel::Source] << v;
	}
	m_amplitudes[AwChannel::EEG] << 5000;
	m_amplitudes[AwChannel::SEEG] << 5000;
	m_amplitudes[AwChannel::EMG] << 5000;
	m_amplitudes[AwChannel::ECG] << 5000;
	m_amplitudes[AwChannel::Other] << 5000;
	m_amplitudes[AwChannel::Trigger] << 5000;
	m_amplitudes[AwChannel::ICA] << 5000;
	m_amplitudes[AwChannel::Source] << 5000;
	m_amplitudes[AwChannel::ECoG] << 5000;
}

void AwAmplitudeManager::reset()
{
	//// default amplitudes
	defaults();
	emit amplitudesChanged();
}


void AwAmplitudeManager::save()
{
	QFile file(m_filePath);
	if (!file.open(QIODevice::ReadWrite|QIODevice::Truncate|QIODevice::Text))
		return;
	QTextStream stream(&file);
	stream << "[Levels]" << endl;
	for (int i = 0; i < AW_CHANNEL_TYPES; i++) 
		stream << QString("%1=%2").arg(AwChannel::types.value(i)).arg(m_amplitude[i]) << endl;
	file.close();
}

void AwAmplitudeManager::load()
{
	QSettings settings(m_filePath, QSettings::IniFormat);
	settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
	settings.beginGroup("Levels");
	for (int i = 0; i < AW_CHANNEL_TYPES; i++)  {
		m_amplitude[i] = (float)settings.value(AwChannel::types.value(i)).toDouble();
		// check if amplitudes are present in scales, if not add the value to the scale
		if (m_amplitudes[i].indexOf(m_amplitude[i]) == -1) {
			m_amplitudes[i].append(m_amplitude[i]);
			qSort(m_amplitudes[i]);
		}
	}
	settings.endGroup();
	emit amplitudesChanged();
}

void AwAmplitudeManager::closeFile()
{
	if (!m_filePath.isEmpty())
		save();
	m_filePath = "";
	reset();
}

void AwAmplitudeManager::quit()
{
	closeFile();
}

float AwAmplitudeManager::middleValueForScale(int scale)
{
	int index = -1;
	float res = 0.;

	index = m_amplitudes[scale].size() / 2;
	return m_amplitudes[scale].at(index);
}

void AwAmplitudeManager::insertValueInScale(int type, float value)
{
	int index = m_amplitudes[type].indexOf(value);
	if (index == -1) {
		m_amplitudes[type] << value;
		qSort(m_amplitudes[type]);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////
/// SLOTS


// setAmplide() will set the current amplitude gain for a particular type of channel.
// theses values will be saved when the file or AnyWave is closed
void AwAmplitudeManager::setAmplitude(int type, float value)
{
	m_amplitude[type] = value;
}


void AwAmplitudeManager::setFilename(const QString &path)
{
	// name the amplitude path based on data file path passed as parameter
	m_filePath = path;
	
	if (QFile::exists(m_filePath))
		load();
}