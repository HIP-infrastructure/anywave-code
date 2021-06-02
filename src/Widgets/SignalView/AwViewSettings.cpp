#include <widget/SignalView/AwViewSettings.h>
#include <AwChannel.h>


AwViewSettings::AwViewSettings(QObject *parent) : QObject(parent)
{
	showTimeGrid = showSeconds = showSensors = limitChannels = showMarkerLabels = showMarkerValues = true;
	showZeroLine = false;
	stackChannels = false;
	maxChannels = 40;
	filters << AwChannel::EEG << AwChannel::SEEG << AwChannel::MEG << AwChannel::GRAD << AwChannel::ECoG;
	secsPerCm = 0.5;
	markerBarMode = AwViewSettings::ShowMarkerBar;
	timeMode = AwViewSettings::ShowRelativeTime;
	gainLevels = new AwGainLevels(this);
}

AwViewSettings::AwViewSettings(AwViewSettings *source, QObject *parent) : QObject(parent)
{
	showZeroLine = source->showZeroLine;
	showTimeGrid = source->showTimeGrid;
	showSeconds = source->showSeconds;
	showSensors = source->showSensors;
	stackChannels = source->stackChannels;
	limitChannels = source->limitChannels;
	maxChannels = source->maxChannels;
	filters = source->filters;
	markerBarMode = source->markerBarMode;
	secsPerCm = source->secsPerCm;
	showMarkerLabels = source->showMarkerLabels;
	showMarkerValues = source->showMarkerValues;
	timeMode = source->timeMode;
	gainLevels = new AwGainLevels(source->gainLevels, this);
}
