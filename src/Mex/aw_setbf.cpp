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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs != 6) {
		 mexErrMsgTxt("wrong number of parameters (See help).");
		 return;
	}
    if (mxGetClassID(prhs[0]) != mxCHAR_CLASS) {
        mexErrMsgTxt("wrong parameters: first parameter must be a string (See help).");
        return;
    }
	// expect param 1 to be a cell array of strings (labels)
	if (!mxIsCell(prhs[1])) {
		mexErrMsgTxt("wrong parameters: anywave_labels expected as second parameter (See help).");
		return;
	}
   	// expect param 1 to be a cell array of strings (labels)
	if (!mxIsCell(prhs[2])) {
		mexErrMsgTxt("wrong parameters: source_labels expected as third parameter (See help).");
		return;
	}
	if (mxGetClassID(prhs[3]) != mxDOUBLE_CLASS) {
		mexErrMsgTxt("wrong parameters: matrix expected as fourth parameter (See help).");
		return;
	}
	if (mxGetClassID(prhs[4]) != mxDOUBLE_CLASS) {
		mexErrMsgTxt("wrong parameters: HP filter expected as fifth parameter (See help).");
		return;
	}
	if (mxGetClassID(prhs[5]) != mxDOUBLE_CLASS) {
		mexErrMsgTxt("wrong parameters: LP filter expected as sixth parameter (See help).");
		return;
	}

    // get string type
    const mxArray *m_type = prhs[0];
    size_t strlen = mxGetN(m_type) + 1;
    char *str = (char *)mxMalloc(strlen);
    mxGetString(m_type, str, (mwSize)strlen);
    QString type = QString(str);
    mxFree(str);
	// Get anywave_labels
	const mxArray *m_aw_labels = prhs[1];
	QStringList awLabels;
	for (int i = 0; i < mxGetNumberOfElements(m_aw_labels); i++) {
		mxArray *m_label = mxGetCell(m_aw_labels, i);
		size_t strlen = mxGetN(m_label) + 1;
		char *str = (char *)mxMalloc(strlen);
		if (mxGetString(m_label, str, (mwSize)strlen) == 0)
			awLabels << QString(str);
		mxFree(str);
	}
    // Get source_labels
    const mxArray *m_source_labels = prhs[2];
    QStringList sourceLabels;
    for (int i = 0; i < mxGetNumberOfElements(m_source_labels); i++) {
        mxArray *m_label = mxGetCell(m_source_labels, i);
        size_t strlen = mxGetN(m_label) + 1;
        char *str = (char *)mxMalloc(strlen);
        if (mxGetString(m_label, str, (mwSize)strlen) == 0)
            sourceLabels << QString(str);
        mxFree(str);
    }
    
    const mxArray *matrix = prhs[3];
    const mwSize *dims = mxGetDimensions(matrix);
    const mwSize ndims = mxGetNumberOfDimensions(matrix);
    if (ndims != 2) {
        mexErrMsgTxt("beamformer matrix must have two dimensions (See help).");
        return;
    }

	double hp, lp;
	hp = *(double *)mxGetData(prhs[4]);
	lp = *(double *)mxGetData(prhs[5]);

	// send data to AnyWave
    QTcpSocket *socket = connect();
    if (socket == NULL) 
        return;
    
    int request = AwRequest::SetBeamFormer;
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
	stream_data.setVersion(QDataStream::Qt_4_4);
	stream_data << request;
    stream_data << type;
	stream_data << awLabels << sourceLabels;
	stream_data << (qint64)dims[0] << (qint64)dims[1];
	double *src = (double *)mxGetData(matrix);
	for (size_t i = 0; i < dims[0] * dims[1]; i++)
		stream_data << src[i];
	
	stream_data << hp << lp;
	writeToHost(socket, getPid(), data);
}