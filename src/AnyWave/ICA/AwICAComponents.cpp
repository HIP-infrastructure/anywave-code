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
#include "AwICAComponents.h"
#include <layout/AwLayoutManager.h>
#include "Montage/AwMontageManager.h"
#include "Prefs/AwSettings.h"
#include <QtGlobal>
#ifndef NDEBUG
#include <QDebug>
#endif
#include "AwICAManager.h"
#include "AwICAPanel.h"
#include <filter/AwFiltering.h>
#include <widget/AwWait.h>
#include "AwPanelItem.h"
#include "Data/AwDataManager.h"
#include "AwSEEGMap.h"


AwICAComponents::AwICAComponents(int type, QObject *parent)
	: QObject(parent)
{
	m_type = type;
	m_panel = nullptr;
	m_seegMap = nullptr;
	connect(this, SIGNAL(componentAdded(int)), this, SLOT(switchFilteringOn()));
	connect(this, SIGNAL(componentRejected(int)), this, SLOT(switchFilteringOn()));
	connect(this, SIGNAL(filteringChecked(bool)), AwICAManager::instance(), SLOT(setICAFiletring(bool)));
	connect(&AwDataManager::instance()->filterSettings(), &AwFilterSettings::settingsChanged, this, &AwICAComponents::setNewFilters);
}

AwICAComponents::~AwICAComponents()
{
	if (m_panel) {
		m_panel->close();
		delete m_panel;
	}
	while (!m_sources.isEmpty())
		delete m_sources.takeFirst();
	if (m_seegMap) {
		m_seegMap->close();
		delete m_seegMap;
	}
}


void AwICAComponents::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_sources);
}

//
// Add the components to signals.
// That means remove it from rejected components.
//
void AwICAComponents::addComponent(int index)
{
	m_rejectedComponents.removeAll(index);
	emit componentAdded(index);
	updateRejectedComponents();
}

void AwICAComponents::rejectComponent(int index)
{
	if (!m_rejectedComponents.contains(index)) {
		m_rejectedComponents << index;
		emit componentRejected(index);
		updateRejectedComponents();
	}
}

void AwICAComponents::updateRejectedComponents()
{
	foreach (AwICAChannel *chan, m_icaChannels) {
		if (m_rejectedComponents.contains(chan->index()))
			chan->setRejected(true);
		else
			chan->setRejected(false);
	}
	if (m_panel)
		m_panel->updateRejectedComponents(m_rejectedComponents);
}

//
// multiply the unmixing matrix by the data matrix to get the components.
// Extract components that matches channels.
void AwICAComponents::computeComponents(AwICAChannelList& channels)
{
	if (channels.isEmpty())
		return;

	fmat data(m_sources.size(), m_sources.first()->dataSize());
	for (uword r = 0; r < data.n_rows; r++) {
		for (uword c = 0; c < data.n_cols; c++)
			data(r, c) = m_sources.at(r)->data()[c];
		m_sources.at(r)->clearData();
	}

	fmat components = m_unmixing * data;

	for (auto ica : channels) {
		float *dest = ica->newData(components.n_cols);
		for (uword c = 0; c < components.n_cols; c++)
			dest[c] = components(ica->index(), c);
		ica->setDataReady();
	}
}

void AwICAComponents::buildChannels(const AwChannelList& channels)
{
	QMutexLocker lock(&m_mutex);
	int nComps = m_icaChannels.size();
	fmat diag(nComps, nComps, fill::eye);
	////	 remove ICA components from diag
	for (auto index : m_rejectedComponents)
		diag(index, index) = 0.;

	fmat data(m_sources.size(), m_sources.first()->dataSize());
	for (uword r = 0; r < data.n_rows; r++) {
		for (uword c = 0; c < data.n_cols; c++)
			data(r, c) = m_sources.at(r)->data()[c];
		m_sources.at(r)->clearData();
	}
	fmat FData;
	try {
		FData = m_mixing * diag * m_unmixing * data;
	}
	catch (std::bad_alloc)
	{ 
		return;
	}
	catch (std::runtime_error) {
		return;
	}

	QStringList keys = m_labelToIndex.keys();
	for (auto c : channels) {
		if (!keys.contains(c->name()))
			continue;
		float *dest = c->newData(FData.n_cols);
		int index = m_labelToIndex.value(c->name());
		for (uword c = 0; c < FData.n_cols; c++)
			dest[c] = FData(index, c);
		c->setDataReady();
	}
}

int  AwICAComponents::loadComponents(AwMATLABFile& file)
{
	// close previous panel is exists
	if (m_panel) {
		delete m_panel;
		m_panel = NULL;
	}

	double lpf, hpf;
	mat unmixingD, mixingD;
	try {
		file.readScalar("lpf", &lpf);
		file.readScalar("hpf", &hpf);
		file.readMatrix("unmixing", unmixingD);
		file.readMatrix("mixing", mixingD);
		file.readStrings("labels", m_labels);
	}
	catch (const AwException& e)
	{
		throw AwException(QString("Error importing ICA matrices: %1").arg(e.errorString()), "AwICAComponents::loadComponents");
		return -1;
	}

	// convert matrices to float
	m_unmixing = conv_to<fmat>::from(unmixingD);
	m_mixing = conv_to<fmat>::from(mixingD);
	m_hpFilter = (float)hpf;
	m_lpFilter = (float)lpf;

	AwMontageManager *mm = AwMontageManager::instance();
	// build source channels list
	int index = 0;
	for (auto s : m_labels) {
		// get the corresponding as recorded channel
		auto asRecorded = mm->asRecordedChannel(s);
		if (asRecorded) {
			AwChannel *source = new AwChannel(asRecorded);
			m_sources << source;
			m_labelToIndex.insert(source->name(), index++);
		}
	}

	// force current filter settings to match those store in the ICA file.
	AwDataManager::instance()->filterSettings().set(this->m_type, m_hpFilter, m_lpFilter, 0.);
	AwDataManager::instance()->filterSettings().apply(m_sources);
	auto dm = AwDataManager::instance();
	//float sr = m_sources.at(0)->samplingRate();
	float sr = dm->value(data_info::max_sr).toFloat();
	// get reader 
	auto reader = dm->reader();
	// get layout
	AwLayout *l = NULL, *l3D = NULL;
	if (m_type == AwChannel::MEG) {
		l = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L2D | AwLayout::MEG);
		l3D = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L3D | AwLayout::MEG);
	}
	else {
		l = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L2D | AwLayout::EEG);
		l3D = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L3D | AwLayout::EEG);
	}

	if (l == NULL && m_type == AwChannel::MEG) {
		l = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L2D | AwLayout::MEG);
		l3D = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L3D | AwLayout::MEG);
	}
	else if (l == NULL && m_type == AwChannel::EEG) {
		l = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L2D | AwLayout::EEG);
		l3D = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L3D | AwLayout::EEG);
	}
	// unmixing matrix is ncomp x nchannels.
	// each line is a component topography
	QVector<float> values(m_mixing.n_rows);

	for (int i = 0; i < m_unmixing.n_rows; i++) {
		AwICAChannel *chan = new AwICAChannel();
		chan->setName(QString("%1_ICA_%2").arg(AwChannel::typeToString(m_type)).arg(i + 1));
		chan->setSamplingRate(sr);
		chan->setIndex(i);
		chan->setLayout2D(l);
		if (l3D)
			chan->setLayout3D(l3D);
		chan->setComponentType(m_type);
		// topography values for component i are in the ith column of matrix mixing
		for (uword r = 0; r < m_mixing.n_rows; r++)
			values[r] = m_mixing(r, i);
		// topography values for component i are in the ith column of matrix mixing
		chan->setTopoValues(values);
		chan->setLabels(m_labels);	
		chan->setDisplayPluginName("ICA SignalItem");
		m_icaChannels << chan;
	}
	return 0;
}

int AwICAComponents::loadComponents(AwHDF5& file)
{
	AwWait wait;
	// close previous panel is exists
	if (m_panel) {
		m_panel->close();
		delete m_panel;
		m_panel = nullptr;
	}

	int row, col;
	if (file.readFloatAttr("LPF", &m_lpFilter) != AwHDF5::Ok)
		return -1;
	if (file.readFloatAttr("HPF", &m_hpFilter) != AwHDF5::Ok)
		return -1;

	double *tmp = file.readDoubleMatrix("unmixing", &row, &col);
	if (tmp == NULL)
		return -1;

	// data read are row major => fill a col major matrix
	mat unmix(tmp, col, row);
	unmix = unmix.t();
	m_unmixing = conv_to<fmat>::from(unmix);
		
	tmp = file.readDoubleMatrix("mixing", &row, &col);
	if (tmp == NULL)
		return -1;

	mat mix(tmp, col, row);
	mix = mix.t();
	m_mixing = conv_to<fmat>::from(mix);

	QString labels = file.readString("labels");

	if (labels.isEmpty())
		return AwHDF5::ReadError;

	m_labels = labels.split(",");

	AwMontageManager *mm = AwMontageManager::instance();
	
	// build source channels list
	int index = 0;
	foreach (QString s, m_labels)	{
		// get the corresponding as recorded channel
		AwChannel *asRecorded = mm->asRecordedChannel(s);
		if (asRecorded)		{
			AwChannel *source = new AwChannel(asRecorded);
			m_sources << source;	
			m_labelToIndex.insert(source->name(), index++);
		}
	}
	// force current filter settings to match those store in the ICA file.
	AwDataManager::instance()->filterSettings().set(this->m_type, m_hpFilter, m_lpFilter, 0.);
	AwDataManager::instance()->filterSettings().apply(m_sources);
	auto dm = AwDataManager::instance();
	//float sr = m_sources.at(0)->samplingRate();
	float sr = dm->value(data_info::max_sr).toFloat();
	// get reader 
	auto reader = dm->reader();

	// get layout
	AwLayout *l = NULL, *l3D = NULL;
	if (m_type == AwChannel::MEG) {
		l = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L2D|AwLayout::MEG);
		l3D =  AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L3D|AwLayout::MEG);
	}
	else {
		l = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L2D|AwLayout::EEG);
		l3D = AwLayoutManager::instance()->layoutFromFile(reader, AwLayout::L3D|AwLayout::EEG);
	}

	if (l == NULL && m_type == AwChannel::MEG) {
		l = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L2D|AwLayout::MEG);
		l3D = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L3D|AwLayout::MEG);
	}
	else if (l == NULL && m_type == AwChannel::EEG) {
		l = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L2D|AwLayout::EEG);
		l3D = AwLayoutManager::instance()->guessLayout(reader, AwLayout::L3D|AwLayout::EEG);
	}

	// unmixing matrix is ncomp x nchannels.
	// each line is a component topography
	QVector<float> values(m_mixing.n_rows);

	for (int i = 0; i < m_unmixing.n_rows; i++)	{
		AwICAChannel *chan = new AwICAChannel();
		//chan->setName(AwChannel::typeToString(m_type) + "_ICA " + QString::number(i + 1));
		chan->setName(QString("%1_ICA %2").arg(AwChannel::typeToString(m_type)).arg(i + 1));
		chan->setSamplingRate(sr);
		chan->setIndex(i);
		chan->setLayout2D(l);
		if (l3D)
			chan->setLayout3D(l3D);
		chan->setComponentType(m_type);
		// topography values for component i are in the ith column of matrix mixing
		memcpy(values.data(), m_mixing.colptr(i), m_mixing.n_rows * sizeof(float));
		// set channel labels in the right order
		chan->setLabels(m_labels);
		// set values for topography
		chan->setTopoValues(values);
		chan->setDisplayPluginName("ICA SignalItem");
		m_icaChannels << chan;
	}
	AwSettings::getInstance()->setValue(aws::ica_file, file.fileName());
	return AwHDF5::Ok;
}

QList<AwPanelItem *> AwICAComponents::createPanelItems()
{
	QList<AwPanelItem *> items;
	for (AwICAChannel *chan : m_icaChannels) 
		items << new AwPanelItem(chan);
	return items;
}

void AwICAComponents::showSEEGMap()
{
	if (m_seegMap == nullptr)
		m_seegMap = new AwSEEGMap(this);
	m_seegMap->show();
}


void AwICAComponents::showPanel()
{
	if (m_panel == NULL) {
		m_panel = new AwICAPanel(m_type);
		connect(m_panel, SIGNAL(componentModified(int, bool)), this, SLOT(updateComponentState(int, bool)));
	}
	m_panel->show();
}


void AwICAComponents::updateComponentState(int index, bool rejected)
{
	if (rejected)
		rejectComponent(index);
	else
		addComponent(index);
}