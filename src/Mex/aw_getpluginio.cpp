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

	if (!request.writeToHost())
		return output;
	int dataSize = request.getResponse();
	if (dataSize < 0)
		return output;

    
    // reading response
    QDataStream in(request.socket());
    in.setVersion(QDataStream::Qt_4_4);
    QStringList labels, refs, types; // get labels of input channels, their references (may be empty and their types.
    float max_sr, total_dur;
    QString temp_dir, plugin_dir, file, ica_file, data_dir;
    // prepare matlab structure for output
    const char *fields[] = { "file", "data_dir", "labels", "refs", "types", "max_sr", "total_duration", "temp_dir", "plugin_dir", "ica_file" };
    const int nFields = 10;
    output = mxCreateStructMatrix(1, 1, nFields, fields);
	in >> file >> labels >> refs >> max_sr >> total_dur >> temp_dir >> plugin_dir >> ica_file >> data_dir >> types;
    
    // labels cell array
    mxArray *f_labels = NULL;
    if (!labels.isEmpty()) {
        f_labels = mxCreateCellMatrix(1, labels.size());
        for (int i = 0; i < labels.size(); i++) {
            mxArray *label = mxCreateString(labels.at(i).toLatin1().data());
            mxSetCell(f_labels, i, label);
        }
    }
    else 
        f_labels = mxCreateCellMatrix(1, 1);
    mxSetField(output, 0, "labels", f_labels);
    
    // refs
    mxArray *f_refs = NULL;
    if (!refs.isEmpty()) {
        f_refs = mxCreateCellMatrix(1, refs.size());
        for (int i = 0; i < refs.size(); i++) {
            mxArray *ref = mxCreateString(refs.at(i).toLatin1().data());
            mxSetCell(f_refs, i, ref);
        }
    }
    else
        f_refs = mxCreateCellMatrix(1, 1);
    mxSetField(output, 0, "refs", f_refs);

	// types
	mxArray *f_types = NULL;
	if (!types.isEmpty()) {
		f_types = mxCreateCellMatrix(1, types.size());
		for (int i = 0; i < types.size(); i++) {
			mxArray *type = mxCreateString(types.at(i).toLatin1().data());
			mxSetCell(f_types, i, type);
		}
	}
	else
		f_refs = mxCreateCellMatrix(1, 1);
	mxSetField(output, 0, "types", f_types);
    
    // max_sr
    mxArray *f_sr = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
    double *v = (double *)mxGetData(f_sr);
    v[0] = (double)max_sr;
    mxSetField(output, 0, "max_sr", f_sr);
    
    // total_duration
    mxArray *f_dur = mxCreateNumericMatrix(1, 1, mxDOUBLE_CLASS, mxREAL);
    v = (double *)mxGetData(f_dur);
    v[0] = (double)total_dur;
    mxSetField(output, 0, "total_duration", f_dur);
    
    // temp_dir
    mxArray *f_temp =  mxCreateString(temp_dir.toStdString().c_str());
    mxSetField(output, 0, "temp_dir", f_temp);
    
    // plugin_dir
    mxArray *f_plugin =  mxCreateString(plugin_dir.toStdString().c_str());
    mxSetField(output, 0, "plugin_dir", f_plugin);
    
    // file
    mxArray *f_file =  mxCreateString(file.toStdString().c_str());
    mxSetField(output, 0, "file", f_file);
    
    // ica_file
    mxArray *f_ica_file =  mxCreateString(ica_file.toStdString().c_str());
    mxSetField(output, 0, "ica_file", f_ica_file);

	// data_dir (the path to the folder containing the current data file)
	mxArray *f_data_dir = mxCreateString(data_dir.toStdString().c_str());
	mxSetField(output, 0, "data_dir", f_data_dir);
    
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