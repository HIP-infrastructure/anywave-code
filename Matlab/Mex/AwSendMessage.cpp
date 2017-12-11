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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{    
    // one input param (string)
    if (nrhs != 1)
    {
        mexErrMsgTxt("One parameter expected: 'message'.");
        return;
    }
    if (mxGetClassID(prhs[0]) != mxCHAR_CLASS)
    {
        mexErrMsgTxt("Parameter must be a string.");
        return;
    }
        
    QTcpSocket* socket = connect();
    if (!socket)
    {
        mexErrMsgTxt("Could not connect to AnyWave.");
        delete socket;
        return;
    }
     
    QString message(mxArrayToString(prhs[0]));
    
    // creating data to send to AnyWave
//    QString request = "send message";
    int request = AwRequest::SendMessage;
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_data.setVersion(QDataStream::Qt_4_4);
    stream_size << getPid();
    stream_data << request;    
    stream_data << message;
    stream_size << data.size();
    socket->write(size);
    socket->write(data);
    socket->waitForBytesWritten();
         
	// end 
    delete socket;
}