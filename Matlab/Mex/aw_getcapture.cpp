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
    if (nlhs == 0) {
         mexErrMsgTxt("output parameter is required (See help).");
         return;
    }
    
    QTcpSocket *socket = connect();
    mxArray *output = NULL;
    if (socket == NULL)  {
        mexErrMsgTxt("Could not connect to AnyWave.");
    }
    int request = AwRequest::GetScreenCapture;
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
	stream_data.setVersion(QDataStream::Qt_4_4);
    stream_data << request;
    
    writeToHost(socket, getPid(), data);
    // waiting for response
	int dataSize = waitForResponse(socket);  
   	if (dataSize == -1)	{
		mexErrMsgTxt("Bad status received from AnyWave.");
		delete socket;
	}
    // Get response
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_4);
    QString file;
    in >> file;
    plhs[0] = mxCreateString(file.toStdString().c_str());
    delete socket;
}