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
#include <process/AwTcpProcessRequest.h>

static mxArray *request_info(const QString& file);

mxArray* request_info(const QString& file)
{
    QTcpSocket *socket = connect();
    mxArray *output = NULL;
    if (socket == NULL) {
        mexErrMsgTxt("Could not connect to AnyWave.");
        return output;
    }
    int request = AwRequest::GetFileInfo;
    
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    stream_data.setVersion(QDataStream::Qt_4_4);
    stream_size << getPid();
    stream_data << request;
	stream_data << file;
    
    // send request
    stream_size << data.size();
    socket->write(size);
    socket->write(data);
    socket->waitForBytesWritten();
    
    // wait for response
    int dataSize = waitForResponse(socket);
    if (dataSize == -1) {
         mexErrMsgTxt("No response from AnyWave.");
        return NULL;
    }
    
    // reading response
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_4);
    QStringList labels, types;
    float max_sr, total_dur;
	int flag;

    // prepare matlab structure for output
    const char *fields[] = { "labels", "max_sr", "total_duration"};
    const int nFields = 3;
    output = mxCreateStructMatrix(1, 1, nFields, fields);
	in >> flag;
	if (flag == -1) { // error
		return output;
	}

    in >> labels >> max_sr >> total_dur;
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
    
    return output;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{    
    if (nlhs == 0 || nrhs == 0)  // no output param or not input param => do nothing
         return;

	if (!mxIsChar(prhs[0])) {
		 mexErrMsgTxt("first argument must be a file path.");
		 return;
	}
	QString file(mxArrayToString(prhs[0]));
    mxArray *out = request_info(file);
    if (out) 
        plhs[0] = out;
}