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

static mxArray *parse_cfg(const mxArray *);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs > 1) {
		// one parameter only is required.
		mexErrMsgTxt("Wrong number of parameters (See help).");
		return;
	}
	
	mxArray *out = NULL, *in = NULL;

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

/// parse the cfg structure set as input parameter
mxArray *parse_cfg(const mxArray *cfg)
{
	TCPRequest request(AwRequest::GetDataEx);
	if (request.status() != TCPRequest::connected)
		return NULL;

	QDataStream *stream = request.stream();
	mxArray *output = NULL;

	if (cfg)
		*stream << toJson(cfg);
	
	// if cfg == NULL => send an empty string (default parameters = all data from current input channels set for the process).

	if (!request.sendRequest())
		return NULL;

	int dataSize = request.getResponse();
	if (dataSize < 0)
		return NULL;

	// Reading response..
	QDataStream in(request.socket());
	in.setVersion(QDataStream::Qt_4_4);

	int nChannels = 0;

	// create MATLAB data struct for output
	int nfields = 9;
	const char *fields[] = { "name", "type", "ref", "samplingRate", "data", "hpf", "lpf", "notch", "selected" };

	// get response from AnyWave
	in >> nChannels;

	if (nChannels == 0)  // no channels returned (means that the requested labels did not match existing channels in AnyWave.
		return output;
	
	output = mxCreateStructMatrix(1, nChannels, nfields, fields);
	for (auto i = 0; i < nChannels; i++) {
		if (request.getResponse() < 0) 
			return output;
		
		QString name, ref, type;
		float samplingRate, hpf, lpf, notch;
		qint64 nSamples;
		int selected;
		in >> name >> type >> ref >> samplingRate >> hpf >> lpf >> notch >> nSamples >> selected;

		mxSetField(output, i, "name", mxCreateString(name.toStdString().c_str()));
		mxSetField(output, i, "ref", mxCreateString(ref.toStdString().c_str()));
		mxSetField(output, i, "type", mxCreateString(type.toStdString().c_str()));
		mxSetField(output, i, "samplingRate",  floatToMat(samplingRate));
		mxSetField(output, i, "hpf", floatToMat(hpf));
		mxSetField(output, i, "lpf", floatToMat(lpf));
		mxSetField(output, i, "notch", floatToMat(notch));
		mxSetField(output, i, "selected", boolToLogical(selected > 0));

		if (nSamples == 0) 
			mxSetField(output, i, "data", mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL));
		else {
			mxArray *f_data = mxCreateNumericMatrix(1, nSamples, mxSINGLE_CLASS, mxREAL);
			mxSetField(output, i, "data", f_data);
			float *data = (float *)mxGetData(f_data);
			// reading chuncks of data
			bool finished = false;
			qint64 nSamplesRead = 0;
			qint64 chunkSize;
			do {
				// waiting for chunk of data
				dataSize = waitForResponse(request.socket());
				if (dataSize == -1) {
					mexPrintf("Error while receiving data.");
					return output;
				}
				in >> chunkSize;
				if (chunkSize == 0) { // finished receiving data
					finished = true;
				}
				else {
					for (qint64 j = 0; j < chunkSize; j++) {
						in >> data[j + nSamplesRead];
					}
					nSamplesRead += chunkSize;
				}
			} while (!finished);
		}
	}
	return output;
}

