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
#include <widget/SignalView/AwBaseSignalView.h>
#include <widget/SignalView/AwNavigationBar.h>
#include <widget/SignalView/AwBaseMarkerBar.h>
#include <widget/AwMessageBox.h>
#include <QWidget>
#include <QVBoxLayout>
#include <QtGlobal>
#include <AwGlobalMarkers.h>


AwBaseSignalView::AwBaseSignalView(QWidget *parent, Qt::WindowFlags f, int flags, AwViewSettings *settings)
	: QWidget(parent, f) 
{
	m_flags = flags;
	if (settings == nullptr)
		m_settings = new AwViewSettings(this);
	else {
		m_settings = settings;
		m_settings->setParent(this);
	}
	m_positionInFile = 0;
	m_pageDuration = 0;
	m_physics = new AwDisplayPhysics;
	m_physics->setSecsPerCm(m_settings->secsPerCm);
	if (m_settings->timeScaleMode == AwViewSettings::FixedPageDuration) {
		m_pageDuration = m_settings->fixedPageDuration;
		m_physics->setPageDuration(m_settings->fixedPageDuration);
	}
	m_startPosition = 0.;
	
	setFocusPolicy(Qt::StrongFocus);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	m_scene = new AwGraphicsScene(m_settings, m_physics, 0);
	m_view = new AwGraphicsView(m_scene, m_settings, m_physics, 0);
	m_navBar = new AwNavigationBar(this, flags);
	m_markerBar = new AwBaseMarkerBar(m_physics, this);
	m_markerInspector = nullptr;
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(3, 3, 3, 3);
	layout->addWidget(m_view);
	layout->addWidget(m_markerBar);
	layout->addWidget(m_navBar);
	setLayout(layout);
	m_scene->applyNewSettings(m_settings);
	m_navBar->setNewSettings(m_settings);
	if (flags & AwBaseSignalView::NoMarkerBar) {
		m_markerBar->hide();
		m_settings->markerBarMode = AwViewSettings::HideMarkerBar;
	}
	if (flags & AwBaseSignalView::ViewAllChannels) {
		m_settings->filters.clear();
		m_settings->filters << AwChannel::EEG << AwChannel::MEG << AwChannel::SEEG << AwChannel::ICA << AwChannel::Source
			<< AwChannel::ECG << AwChannel::EMG << AwChannel::Trigger << AwChannel::Other << AwChannel::GRAD << AwChannel::Reference;
	}
	makeConnections();

	connect(&m_filterSettings, &AwFilterSettings::settingsChanged, this, &AwBaseSignalView::setNewFilters);
}

AwBaseSignalView::~AwBaseSignalView()
{
	delete m_physics;
    m_scene->clearChannels();

	if (m_markerInspector)
		delete m_markerInspector;
	delete m_view;
	delete m_scene;
}

void AwBaseSignalView::setFlags(int flags)
{
	m_flags = flags;
	if (flags & AwBaseSignalView::NoMarkerBar) {
		m_markerBar->hide();
		m_settings->markerBarMode = AwViewSettings::HideMarkerBar;
	}
	if (flags & AwBaseSignalView::ViewAllChannels) {
		m_settings->filters.clear();
		for (int i = 0; i < AW_CHANNEL_TYPES; i++)
			m_settings->filters << i;
	}
	m_navBar->setFlags(flags);
	if (m_flags & AwBaseSignalView::NoNavBar)
		m_navBar->setVisible(false);
}

void AwBaseSignalView::setViewSettings(AwViewSettings* settings)
{
	if (m_settings == settings)
		return;
	delete m_settings;
	m_settings = settings;
	m_settings->setParent(this);
	updateSettings(m_settings, AwViewSettings::AllFlags);
}

void AwBaseSignalView::makeConnections()
{
	connect(m_navBar, SIGNAL(settingsChanged(AwViewSettings *, int)), this, SIGNAL(settingsChanged(AwViewSettings *, int)));
	m_navBar->setEnabled(false);
	if (m_flags & AwBaseSignalView::NoNavBar)
		m_navBar->setVisible(false);

	connect(m_scene, SIGNAL(needRefresh()), this, SLOT(reloadData()));
	connect(m_scene, SIGNAL(updatePositionInFile(float)), this , SLOT(setPositionInFile(float)));
	connect(m_scene, &AwGraphicsScene::channelsSelectionChanged, m_navBar, &AwNavigationBar::updateNumberOfSelectedChannels);
	connect(m_scene, SIGNAL(closeViewClicked()), this, SIGNAL(closeViewClicked()));
	connect(m_scene, SIGNAL(cursorPositionChanged(float)), this, SIGNAL(cursorPositionChanged(float)));
	connect(m_scene, SIGNAL(mappingPositionChanged(float)), this, SIGNAL(mappingPositionChanged(float)));
	connect(m_scene, SIGNAL(numberOfDisplayedChannelsChanged(int)), m_view, SLOT(layoutItems()));
	connect(m_scene, SIGNAL(numberOfDisplayedChannelsChanged(int)), m_navBar, SLOT(updateNumberOfChannels(int)));
	connect(m_scene, SIGNAL(QTSModeEnded()), this, SIGNAL(QTSModeEnded()));
	connect(m_scene, &AwGraphicsScene::itemsOrderChanged, this, &AwBaseSignalView::channelsOrderChanged);
	// cursor specific
	connect(m_scene, SIGNAL(cursorClickedAtTime(float)), this, SIGNAL(cursorClicked(float)));
	connect(m_scene, SIGNAL(mappingTimeSelectionDone(float, float)), this, SIGNAL(mappingTimeSelectionDone(float, float)));
	connect(m_markerBar, SIGNAL(showMarkerClicked(AwMarker *)), m_scene, SLOT(highlightMarker(AwMarker *)));
	connect(m_markerBar, SIGNAL(showMarkerClicked(AwMarker *)), this, SIGNAL(markerBarHighlighted(AwMarker *)));
	connect(m_markerBar, SIGNAL(showMarkersClicked(bool)), m_scene, SLOT(showMarkers(bool)));
	connect(m_markerBar, SIGNAL(positionChanged(float)), m_navBar, SLOT(updatePositionInFile(float)));

	connect(m_navBar, SIGNAL(startOfDataClicked()), m_scene, SLOT(goToStart()));
	connect(m_navBar, SIGNAL(endOfDataClicked()), m_scene, SLOT(goToEnd()));
	connect(m_navBar, SIGNAL(pageForwardClicked()), m_scene, SLOT(nextPage()));
	connect(m_navBar, SIGNAL(pageBackwardClicked()), m_scene, SLOT(previousPage()));
	connect(m_navBar, SIGNAL(positionChanged(int)), this, SLOT(goToPos(int)));

	// redirect settings signals to one signal to inform Display Setup Manager that settings have been changed and must be saved
	connect(m_navBar, SIGNAL(settingsChanged(AwViewSettings *, int)), this, SIGNAL(settingsChanged()));

	connect(m_navBar, SIGNAL(settingsChanged(AwViewSettings *, int)), m_view, SLOT(updateSettings(AwViewSettings *, int)));
	connect(m_navBar, SIGNAL(settingsChanged(AwViewSettings *, int)), m_scene, SLOT(updateSettings(AwViewSettings *, int)));
	connect(m_navBar, SIGNAL(settingsChanged(AwViewSettings *, int)), this, SLOT(updateSettings(AwViewSettings *, int)));
	connect(m_navBar, SIGNAL(markingStarted()), this, SLOT(startMarking()));
	connect(m_navBar, &AwNavigationBar::filterButtonClicked, this, &AwBaseSignalView::openFilterGUI);

	connect(m_view, SIGNAL(pageDurationChanged(float)), m_navBar, SLOT(updatePageDuration(float)));
	connect(m_view, SIGNAL(pageDurationChanged(float)), this, SLOT(updatePageDuration(float)));
	connect(m_view, SIGNAL(pageDurationChanged(float)), m_scene, SLOT(setPageDuration(float)));
	connect(m_view, SIGNAL(pageDurationChanged(float)), m_markerBar, SLOT(setPageDuration(float)));

	// amplitude
	AwAmplitudeWidget *ampWidget = m_navBar->amplitudeWidget();
	ampWidget->setGainLevels(m_settings->gainLevels);

	connect(ampWidget, SIGNAL(amplitudesChanged()), this, SLOT(setAmplitudes()));
	connect(ampWidget, SIGNAL(amplitudeChanged(int, float)), this, SLOT(setAmplitude(int, float)));
}

void AwBaseSignalView::changeObjects(AwGraphicsView *v, AwGraphicsScene *s, AwNavigationBar *navBar, AwBaseMarkerBar *markBar)
{
	disconnect(m_scene, 0, 0, 0);
	disconnect(m_view, 0, 0, 0);
	disconnect(m_markerBar, 0, 0, 0);
	disconnect(m_navBar, 0, 0, 0);
	if (v) {
		layout()->replaceWidget(m_view, v);
		delete m_view;
		m_view = v;
	}
	if (s) {
		delete m_scene;
		m_scene = s;
	}
	if (navBar) {
		layout()->replaceWidget(m_navBar, navBar);
		delete m_navBar;
		m_navBar = navBar;
	}
	if (markBar) {
		layout()->replaceWidget(m_markerBar, markBar);
		delete m_markerBar;
		m_markerBar = markBar;
	}
	// redo connections
	makeConnections();
}

void AwBaseSignalView::setRecordedTime(const QTime& time)
{
	m_recordedTime = time;
	m_view->setRecordedTime(time);
}

void AwBaseSignalView::setTotalDuration(float dur)
{
	m_scene->reset();
	m_scene->setFileDuration(dur);
	m_totalDuration = dur;
	m_navBar->setEnabled(true);
	m_navBar->setTotalDuration(dur);
	m_markerBar->setEnabled(true);
	m_markerBar->setPositionInFile(m_startPosition);
	m_markerBar->setTotalDuration(dur);
	m_navBar->updatePageDuration(m_view->pageDuration());
	m_navBar->updatePositionInFile(m_startPosition);
}

void AwBaseSignalView::setChannels(const QList<QSharedPointer<AwChannel>>& channels)
{
	m_channelSharedPtrs.clear();
	m_scene->clearChannels();
	m_montageChannels.clear();
	m_channels.clear();
	if (channels.isEmpty())
		return;
	m_channelSharedPtrs = channels;
	m_montageChannels = AwChannel::toChannelList(m_channelSharedPtrs);
	applyGainLevels();
	applyChannelFilters();
	m_scene->setChannels(m_channels);
	reloadData();
}

void AwBaseSignalView::setChannels(const AwChannelList& channels)
{
	m_channelSharedPtrs.clear();
	m_scene->clearChannels();
	m_montageChannels.clear();
	m_channels.clear();
	if (channels.isEmpty())
		return;

	// DO NOT CLONE channels in BaseSignalView class
	for (auto c : channels) {
		QSharedPointer<AwChannel> shared = QSharedPointer<AwChannel>(c);
		m_channelSharedPtrs << shared;
	}
	m_montageChannels = AwChannel::toChannelList(m_channelSharedPtrs);
	applyGainLevels();
	applyChannelFilters();
	m_scene->setChannels(m_channels);
	reloadData();
}

void AwBaseSignalView::applyGainLevels()
{
	m_channelTypes.clear();
	for (auto c : m_montageChannels) 
		m_channelTypes.insert(c->type(), c);
	// set gain levels for channels that are present
	auto types = m_channelTypes.uniqueKeys();
	for (auto t : types) {
		auto gl = m_settings->gainLevels->getGainLevelFor(t);
		auto channels = m_channelTypes.values(t);
		for (auto c : channels) 
			c->setGain(gl->value());
	}
	m_navBar->amplitudeWidget()->setGainLevels(m_settings->gainLevels);
}

void AwBaseSignalView::applyChannelFilters()
{
	// clear current channel list
	m_channels.clear();
	// rebuilding list applying filters
	for (AwChannel *c : m_montageChannels)	{
		if (m_settings->filters.contains(c->type())) {
			m_channels << c;
		}
	}
}

void AwBaseSignalView::updatePageDuration(float duration)
{
	m_pageDuration = duration;
	reloadData();
}

AwChannelList AwBaseSignalView::selectedChannels()
{
	return m_scene->selectedChannels();
}


void AwBaseSignalView::setPositionInFile(float pos)
{
	if (pos > m_startPosition)
		m_positionInFile = pos;
	else 
		m_positionInFile = m_startPosition;
	m_view->setPositionInFile(m_positionInFile);
	m_scene->setPositionInFile(m_positionInFile);
	m_navBar->updatePositionInFile(m_positionInFile);
	m_markerBar->setPositionInFile(m_positionInFile);
	reloadData();
	updateVisibleMarkers();
	emit positionChanged(m_positionInFile);
}

void AwBaseSignalView::updateVisibleMarkers()
{
	m_visibleMarkers = AwMarker::intersect(m_markers, m_positionInFile - m_startPosition, m_positionInFile - m_startPosition+ m_pageDuration);
	m_scene->setMarkers(m_visibleMarkers);
	m_markerBar->refresh();
}

void AwBaseSignalView::getNewMarkers()
{
	auto globals = AwGlobalMarkers::instance();
	auto list = globals->displayed();
	if (list == nullptr)
		return;
	m_markers = *list;
	updateVisibleMarkers();
}

void AwBaseSignalView::setMarkers(const AwMarkerList& markers)
{
	// that should be used in a plugin
	m_markers = markers;
	updateVisibleMarkers();
}

void AwBaseSignalView::clean()
{
	m_positionInFile = 0;
	m_scene->clean();
	m_scene->setMarkingMode(false);
	m_markerBar->clean();
	m_navBar->clean();
}

void AwBaseSignalView::goToPos(int pos)
{
	float p = (float)pos;
	p /= 1000;
	// Beware: scrollbar send values scaled by 1000 (to match ms precision)
	setPositionInFile(p);
}


void AwBaseSignalView::reloadData()
{
	if (m_channels.isEmpty())
		return;
	m_client.requestData(&m_channels, m_positionInFile, m_pageDuration);
	dataReceived();
	emit dataLoaded(m_positionInFile, m_pageDuration);
}

void AwBaseSignalView::dataReceived()
{
	m_scene->updateChannelsData();
	m_scene->update();
}


void AwBaseSignalView::updateSettings(AwViewSettings *settings, int flags)
{
	if (m_settings != settings)
		return;

	bool reload = false;

	if (flags & AwViewSettings::Filters) {
		// filter channels
		m_channels.clear();
		m_scene->clearChannels();
		for (AwChannel *c : m_montageChannels)
			if (settings->filters.contains(c->type()))
				m_channels << c;
		m_scene->setChannels(m_channels);
		if (m_channels.isEmpty())
			AwMessageBox::information(m_view, tr("View Settings"), tr("No channels will be shown regarding the options selected."));
		else
			reload = true;
	}

	if (flags & AwViewSettings::ShowMarkers)
		m_scene->showMarkers(m_settings->showMarkers);
	
	if (flags & AwViewSettings::MarkerBarMode)
		if (settings->markerBarMode == AwViewSettings::ShowMarkerBar)
			m_markerBar->show();
		else
			m_markerBar->hide();

	if (flags & AwViewSettings::TimeScaleMode) {
		if (m_settings->timeScaleMode == AwViewSettings::FixedPageDuration) 
			m_pageDuration = settings->fixedPageDuration;
		if (m_settings->timeScaleMode == AwViewSettings::PaperLike) 
			m_pageDuration = m_view->pageDuration();
		reload = true;
	}
	if (flags & AwViewSettings::PageDuration) {
		m_pageDuration = settings->fixedPageDuration;
		reload = true;
	}

	if (flags & AwViewSettings::SecPerCm)
		reload = true;
	if (reload)
		reloadData();
}

void AwBaseSignalView::setAmplitude(int type, float value)
{
	auto types = m_channelTypes.uniqueKeys();
	if (!types.contains(type))
		return;
	auto channels = m_channelTypes.values(type);
	for (auto c : channels) 
		c->setGain(value);
	m_scene->updateSignals();
}

void AwBaseSignalView::setAmplitudes()
{
	auto types = m_channelTypes.uniqueKeys();
	for (auto t : types) {
		auto gl = m_settings->gainLevels->getGainLevelFor(t);
		auto channels = m_channelTypes.values(t);
		for (auto c : channels)
			c->setGain(gl->value());
	}
	m_scene->updateSignals();
}

void AwBaseSignalView::startMarking()
{
	QStringList labels;
	foreach (AwMarker *m, m_markers)
		if (!labels.contains(m->label()))
			labels << m->label();

	if (!m_markerInspector) {
		m_markerInspector = new AwMarkerInspector(m_markers, labels);
		connect(m_markerInspector, SIGNAL(settingsChanged(AwMarkingSettings *)), m_scene, SLOT(setMarkingSettings(AwMarkingSettings *)));
		connect(m_markerInspector, SIGNAL(closed()), this, SLOT(stopMarking()));
	}
	else {
		m_markerInspector->setMarkers(m_markers);
		m_markerInspector->setTargets(labels);
	}
	m_scene->setMarkingSettings(&m_markerInspector->settings());
	m_markerInspector->show();
	m_scene->setMarkingMode(true);
}

void AwBaseSignalView::stopMarking()
{
	m_scene->setMarkingMode(false);
}

void AwBaseSignalView::setSecPerCm(float value)
{
	m_settings->secsPerCm = value;
	m_navBar->changeSettings(m_settings, AwViewSettings::SecPerCm);
}

void AwBaseSignalView::showElectrodesNames(bool flag)
{
	m_settings->showSensors = flag;
	m_navBar->changeSettings(m_settings, AwViewSettings::ShowSensors);
}

void AwBaseSignalView::showMarkersValues(bool flag)
{
	m_settings->showMarkerValues = flag;
	m_navBar->changeSettings(m_settings, AwViewSettings::ShowMarkerValue);
}

void AwBaseSignalView::showMarkersLabels(bool flag)
{
	m_settings->showMarkerLabels = flag;
	m_navBar->changeSettings(m_settings, AwViewSettings::ShowMarkerLabel);
}

void AwBaseSignalView::stackChannels(bool flag)
{
	m_settings->stackChannels = flag;
	m_navBar->changeSettings(m_settings, AwViewSettings::Overlay);
}

void AwBaseSignalView::makeChannelVisible(int type)
{
	if (!m_settings->filters.contains(type))
		m_settings->filters.append(type);
}

void AwBaseSignalView::removeVisibleChannel(int type)
{
	if (type == -1)
		m_settings->filters.clear();
	else
		m_settings->filters.removeAll(type);
}

void AwBaseSignalView::setNewFilters(const AwFilterSettings& settings)
{
	settings.apply(m_channels);
	reloadData();
}

void AwBaseSignalView::openFilterGUI()
{
	auto ui = m_filterSettings.ui();
//	if (m_filterSettings.isEmpty())
//		m_filterSettings.initWithChannels(m_channels);
	ui->show();
}

void AwBaseSignalView::processEvent(QSharedPointer<AwEvent> e)
{
	auto data = e->data();
	switch (e->id()) {
	case AwEvent::ShowChannels:
		break;
	case AwEvent::HighlightTimeSelection:

		break;
	}
}
