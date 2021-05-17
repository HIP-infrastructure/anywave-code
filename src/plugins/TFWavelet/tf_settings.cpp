#include "tf_settings.h"
#include "TFWavelet2.h"

TFSettings::TFSettings()
{
	tolerance = 0.5;
	wavelet = TFWavelet2::Gabor;
	xi = 7;
	freq_min = 5;
	freq_max = 0;
	useDIFF = false;
	step = 1.;
	computeMap = false;
	map_start = map_duration = 0.;
	useBaselineCorrection = false;
}

TFSettings::TFSettings(TFSettings *settings)
{
	tolerance = settings->tolerance;
	wavelet = settings->wavelet;
	xi = settings->xi;
	freq_min = settings->freq_min;
	freq_max = settings->freq_max;
	useDIFF = settings->useDIFF;
	useBaselineCorrection = settings->useBaselineCorrection;
	step = settings->step;
	computeMap = settings->computeMap;
	map_start = settings->map_start;
	map_duration = settings->map_duration;
}