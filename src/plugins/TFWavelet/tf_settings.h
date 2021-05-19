#pragma once
#include <QVector>
class AwMarker;

class TFSettings
{
public:
	TFSettings();
	TFSettings(TFSettings *settings);
	float tolerance;
	int wavelet;
	int xi;
	float freq_min, freq_max;
	bool useDIFF;
	QVector<float> freqs;
	float step;	// step between frequencies
	bool computeMap;
	float map_start, map_duration;
	bool useBaselineCorrection;
	QString baselineMarker;	// marker label to use for baseline correction.
};