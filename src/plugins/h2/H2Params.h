/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
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