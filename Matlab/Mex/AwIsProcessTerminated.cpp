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
#include <process/AwTcpProcessRequest.h>

mxArray *handleIsProcessTerminated(QTcpSocket *socket)
{
    // creating data to send to AnyWave
 //   QString request = "is process terminated";
    int request = AwRequest::IsTerminated;
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    stream_data.setVersion(QDataStream::Qt_4_4);
    stream_size << getPid();
    stream_data << request;    
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
    // get response data
    bool isTerminated;
    in >> isTerminated;
    mwSize dims[2] = { 1, 1 };
    mxArray *out = mxCreateNumericArray(2, dims, mxLOGICAL_CLASS, mxREAL);
    int *v = (int *)mxMalloc(sizeof(int));
    *v = isTerminated;
    mxSetData(out, v);
    return out;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{    
   
    // no input params required    
    QTcpSocket* socket = connect();
    if (!socket)
    {
        mexErrMsgTxt("Could not connect to AnyWave.");
        delete socket;
        return;
    }
     
    mxArray *out = handleIsProcessTerminated(socket);
    if (out)
        plhs[0] = out;
    else
    {
        mexErrMsgTxt("Bad status received from AnyWave.");
        delete socket;
        return;
    }
      
	// end 
    delete socket;
}