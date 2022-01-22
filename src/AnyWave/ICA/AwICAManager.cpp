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
#include "AwICAManager.h"
#include <hdf5/AwHDF5.h>
#include <matlab/AwMATLAB.h>
#include "AwICAComponents.h"
#include <widget/AwMessageBox.h>
#include "Prefs/AwSettings.h"
#include <QFileInfo>

// statics init and definitions
AwICAManager *AwICAManager::m_instance = 0;

AwICAManager *AwICAManager::instance()
{
	if (!m_instance) {
		m_instance = new AwICAManager;
	}
	return m_instance;
}

AwICAManager::AwICAManager(QObject *parent)
	: QObject(parent)
{
//	for (int i = 0; i < AW_CHANNEL_TYPES; i++)
//		m_comps[i] = nullptr;
	m_isFilteringOn = false;	// no ICA filtering by default
}

AwICAManager::~AwICAManager()
{
}

void AwICAManager::closeFile()
{
//	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
//		if (m_comps[i])
//			delete m_comps[i];
//		m_comps[i] = NULL;
//	}
	m_componentsMap.clear();
}

void AwICAManager::turnICAOff()
{
	emit icaComponentsUnloaded();

	m_isFilteringOn = false;
	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	//	if (m_comps[i]) {
	//		delete m_comps[i];
	//		m_comps[i] = NULL;
	//	}
	//}
	m_componentsMap.clear();
}

bool AwICAManager::reject(int type)
{
	if (!m_isFilteringOn)
		return false;

	//if (m_comps[type])
	//	return m_comps[type]->isRejectionNeeded();

	if (m_componentsMap.contains(type)) 
		return m_componentsMap.value(type)->isRejectionNeeded();
	
	return false;
}

void AwICAManager::rejectComponents(int type, const AwChannelList& channels)
{
//	m_comps[type]->buildChannels(channels);

	if (m_componentsMap.contains(type))
		m_componentsMap.value(type)->buildChannels(channels);
}

bool AwICAManager::containsComponents(int type)
{
	if (!m_componentsMap.contains(type))
		return false;
	return m_componentsMap.value(type)->components().size() > 0;

	//if (m_comps[type])
	//	if (m_comps[type]->components().size())
	//		return true;
	//
	//return false;
}


AwICAChannelList AwICAManager::getChannelsOfAllComponents()
{
	AwICAChannelList res;

	const auto& keys = m_componentsMap.keys();
	for (const auto& key : keys)
		res += m_componentsMap.value(key)->components();
	return res;
}

//
// returns the total number of components (MEG, EEG, EMG)
//
int AwICAManager::numberOfComponents()
{
	//int count = 0;
	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	//	if (m_comps[i])
	//		count += m_comps[i]->components().size();
	//}
	//return count;
	const auto& keys = m_componentsMap.keys();
	int count = 0;
	for (const auto& key : keys) 
		count += m_componentsMap.value(key)->components().size();
	return count;
}

int AwICAManager::loadComponents(const QString& icaFile)
{
	int row, col;
	// turn previously ICA components to off
	turnICAOff();
	QString inputFile = icaFile;

	// check for 'old' HDF5 format
	if (inputFile.endsWith(".h5"))
		inputFile = convertToMatlab(inputFile);
	
	if (inputFile.isEmpty()) {
		AwMessageBox::critical(0, tr("ICA Components"), "Failed to convert to .mat");
		return - 1;
	}
	AwMATLABFile file;
	QString modality;
	try {
		file.open(inputFile);
		file.readString("modality", modality);
	}
	catch (AwException &e) {
		AwMessageBox::critical(0, tr("ICA Components"), e.errorString());
		return -1;
	}

	int type = AwChannel::stringToType(modality);
//	AwICAComponents *comp = new AwICAComponents(type);
	if (m_componentsMap.contains(type))
		m_componentsMap.remove(type);
	m_componentsMap.insert(type, QSharedPointer<AwICAComponents>(new AwICAComponents(type)));
	auto comp = m_componentsMap.value(type).get();
	try {
		comp->loadComponents(file);
	}
	catch (const AwException& e) {
		AwMessageBox::critical(0, tr("ICA Components"), e.errorString());
	//	delete comp;
		m_componentsMap.remove(type);
		return -1;
	}
	//if (m_comps[type])
	//	delete m_comps[type];
	//m_comps[type] = comp;
	AwSettings::getInstance()->setValue(aws::ica_file, icaFile);
	emit componentsFiltersLoaded(comp->lpFilter(), comp->hpFilter());
	emit componentsLoaded();
	return 0;
}

QVector<int> AwICAManager::getRejectedComponentsIndexes(int type)
{
	//QVector<int> res;
	//if (m_comps[type]) {
	//	for (auto c : m_comps[type]->components())
	//		if (c->isRejected())
	//			res << c->index();
	//}
	//return res;

	QVector<int> res;
	if (m_componentsMap.contains(type)) {
		for (auto const& c : m_componentsMap.value(type)->components())
			if (c->isRejected())
				res << c->index();
	}
	return res;
}

QMap<int, QVector<int>> AwICAManager::getAllRejectedComponents()
{
	QMap <int, QVector<int>> res;
	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	//	if (m_comps[i]) {
	//		auto r = getRejectedComponentsIndexes(i);
	//		if (!r.isEmpty())
	//			res[i] = r;
	//	}
	//		
	//}
	const auto& keys = m_componentsMap.keys();
	for (const auto& key : keys) {
		auto r = getRejectedComponentsIndexes(key);
		if (r.size())
			res.insert(key, r);
	}

	return res;
}

QStringList AwICAManager::getRejectedLabels() 
{
	QStringList res;
	//for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
	//	if (m_comps[i]) {
	//		for (auto comp : getRejectedComponentsIndexes(i))
	//			res << m_comps[i]->components().value(comp)->name();
	//	}
	//}
	const auto& keys = m_componentsMap.keys();
	for (const auto& key : keys) {
		for (auto const& comp : getRejectedComponentsIndexes(key))
			res << m_componentsMap.value(key)->components().value(comp)->name();
	}
	return res;
}

AwICAComponents *AwICAManager::getComponents(int type)
{
	//return m_comps[type];
	return m_componentsMap.value(type).get();
}

//AwICAComponents **AwICAManager::getAllComponents()
//{
//	return m_comps;
//}

///
/// convert the old HDF5 file to the new .mat format and return the path to the converted matlab file.
/// gain are also converted as HDF5 ICA was computed with MEG values in T and MATLAB ICA uses pT.
QString AwICAManager::convertToMatlab(const QString& fileName)
{
	AwHDF5 file(fileName);
	
	if (file.open(AwHDF5::Read, "/") != AwHDF5::Ok)
		return QString();

	int modality;
	if (file.isGroup("ica/MEG")) {
		file.setGroup("ica/MEG");
		modality = AwChannel::MEG;
	}
	else if (file.isGroup("ica/EEG")) {
		file.setGroup("ica/EEG");
		modality = AwChannel::EEG;
	}
	else if (file.isGroup("ica/EMG")) {
		file.setGroup("ica/EMG");
		modality = AwChannel::EMG;
	}
	else if (file.isGroup("ica/Source")) {
		file.setGroup("ica/Source");
		modality = AwChannel::Source;
	}
	else if (file.isGroup("ica/SEEG")) {
		file.setGroup("ica/SEEG");
		modality = AwChannel::SEEG;
	}

	float lf, hf;
	if (file.readFloatAttr("LPF", &lf) != AwHDF5::Ok)
		return QString();
	if (file.readFloatAttr("HPF", &hf) != AwHDF5::Ok)
		return QString();

	int row, col;
	double *tmp = file.readDoubleMatrix("unmixing", &row, &col);
	if (tmp == NULL)
		return QString();
	// data read are row major => fill a col major matrix
	mat unmix(tmp, col, row);
	unmix = unmix.t();
	if (modality == AwChannel::MEG)
		unmix *= 1e-12;
	tmp = file.readDoubleMatrix("mixing", &row, &col);
	if (tmp == NULL)
		return QString();

	mat mix(tmp, col, row);
	mix = mix.t();
	if (modality == AwChannel::MEG)
		mix *= 1e-12;
	QString labels = file.readString("labels");

	if (labels.isEmpty())
		return QString();

	QStringList names = labels.split(",");
	QFileInfo fi(fileName);
	QString path = fi.absolutePath();


	QString newFile = QString("%1/ica_%2.mat").arg(path).arg(AwChannel::typeToString(modality));
	AwMATLABFile matlabFile;
	try {
		matlabFile.create(newFile);
		matlabFile.writeString(QString("modality"), AwChannel::typeToString(modality));
		matlabFile.writeScalar(QString("lpf"), (double)lf);
		matlabFile.writeScalar(QString("hpf"), (double)hf);
		matlabFile.writeScalar(QString("sr"), (double)-1);
		matlabFile.writeMatrix(QString("mixing"), mix);
		matlabFile.writeMatrix(QString("unmixing"), unmix);
		matlabFile.writeStringCellArray(QString("labels"), names);
	}
	catch (AwException &e)
	{
		return QString();
	}
	return newFile;
}