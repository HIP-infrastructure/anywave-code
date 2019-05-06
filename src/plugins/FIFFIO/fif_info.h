#pragma once

class fifProj;
class fifCtfComp;
#include <QList>

class fifInfo
{
public:
	fifInfo();
	float sfreq, highpass, lowpass;
	int meas_date[2];
	QList<fifProj *> projs;
	QList<fifCtfComp *> comps;

};