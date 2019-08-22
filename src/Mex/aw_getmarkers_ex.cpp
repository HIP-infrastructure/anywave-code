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
#include "common.h"
#include <AwProcess.h>


mxArray *parse_cfg(const mxArray *);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nlhs == 0)
		return;
	if (nrhs > 1) {
		// one parameter only is required.
		mexErrMsgTxt("Wrong number of parameters (See help).");
		return;
	}

	mxArray *out = NULL;

	if (nrhs == 1) {
		if (mxGetClassID(prhs[0]) != mxSTRUCT_CLASS) {
			mexErrMsgTxt("the parameter must be a struct (See help).");
			return;
		}
		out = parse_cfg(prhs[0]);
	}
	if (nrhs == 0) {
		out = parse_cfg(0);
	}

	if (out)
		plhs[0] = out;
}

mxArray *parse_cfg(const mxArray *cfg)
{
	TCPRequest request(AwRequest::GetMarkersEx);
	if (request.status() != TCPRequest::connected)
		return NULL;

	QDataStream *stream = request.stream();
	mxArray *output = NULL;

	if (cfg)
		*stream << toJson(cfg);
	else
		*stream << QString();
	// if cfg == NULL => send an empty string (default parameters = all data from current input channels set for the process).

	if (!request.sendRequest())
		return NULL;

	int dataSize = request.getResponse();
	if (dataSize < 0)
		return NULL;

	// Reading response..
	QDataStream in(request.socket());
	in.setVersion(QDataStream::Qt_4_4);

	int nMarkers;
	in >> nMarkers;
	// building the array of struct AwMarker
	const char *fields[] = { "label", "position", "duration", "value", "channels" };

	// handle empty results
	if (nMarkers == 0)
		output = mxCreateStructMatrix(0, 0, 5, fields);
	else {
		output = mxCreateStructMatrix(1, (size_t)nMarkers, 5, fields);
		for (int i = 0; i < nMarkers; i++) {
			QString label;
			float start, duration;
			float value;
			QStringList channels;

			in >> label >> start >> duration >> value >> channels;

			// label
			mxArray *output_label = mxCreateString(label.toStdString().c_str());
			mxSetField(output, i, "label", output_label);

			// start
			mxArray *output_start = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
			float *s = (float *)mxGetData(output_start);
			s[0] = start;
			mxSetField(output, i, "position", output_start);

			// duration
			mxArray *output_duration = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
			float *d = (float *)mxGetData(output_duration);
			d[0] = duration;
			mxSetField(output, i, "duration", output_duration);

			// value
			mxArray *output_value = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
			float *v = (float *)mxGetData(output_value);
			v[0] = value;
			mxSetField(output, i, "value", output_value);

			// channels
			mxArray *output_channels = NULL;
			if (!channels.isEmpty()) {
				output_channels = mxCreateCellMatrix(1, channels.size());
				for (mwSize j = 0; j < channels.size(); j++) {
					mxArray *target = mxCreateString(channels.at(j).toLatin1().data());
					mxSetCell(output_channels, j, target);
				}
			}
			else // create an empty cell array
				output_channels = mxCreateCellMatrix(1, 1);
			mxSetField(output, i, "channels", output_channels);
		}
	}
	return output;
}