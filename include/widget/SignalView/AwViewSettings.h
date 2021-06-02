#ifndef AWVIEWSETTINGS_H
#define AWVIEWSETTINGS_H
#include <AwGlobal.h>
#include <QObject>
#include <AwGainLevels.h>

class AW_WIDGETS_EXPORT AwViewSettings : public QObject
{
	Q_OBJECT
public:
	AwViewSettings(QObject *parent);
	AwViewSettings(AwViewSettings *source, QObject *parent);

	enum UpdateFlags { LimitNumberOfChannels = 0x1, Filters = 0x2, MaxNumberOfChannels = 0x4,
		ShowMarkerLabel = 0x8, ShowMarkerValue = 0xA, ShowBaseLine = 0x20, ShowTimeGrid = 0x40, 
		ShowSecondTicks = 0x80, ShowSensors = 0x100, Overlay = 0x200, MarkerBarMode = 0x400, 
		SecPerCm = 0x800, ShowMarkerBar = 0x1000, HideMarkerBar = 0x2000, ShowRelativeTime = 0x4000, ShowRecordedTime = 0x8000 };

	int maxChannels, markerBarMode;
	bool limitChannels, showMarkerLabels, showMarkerValues, showZeroLine, showTimeGrid, showSeconds, showSensors, stackChannels;
	int timeMode;	// should be ShowRelativeTime or ShowRecordedTime
	QList<int> filters;
	float secsPerCm;
	AwGainLevels* gainLevels;
};

#endif