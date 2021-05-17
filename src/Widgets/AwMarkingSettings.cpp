#include <AwMarkingSettings.h>


void AwMarkingSettings::setPredefinedMarkers(const AwMarkerList& markers)
{
	this->predefinedMarkers = markers;
	this->selectedPredefinedMarkers.clear();
}

AwMarkerList AwMarkingSettings::getSelectedPredefinedMarkers()
{
	AwMarkerList res;
	for (auto selected : this->selectedPredefinedMarkers)
		res.append(this->predefinedMarkers.at(selected));
	return res;
}