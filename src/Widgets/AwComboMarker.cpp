#include <widget/AwComboMarker.h>


void AwComboMarker::setMarkers(const AwMarkerList& markers)
{
	clear();
	addItems(AwMarker::getAllLabels(markers));
}

