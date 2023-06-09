#ifndef H2PARAMS_H
#define H2PARAMS_H
//
// struct to dispatch for background computation
//
class AwMarker;
#include <AwChannel.h>
#include <QVector>
class H2;


class h2_params
{
public:
	// input
	AwChannel *x;
	AwChannel *y;
	int horizon;	// time window in samples
	int nbBins;		// Number of bins for h2 computing
	int maxLag;		// maximum lag in samples
	int step;		// step in samples
	// output
	QVector<double> h2;
	QVector<int> lag;
	quint32 index_x;		// row number in result matrix (in fact the channel number of X (in X v Y)
	quint32 index_y;		// column number in result matrix (in fact the channel number of Y (in X v Y)
	H2 *h2_process;			// pointer to process (used to handle abort requests)
	int method;				// comput H2 or R2
	int triggeredChannel;	// index of channel used to compute H2 based on triggered events.
	h2_params() {
		x = y = NULL; horizon = nbBins = maxLag = step = triggeredChannel = -1;
		index_x = index_x = 0;
		method = 0;
	}
};

//
typedef struct 
{
	AwMarker *marker;
	int nIterations;
	float samplingRate;
	QList<h2_params *> params;
	AwChannelList channels;		// copy of selected channels, duplicated
	H2 *h2_process;			// pointer to process (used to handle abort requests)
} H2_run;

#endif