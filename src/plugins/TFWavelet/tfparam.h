#pragma once

#include <aw_armadillo.h>
class TFSettings;
class AwChannel;

// defines parameters for TF computation and also results:
typedef struct {
	TFSettings *settings;	// class handling computation paremeters
	AwChannel *x;			// channel to compute
	mat data;				// resulting matrix
	mat baselineData;		// resulting matrix for baseline correction. Empty if no baseline correction
} TFParam;
