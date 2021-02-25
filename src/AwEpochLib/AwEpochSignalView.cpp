#include <epoch/AwEpochSignalView.h>
#include <epoch/AwEpochTree.h>


AwEpochSignalView::AwEpochSignalView(QWidget *parent, Qt::WindowFlags f, int flags, AwViewSettings *settings) :
	AwBaseSignalView(parent, f, flags, settings)
{
	m_epoch = Q_NULLPTR;
	// set default flags for epoch signal view
	int flags_ = AwBaseSignalView::NoNavButtons | AwBaseSignalView::NoInfoLabels | AwBaseSignalView::ViewAllChannels
		| AwBaseSignalView::NoMarkerBar | AwBaseSignalView::NoSettingsButton;
	setFlags(flags_);
	setSecPerCm(0.1);
	showElectrodesNames(false);
	showZeroLine(false);
	showMarkersLabels(false);
	showMarkersValues(false);
	showMarkers(true);
	stackChannels(true);
}


void AwEpochSignalView::updatePageDuration(float duration)
{
	if (m_channels.isEmpty() || m_epoch == Q_NULLPTR)
		return;

	m_pageDuration = std::min(m_epoch->condition()->epochDuration() , duration);
	reloadData();
}

void AwEpochSignalView::setChannels(const AwChannelList& channels)
{
	// always use the channels set by the Epoch.
	// We assume setChannels() is called from setEpoch()
	m_scene->clearChannels();
	m_scene->setChannels(m_channels);
	reloadData();
}

void AwEpochSignalView::setEpoch(AwEpoch *epoch)
{
	m_epoch = epoch;
	m_positionInFile = - epoch->condition()->preLatency();
	// channels will be those of the epoch Tree
	m_channels = epoch->channels();
	m_pageDuration = m_epoch->condition()->epochDuration();
	m_view->setTimeShift(-epoch->condition()->preLatency());
	setTotalDuration(m_pageDuration);
	// channels must contain data.
	setChannels(m_channels);
	// compute the gain factor for the max amplitude is shown for 3cm height
	auto gain = m_epoch->maxAmplitude() / 3.;
	m_navBar->amplitudeWidget()->changeCurrentChannelTypeAndValue(m_channels.first()->type(), gain);
}

void AwEpochSignalView::reloadData()
{
	if (m_channels.isEmpty())
		return;
	dataReceived();
	emit dataLoaded(m_positionInFile, m_pageDuration);
}

