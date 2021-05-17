#ifndef DISPLAY_SETTINGS_H
#define DISPLAY_SETTINGS_H

class DisplaySettings
{
public:
	DisplaySettings();

	enum Normalization { NoNorm = 1, N10log10Divisive = 2, ZScore = 4 };
	enum Flags { Normalization = 1, ColorMap = 2 , ZScale = 4, LogScaleSwitch = 32};
	enum ZMinMax { MaxToMax = 1, MinToMax = 2, ZeroToMax = 4};
	enum Values { Modulus, Modulus2 };
	int colorMap;
	int normalization;
	int zInterval;
	int values;
	bool logScale;
};


#endif