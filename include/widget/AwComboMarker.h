#pragma once
#include <QComboBox>
#include <AwGlobal.h>
#include <AwMarker.h>
#include <QStringList>

// this combo box class is designed to select a marker from a list. Markers can be sorted to only display unique names or not.
// the default flag is to not filter anything, so all markers set by setMarkers will be added.
// use AwComboMarker::ExcludeNoDuration to only have markers with duration in the list.

class AW_WIDGETS_EXPORT AwComboMarker : public QComboBox
{
	Q_OBJECT
public:
	explicit AwComboMarker(QWidget *parent = nullptr) : QComboBox(parent) {}
	void setMarkers(const AwMarkerList& markers);
};
