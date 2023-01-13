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

AwBaseSignalView::AwBaseSignalView(AwViewSettings *settings, QWidget *parent)
	: QWidget(parent) 
{
	if (settings == nullptr) 
		m_settings = new AwViewSettings(this);
	else {
		m_settings = settings;
		m_settings->setParent(this); // take ownership
	}
	if (m_settings->timeScale == AwViewSettings::FixedPageDuration) 
		m_settings->pageDuration = m_settings->fixedPageDuration;
	setFocusPolicy(Qt::StrongFocus);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	m_settings->posInFile = m_settings->startPosition;
	m_scene = new AwGraphicsScene(m_settings, 0);
	m_view = new AwGraphicsView(m_scene, m_settings, 0);
	m_navBar = new AwNavigationBar(m_settings, this);
	m_markerBar = new AwBaseMarkerBar(m_settings, this);
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(3, 3, 3, 3);
	layout->addWidget(m_view);
	layout->addWidget(m_markerBar);
	layout->addWidget(m_navBar);
	setLayout(layout);
	if (!m_settings->showMarkerBar)
		m_markerBar->hide();
	m_navBar->setVisible(m_settings->showNavBar);
	if (m_settings->showAllChannels) {
		m_settings->filters.clear();
		for (auto type : AwChannel::intTypes)
			m_settings->filters << type;
	}
	makeConnections();
	connect(&m_filterSettings, &AwFilterSettings::settingsChanged, this, &AwBaseSignalView::setNewFilters);
}

AwBaseSignalView::~AwBaseSignalView()
{
	m_scene->clearChannels();
	delete m_view;
	delete m_scene;
}

void AwBaseSignalView::makeConnections()
{
	m_navBar->setEnabled(false);
	connect(m_scene, &AwGraphicsScene::needRefresh, this, &AwBaseSignalView::reloadData);
	connect(m_scene, &AwGraphicsScene::channelsSelectionChanged, m_navBar, &AwNavigationBar::updateNumberOfSelectedChannels);
	connect(m_scene, &AwGraphicsScene::closeViewClicked, this, &AwBaseSignalView::closeViewClicked);
	connect(m_scene, SIGNAL(cursorPositionChanged(float)), this, SIGNAL(cursorPositionChanged(float)));
	connect(m_scene, SIGNAL(mappingPositionChanged(float)), this, SIGNAL(mappingPositionChanged(float)));
	connect(m_scene, SIGNAL(numberOfDisplayedChannelsChanged(int)), m_view, SLOT(layoutItems()));
	connect(m_scene, SIGNAL(numberOfDisplayedChannelsChanged(int)), m_navBar, SLOT(updateNumberOfChannels(int)));
	connect(m_scene, SIGNAL(QTSModeEnded()), this, SIGNAL(QTSModeEnded()));
	connect(m_scene, &AwGraphicsScene::itemsOrderChanged, this, &AwBaseSignalView::channelsOrderChanged);
	connect(m_scene, &AwGraphicsScene::markerChanged, this, &AwBaseSignalView::markerChanged);
	// cursor specific
	connect(m_scene, SIGNAL(cursorClickedAtTime(float)), this, SIGNAL(cursorClicked(float)));
	connect(m_scene, SIGNAL(mappingTimeSelectionDone(float, float)), this, SIGNAL(mappingTimeSelectionDone(float, float)));
	connect(m_markerBar, &AwBaseMarkerBar::showMarkerClicked, m_scene, &AwGraphicsScene::highlightMarker);
	connect(m_markerBar, &AwBaseMarkerBar::showMarkerClicked, this, &AwBaseSignalView::markerBarHighlighted);
	// redirect settings signals to one signal to inform Display Setup Manager that settings have been changed and must be saved
	connect(m_navBar, &AwNavigationBar::filterButtonClicked, this, &AwBaseSignalView::openFilterGUI);
	// amplitude
	AwAmplitudeWidget *ampWidget = m_navBar->amplitudeWidget();
	ampWidget->setGainLevels(m_settings->gainLevels);
	connect(ampWidget, SIGNAL(amplitudesChanged()), this, SLOT(setAmplitudes()));
	connect(ampWidget, SIGNAL(amplitudeChanged(int, float)), this, SLOT(setAmplitude(int, float)));
	connect(m_scene, &AwGraphicsScene::settingsChanged, this, &AwBaseSignalView::updateSettings);
	connect(m_markerBar, &AwBaseMarkerBar::settingsChanged, this, &AwBaseSignalView::updateSettings);
	connect(m_view, &AwGraphicsView::settingsChanged, this, &AwBaseSignalView::updateSettings);
	connect(m_navBar, &AwNavigationBar::settingsChanged, this, &AwBaseSignalView::updateSettings);
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
	m_settings->fileDuration = dur;
	m_scene->updateSettings(aw::view_settings::file_duration);
	m_scene->updateSettings(aw::view_settings::pos_in_file);
	m_scene->updateSettings(aw::view_settings::page_duration);
	m_scene->reset();
	m_navBar->setEnabled(true);
	m_markerBar->setEnabled(true);
	m_markerBar->updateSettings(aw::view_settings::file_duration);
	m_markerBar->updateSettings(aw::view_settings::pos_in_file);
	m_markerBar->updateSettings(aw::view_settings::page_duration);

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

AwChannelList AwBaseSignalView::selectedChannels()
{
	return m_scene->selectedChannels();
}

void AwBaseSignalView::setMarkers(const AwSharedMarkerList& markers)
{
	// that should be used in a plugin
	m_scene->setMarkers(markers);
	m_markerBar->setMarkers(markers);
}

void AwBaseSignalView::clean()
{
	m_scene->clean();
	m_scene->setMarkingMode(false);
	m_markerBar->clean();
	m_navBar->clean();
}

void AwBaseSignalView::reloadData()
{
	if (m_channels.isEmpty())
		return;
	if (m_client.isConnected()) {
		m_client.requestData(&m_channels, m_settings->posInFile, m_settings->pageDuration);
		dataReceived();
		emit dataLoaded(m_settings->posInFile, m_settings->pageDuration);
	}
}

void AwBaseSignalView::dataReceived()
{
	m_scene->updateChannelsData();
	m_scene->update();
}

/// <summary>
/// receive all signals from child objects (scene, nav bar, marker bar)
/// </summary>
/// <param name="key"></param>
void AwBaseSignalView::updateSettings(int key, int sender)
{
	bool reload = false;
	switch (key) {
	case aw::view_settings::old_eeg_mode:
	case aw::view_settings::show_sensors:
	case aw::view_settings::show_zero_line:
	case aw::view_settings::marker_visibility:
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		break;
	case aw::view_settings::show_time_grid:
	case aw::view_settings::show_seconds:
	case aw::view_settings::stack_channels:
	case aw::view_settings::limit_channel_per_view:
	case aw::view_settings::max_visible_channels:
	case aw::view_settings::time_representation:
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);
		break;
		
	case aw::view_settings::show_markers:
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		break;
	case aw::view_settings::show_marker_bar:
		if (sender != aw::view_settings::sender_marker_bar)
			m_markerBar->setVisible(m_settings->showMarkerBar);
		break;
	case aw::view_settings::time_scale:
		// view must be updated first (in case we have to compute a new page duration)
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		if (sender != aw::view_settings::sender_marker_bar)
			m_markerBar->updateSettings(key);
		if (sender != aw::view_settings::sender_nav_bar)
			m_navBar->updateSettings(key);
		reload = true;
		break;
	case aw::view_settings::fixed_page_duration:   // fixed page duration value has changed  (the time scale is still set to FixedPageDuration)
		if (sender != aw::view_settings::sender_signalview) {
			m_settings->pageDuration = m_settings->fixedPageDuration;
			reload = true;
		}
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);
		if (sender != aw::view_settings::sender_marker_bar)
			m_markerBar->updateSettings(key);
		if (sender != aw::view_settings::sender_nav_bar)
			m_navBar->updateSettings(key);
		break;
	case aw::view_settings::secs_per_cm:
		if (sender != aw::view_settings::sender_signalview) 
			reload = true;
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);
		if (sender != aw::view_settings::sender_nav_bar)
			m_navBar->updateSettings(key);
		break;
	case aw::view_settings::channels_modalities:
		m_channels.clear();
		m_scene->clearChannels();
		for (AwChannel *c : m_montageChannels)
			if (m_settings->filters.contains(c->type()))
				m_channels << c;
		m_scene->setChannels(m_channels);
		if (m_channels.isEmpty())
			AwMessageBox::information(m_view, "View Settings", "No visible channels (change modalities filters)");
		else
			reload = true;
		break;
	case aw::view_settings::page_duration:
		if (sender != aw::view_settings::sender_scene) 
			m_scene->updateSettings(key);
		if (sender != aw::view_settings::sender_nav_bar)
			m_navBar->updateSettings(key);
		if (sender != aw::view_settings::sender_marker_bar)
			m_markerBar->updateSettings(key);
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);
		if (sender != aw::view_settings::sender_signalview)
			reload = true;
		break;
	case aw::view_settings::file_duration:
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		if (sender != aw::view_settings::sender_nav_bar)
			m_navBar->updateSettings(key);
		if (sender != aw::view_settings::sender_marker_bar)
			m_markerBar->updateSettings(key);
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);

		break;
	case aw::view_settings::pos_in_file:
		if (sender != aw::view_settings::sender_scene)
			m_scene->updateSettings(key);
		if (sender != aw::view_settings::sender_nav_bar)
			m_navBar->updateSettings(key);
		if (sender != aw::view_settings::sender_marker_bar)
			m_markerBar->updateSettings(key);
		if (sender != aw::view_settings::sender_view)
			m_view->updateSettings(key);
		if (sender != aw::view_settings::sender_signalview)
			reload = true;
		emit positionChanged(m_settings->posInFile);
		break;
	}
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
	// auto set show markers to off when marking
	m_settings->showMarkers = false;
	m_scene->updateSettings(aw::view_settings::show_markers);
	m_scene->setMarkingMode(true);
}

void AwBaseSignalView::stopMarking()
{
	m_scene->setMarkingMode(false);
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

void AwBaseSignalView::synchronizeOnPosition(float pos)
{
	float newPos = pos;
	if (pos < 0.)
		newPos = 0.;
	if (newPos + m_settings->pageDuration > m_settings->fileDuration)
		newPos = m_settings->fileDuration - m_settings->pageDuration;
	if (newPos == m_settings->posInFile) // the position is the actual one => do nothing
		return;
	m_settings->posInFile = newPos;
	updateSettings(aw::view_settings::pos_in_file, aw::view_settings::sender_signalview);
}

void AwBaseSignalView::centerViewOnPosition(float pos)
{
	float half_page = m_settings->pageDuration / 2;
	float start = pos - half_page;
	if (start < 0)
		start = 0;
	synchronizeOnPosition(start);
	m_scene->highlightPosition(pos);
}