#include "display_settings.h"
#include <widget/AwColorMap.h>

DisplaySettings::DisplaySettings()
{
	colorMap =  AwColorMap::Parula;
	normalization = DisplaySettings::NoNorm;
	zInterval = DisplaySettings::MinToMax;
	logScale = true;	// default to log scale for frequencies
	values = DisplaySettings::Modulus; 
}