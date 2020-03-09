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
#include <QtMath>

static mxArray *parse_cfg(const mxArray *);
static mxArray *request_markers(const QString& file, const QStringList& channels);

mxArray *request_markers(const QString& file, const QStringList& channels)
{
	QTcpSocket *socket = connect();
	mxArray *output = NULL;
	if (socket == NULL) {
		mexErrMsgTxt("Could not connect to AnyWave.");
		return output;
	}
	int request = AwRequest::GetTriggers;
	QByteArray size;
	QDataStream stream_size(&size, QIODevice::WriteOnly);
	QByteArray data;
	QDataStream stream_data(&data, QIODevice::WriteOnly);
	stream_size.setVersion(QDataStream::Qt_4_4);
	stream_data.setVersion(QDataStream::Qt_4_4);
	stream_size << getPid();
	stream_data << request;
	stream_data << file << channels;

	// send request
	stream_size << data.size();
	socket->write(size);
	socket->write(data);
	socket->waitForBytesWritten();

	// waiting for response
	int dataSize = waitForResponse(socket);
	if (dataSize == -1)
		return NULL;

	QDataStream in(socket);
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
			mxSetField(output, i, "label", mxCreateString(label.toStdString().c_str()));
			// start
			mxSetField(output, i, "position", doubleToMat((double)start));
			// duration
			mxSetField(output, i, "duration", doubleToMat((double)duration));
			// value
			mxSetField(output, i, "value", doubleToMat((double)value));
			// channels
			mxArray *output_channels = NULL;
			if (!channels.isEmpty()) {
				output_channels = mxCreateCellMatrix(1, channels.size());
				for (mwSize j = 0; j < channels.size(); j++) {
					mxSetCell(output_channels, j, mxCreateString(channels.at(j).toStdString().c_str()));
				}
			}
			else // create an empty cell array
				output_channels = mxCreateCellMatrix(1, 1);
			mxSetField(output, i, "channels", output_channels);
		}
	}
	return output;
}

mxArray *parse_cfg(const mxArray *cfg)
{
	// default value
	QStringList channels;
	QString file;

	if (cfg) {  // parse structure if a structure is passed as parameter
		// check for file
		int f_index = mxGetFieldNumber(cfg, "file");
		if (f_index == -1) { // file field is MANDATORY
			mexErrMsgTxt("Please, specify the file to open (See help).");
			return NULL;
		}
		mxArray *f_file = mxGetField(cfg, 0, "file");
		if (mxIsChar(f_file))
			file = QString(mxArrayToString(f_file));
		else {
			mexErrMsgTxt("file must be a string (See help).");
			return NULL;
		}

		// check for channels
		f_index = mxGetFieldNumber(cfg, "channels");
		if (f_index != -1) {
			mxArray *f_channels = mxGetField(cfg, 0, "channels");
			if (!mxIsCell(f_channels)) {
				mexErrMsgTxt("channels must be a cell array of strings (See help).");
				return NULL;
			}
			for (int i = 0; i < mxGetNumberOfElements(f_channels); i++) {
				mxArray *m_channel = mxGetCell(f_channels, i);
				size_t strlen = mxGetN(m_channel) + 1;
				char *str = (char *)mxMalloc(strlen);
				if (mxGetString(m_channel, str, (mwSize)strlen) == 0)
					channels << QString(str);
				mxFree(str);
			}
		}
	}
	return request_markers(file, channels);
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs > 1) {        // check first parameter to be a struct.
		if (mxGetClassID(prhs[0]) != mxSTRUCT_CLASS) {
			mexErrMsgTxt("the parameter must be a struct (See help).");
			return;
		}
	}

	if (nlhs == 0) {
		mexErrMsgTxt("output parameter is required (See help).");
		return;
	}
	mxArray *out = NULL;
	if (nrhs == 0) {  // no parameter => get all markers.
		out = parse_cfg(NULL);
	}
	else {
		out = parse_cfg(prhs[0]);
	}
	if (out)
		plhs[0] = out;
}
