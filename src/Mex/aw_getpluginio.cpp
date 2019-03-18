/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Universit� d�Aix Marseille (AMU) - 
//                 Institut National de la Sant� et de la Recherche M�dicale (INSERM)
//                 Copyright � 2013 AMU, INSERM
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
//    Author: Bruno Colombet � Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include <AwProcess.h>

static mxArray *request_info();

mxArray* request_info()
{
	mxArray *output = NULL;
	TCPRequest request(AwRequest::GetPluginIO);
	if (request.status() != TCPRequest::connected)
		return output;

	if (!request.sendRequest()) // check for error after sending request
		return output;
	int dataSize = request.getResponse();  // get the response size in bytes
	if (dataSize < 0)
		return output;

    
    // reading response
    QDataStream in(request.socket());
    in.setVersion(QDataStream::Qt_4_4);
    QStringList labels, refs, types, rejected_ics; // get labels of input channels, their references (may be empty and their types.
    float max_sr, total_dur;
    QString temp_dir, plugin_dir, file, ica_file, data_dir;
	int nMarkers = 0;
    // prepare matlab structure for output
    const char *fields[] = { "file", "data_dir", "labels", "refs", "types", "max_sr", "total_duration", "temp_dir", "plugin_dir", "ica_file", "markers", "rejected_ics" };
    const int nFields = 12;
    output = mxCreateStructMatrix(1, 1, nFields, fields);
	in >> file >> labels >> refs >> max_sr >> total_dur >> temp_dir >> plugin_dir >> ica_file >> data_dir >> types >> rejected_ics;

	// send also the markers set as input
	in >> nMarkers;
	mxArray *markers = 0, *tmp = 0;
#define MARKER_FIELDS 6 // edit nFields when changind the fields of the structure.
	const char *marker_fields[] = { "label", "position", "duration", "value", "channels", "chunk" }; // chunk is a vector containing position and duration of the corresponding marker.
	if (nMarkers == 0) {
		markers = mxCreateStructMatrix(0, 0, MARKER_FIELDS, marker_fields);
	}
	else {
		markers = mxCreateStructMatrix(1, (size_t)nMarkers, MARKER_FIELDS, marker_fields);
		for (auto i = 0; i < nMarkers; i++) {
			QString label;
			float start, duration, value;
			QStringList channels;

			in >> label >> start >> duration >> value >> channels;
			QVector<double> chunkVector = { start, duration };

			// label
			mxSetField(markers, i, "label", mxCreateString(label.toStdString().c_str()));
			// start
			mxSetField(markers, i, "position", floatToMat(start));
			// duration
			mxSetField(markers, i, "duration", floatToMat(duration));
			// value
			mxSetField(markers, i, "value", floatToMat(value));
			// chunk
			mxSetField(markers, i, "chunk", doubleVectorToMat(chunkVector));
			// channels
			tmp = NULL;
			if (!channels.isEmpty()) {
				tmp = mxCreateCellMatrix(1, channels.size());
				for (mwSize j = 0; j < channels.size(); j++) {
					mxSetCell(tmp, j, mxCreateString(channels.at(j).toStdString().c_str()));
				}
			}
			else // create an empty cell array
				tmp = mxCreateCellMatrix(1, 1);
			mxSetField(markers, i, "channels", tmp);
		}
	}


	// labels cell array
	if (!labels.isEmpty()) {
		tmp = mxCreateCellMatrix(1, labels.size());
		for (int i = 0; i < labels.size(); i++) {
			mxSetCell(tmp, i, mxCreateString(labels.at(i).toLatin1().data()));
		}
	}
	else
		tmp = mxCreateCellMatrix(1, 1);
	mxSetField(output, 0, "labels", tmp);

	// rejected_ics cell array
	if (!rejected_ics.isEmpty()) {
		tmp = mxCreateCellMatrix(1, rejected_ics.size());
		for (int i = 0; i < rejected_ics.size(); i++) {
			mxSetCell(tmp, i, mxCreateString(rejected_ics.at(i).toLatin1().data()));
		}
	}
	else
		tmp = mxCreateCellMatrix(1, 1);
	mxSetField(output, 0, "rejected_ics", tmp);
    
    // refs
    if (!refs.isEmpty()) {
        tmp = mxCreateCellMatrix(1, refs.size());
        for (int i = 0; i < refs.size(); i++) {
            mxSetCell(tmp, i, mxCreateString(refs.at(i).toLatin1().data()));
		}
    }
    else
        tmp = mxCreateCellMatrix(1, 1);
    mxSetField(output, 0, "refs", tmp);

	// types
	if (!types.isEmpty()) {
		tmp = mxCreateCellMatrix(1, types.size());
		for (int i = 0; i < types.size(); i++) {
			mxSetCell(tmp, i, mxCreateString(types.at(i).toLatin1().data()));
		}
	}
	else
		tmp = mxCreateCellMatrix(1, 1);
	mxSetField(output, 0, "types", tmp);
    
    // max_sr
    mxSetField(output, 0, "max_sr", doubleToMat((double)max_sr));
    // total_duration
    mxSetField(output, 0, "total_duration", doubleToMat((double)total_dur));
    // temp_dir
	mxSetField(output, 0, "temp_dir", mxCreateString(temp_dir.toStdString().c_str()));
    // plugin_dir
    mxSetField(output, 0, "plugin_dir", mxCreateString(plugin_dir.toStdString().c_str()));
    // file
    mxSetField(output, 0, "file", mxCreateString(file.toStdString().c_str()));
    // ica_file
    mxSetField(output, 0, "ica_file", mxCreateString(ica_file.toStdString().c_str()));
	// data_dir (the path to the folder containing the current data file)
	mxSetField(output, 0, "data_dir", mxCreateString(data_dir.toStdString().c_str()));
	// markers
	mxSetField(output, 0, "markers", markers);
    
    return output;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{    
    if (nlhs == 0)  // no output param => do nothing
         return;

    mxArray *out = request_info();
    if (out) 
        plhs[0] = out;
}