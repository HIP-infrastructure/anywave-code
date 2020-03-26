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

static void parse_cfg(const mxArray *, mxArray *plhs[], int nlhs);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nlhs == 0)
		return;

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
		parse_cfg(prhs[0], plhs, nlhs);
	}

	if (nrhs == 0) 
		parse_cfg(NULL, plhs, nlhs);
}

/// parse the cfg structure set as input parameter
void parse_cfg(const mxArray *cfg, mxArray *plhs[], int nlhs)
{
	TCPRequest request(AwRequest::GetDataEx);
	if (request.status() != TCPRequest::connected) {
		mexErrMsgTxt("Failed to connect to AnyWave.");
		return;
	}

	QDataStream *stream = request.stream();
	mxArray *output = NULL;
	if (cfg)
		*stream << toJson(cfg);
	
	if (!request.sendRequest()) {
		mexErrMsgTxt("sending request failed.");
		return;
	}

	int dataSize = request.getResponse();
	if (dataSize < 0) {
		mexErrMsgTxt("AnyWave did not respond to the request.");
		return;
	}

	// Reading response..
	QDataStream in(request.socket());
	in.setVersion(QDataStream::Qt_4_4);

	int nChannels = 0;

	// create MATLAB data struct for output
	const char *fields[] = { "name", "type", "ref", "samplingRate", "data", "hpf", "lpf", "notch" };

	// get response from AnyWave
	in >> nChannels;

	if (nChannels == 0) { // no channels returned (means that the requested labels did not match existing channels in AnyWave.
		plhs[0] = mxCreateStructMatrix(0, 0, 8, fields);
		return;
	}
	if (nChannels == -1) {
		QString errorString;
		in >> errorString;
		mexErrMsgIdAndTxt("AnyWave:aw_getdataex", errorString.toStdString().c_str());
	}
		
	output = mxCreateStructMatrix(1, nChannels, 8, fields);

	for (auto i = 0; i < nChannels; i++) {
		if (request.getResponse() == 0) 
			return;
		
		QString name, ref, type;
		float samplingRate, hpf, lpf, notch;
		qint64 nSamples;
		in >> name >> type >> ref >> samplingRate >> hpf >> lpf >> notch >> nSamples;

		mxSetField(output, i, "name", mxCreateString(name.toStdString().c_str()));
		mxSetField(output, i, "ref", mxCreateString(ref.toStdString().c_str()));
		mxSetField(output, i, "type", mxCreateString(type.toStdString().c_str()));
		mxSetField(output, i, "samplingRate",  doubleToMat((double)samplingRate));
		mxSetField(output, i, "hpf", doubleToMat((double)hpf));
		mxSetField(output, i, "lpf", doubleToMat((double)lpf));
		mxSetField(output, i, "notch", doubleToMat((double)notch));

		if (nSamples == 0) {
			mxSetField(output, i, "data", mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL));
			plhs[0] = output;
			mexErrMsgTxt("No samples sent by AnyWave.");
			return;
		}
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
				if (request.getResponse() == 0) {
					mexErrMsgTxt("Error while receiving data.");
					plhs[0] = output;
					return;
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
	plhs[0] = output;
}

