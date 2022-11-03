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
#include "AwSignalView.h"
#include "ICA/AwICASignalItem.h"
#include <widget/SignalView/AwGraphicsView.h>
#include "AwScene.h"
#include <widget/SignalView/AwNavigationBar.h>
#include "Widgets/AwMarkersBar.h"
#include <AwReadWriteLib.h>
#include "Marker/AwMarkerManager.h"
#include "Data/AwDataServer.h"
#include <AwAmplitudeManager.h>
#include <AwFileIO.h>
#include "ICA/AwICAChannel.h"
#include "Montage/AwMontageManager.h"
#include "ICA/AwICAManager.h"
#include <widget/AwMessageBox.h>
#include "Data/AwDataManager.h"
#include <QSharedPointer>

AwSignalView::AwSignalView(AwViewSettings *settings, int flags, QWidget *parent, Qt::WindowFlags windowFlags) :
	AwBaseSignalView(parent, windowFlags, flags, settings)
{
	auto dm = AwDataManager::instance();
	// Base constructor instantiates scene, view, navbar and marker bar objects
	// replace some objects with subclasses
	AwScene *scene = new AwScene(m_settings, m_physics, 0);
	AwGraphicsView *view = new AwGraphicsView(scene, m_settings, m_physics, 0);
	AwMarkersBar *markBar = new AwMarkersBar(m_physics, this);
	scene->applyNewSettings(m_settings);
	markBar->setNewSettings(m_settings);
	changeObjects(view, scene, nullptr, markBar);
	// connections
	// markers specific
	AwMarkerManager *mm = AwMarkerManager::instance();
//	connect(m_scene, SIGNAL(markerInserted(AwMarker *)), mm, SLOT(addMarker(AwMarker *)));
	connect(m_scene, &AwGraphicsScene::markerInserted, mm, &AwMarkerManager::addMarker);
	connect(m_scene, &AwScene::markerRemoved, mm, &AwMarkerManager::removeMarker);
	connect(m_scene, &AwScene::showMarkerUnderMouse, mm, &AwMarkerManager::highlightMarkerInList);
	connect(this, &AwSignalView::channelsOrderChanged, AwMontageManager::instance(), &AwMontageManager::reorderChannels);
//	connect(mm, SIGNAL(displayedMarkersChanged(const AwMarkerList&)), this, SLOT(getNewMarkers()));
	connect(mm, &AwMarkerManager::displayedMarkersChanged, this, &AwBaseSignalView::setMarkers);
	// filters
	connect(&dm->filterSettings(), &AwFilterSettings::settingsChanged, this, &AwSignalView::setNewFilters);
	connect(AwICAManager::instance(), SIGNAL(filteringSwitched(bool)), this, SLOT(reloadData()));
	m_isActive = false;	// View is not active until AwDisplay set it to Enabled.
	m_flags = UpdateProcess;	// by default a view will inform process manager that its contents changed.
		// init the view based on current open file
	if (dm->isFileOpen()) {
		dm->dataServer()->openConnection(client());
		setTotalDuration(dm->totalDuration());
		setRecordedTime(QTime::fromString(dm->value(keys::time).toString(), Qt::TextDate));
		m_isActive = true;
		reloadData();
	}

}


AwSignalView::~AwSignalView()
{
	AwDataServer::getInstance()->closeConnection(client());
}

void AwSignalView::setChannels(const AwChannelList& channels)
{
	m_scene->clearChannels();
	m_channelSharedPtrs.clear();
	m_channels.clear();
	m_montageChannels.clear();
	if (channels.isEmpty())
		return;

	for (auto c : channels) {
		QSharedPointer<AwChannel> shared = QSharedPointer<AwChannel>(c->duplicate());
		m_channelSharedPtrs << shared;
	}
	m_montageChannels = AwChannel::toChannelList(m_channelSharedPtrs);
	applyGainLevels();
	if (!m_isActive)
		return;
	// Before sending channels to scene, apply filter.
	applyChannelFilters();
	m_scene->setChannels(m_channels);
	reloadData();
}

void AwSignalView::updatePageDuration(float duration)
{
	float dur = m_pageDuration;
	AwBaseSignalView::updatePageDuration(duration);
	
	if (!m_isActive)
		return;

	if (duration > dur)
		reloadData();
}

void AwSignalView::reloadData()
{
	if (!m_isActive)
		return;
	AwBaseSignalView::reloadData();
}

void AwSignalView::updatePositionInFile(float pos)
{
	if (pos != m_positionInFile) {
		m_positionInFile = pos;
		emit positionChanged(pos);

		m_client.requestData(&m_channels, pos, m_pageDuration);
		dataReceived();
	}
}

void AwSignalView::synchronizeOnPosition(float pos)
{
	float newPos;
	if (pos < 0.)
		newPos = 0.;
	if (pos + m_pageDuration > m_totalDuration)
		pos = m_totalDuration - m_pageDuration;
	newPos = pos;
	m_view->setPositionInFile(newPos);
	m_scene->setPositionInFile(newPos);
	m_navBar->updatePositionInFile(newPos);
	m_markerBar->setPositionInFile(newPos);
	if (newPos == m_positionInFile)
		return;
	m_positionInFile = newPos;
	m_client.requestData(&m_channels, newPos, m_pageDuration);
	dataReceived();
}


void AwSignalView::centerViewOnPosition(float pos)
{
	if (!m_isActive)
		return;

	float half_page = m_pageDuration / 2;
	float start = pos - half_page;
	if (start < 0)
		start = 0;
	synchronizeOnPosition(start);
	m_scene->highlightPosition(pos);
}

void AwSignalView::showPosition(float pos)
{
	if (!m_isActive)
		return;

	float half_page = m_pageDuration / 2;

	if (pos < m_positionInFile || pos > m_positionInFile + m_pageDuration)
		synchronizeOnPosition(pos - half_page);

	m_scene->highlightPosition(pos);
}

void AwSignalView::goToPos(int pos)
{
	if (!m_isActive)
		return;
	AwBaseSignalView::goToPos(pos);
}

void AwSignalView::closeFile()
{
	m_settings->channelSelection.clear();
	for (auto c : m_scene->selectedChannels())
		m_settings->channelSelection << c->fullName();
	// clear channels present in scene.
	AwBaseSignalView::clean();
	m_scene->reset();
	AwDataManager::instance()->dataServer()->closeConnection(client());
	m_isActive = false;
}

void AwSignalView::enableView()
{
	auto dm = AwDataManager::instance();
	dm->dataServer()->openConnection(client());
	setTotalDuration(dm->totalDuration());
	setRecordedTime(QTime::fromString(dm->value(keys::time).toString(), Qt::TextDate));
	m_isActive = true;
	reloadData();
}

void AwSignalView::addVirtualChannels(AwChannelList& channels)
{
	foreach (AwChannel *c, channels)
		m_virtualChannels << c;
	AwScene *scene = (AwScene *)m_scene;
	scene->addVirtualChannels(channels);
}

void AwSignalView::removeVirtualChannels(AwChannelList& channels)
{
	foreach(AwChannel *c, channels)
		m_virtualChannels.removeAll(c);
	AwScene *scene = (AwScene *)m_scene;
	scene->removeVirtualChannels(channels);
}


void AwSignalView::removeICAChannels()
{
	bool update = false;
	foreach(AwChannel *c, m_channels) 
		if (c->isICA()) {
			update = true;
			m_channels.removeAll(c);
			delete c;
		}
	
	if (update) {
		m_scene->setChannels(m_channels);
	}
}


void AwSignalView::applyChannelFilters()
{
	// rebuilt current channel list depending on filters
	m_channels.clear();
	for (auto c : m_montageChannels) {
		if (m_settings->filters.contains(c->type()))
			m_channels << c;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////:
/// SLOTS


void AwSignalView::showICAMaps(bool flag)
{
	QList<AwGraphicsSignalItem *> items = scene()->signalItems();
	for (auto i : items) {
		auto channel = i->channel();
		if (channel->type() == AwChannel::ICA && channel->className() == "AwVirtual") {
			AwICASignalItem *ica_item = static_cast<AwICASignalItem *>(i);
			ica_item->showMap(flag);
		}
	}
}

void AwSignalView::refresh()
{
	// ICA specific
	AwICAManager *im = AwICAManager::instance();
	bool eeg = false, meg = false, emg = false;
	bool reload = false;

	for (int i = 0; i < AW_CHANNEL_TYPES; i++) {
		if (im->containsComponents(i) && !AwChannel::getChannelsOfType(m_channels, i).isEmpty()) {
			reload = true;
			break;
		}
	}
	if (reload)
		reloadData();
}

void AwSignalView::selectChannelsFromLabels(const QStringList& labels)
{
	m_scene->selectChannels(labels);
}

void AwSignalView::setCursorPosition(float pos)
{
	m_scene->setCursorPosition(pos);
}

void AwSignalView::dataReceived()
{
	AwBaseSignalView::dataReceived();
	if (m_processFlags == AwSignalView::UpdateProcess)
		emit displayedChannelsUpdated(m_channels);
	m_scene->update();
}