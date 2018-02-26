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
#include <QtCore/QtMath>


static void parse_command(const mxArray *cfg);
static void send_command(int command, QVariantList& args);

void send_command(int command, QVariantList& args)
{
    QTcpSocket* socket = connect();   
    if (!socket)   {
        mexErrMsgTxt("Unable to connect to host.");
        delete socket;
        return;
    }
    
    int request = AwRequest::SendCommand;
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    stream_data.setVersion(QDataStream::Qt_4_4);
    
    stream_size << getPid();
    stream_data << request;
    stream_data << command;
    stream_data << args;
    
    stream_size << data.size();
    socket->write(size);
    socket->write(data);
    socket->waitForBytesWritten();
}

void parse_command(const mxArray *cfg)
{
    // check for command field
    int f_index = mxGetFieldNumber(cfg, "command");
    if (f_index != -1) {
        mxArray *f_cmd = mxGetField(cfg, 0, "command");
        QString cmd = QString(mxArrayToString(f_cmd)).toLower();
        QVariantList args;
        if (cmd == "center position") {
            // looking for position field
            f_index = mxGetFieldNumber(cfg, "position");
            if (f_index != -1) {
                mxArray *f_pos = mxGetField(cfg, 0, "position");
                if (!mxIsDouble(f_pos)) {
                    mexErrMsgTxt("position field must be a double value.");
                    return;
                }
                else {
                    double *v = (double *)mxGetData(f_pos);
                    args << (float)v[0];
                    send_command(AwProcessCommand::CenterOnPos, args);
                }
            }
            else { // no field position
                mexErrMsgTxt("position field is required for command 'center position' (See help).");
                return;
            }
        }
		else if (cmd == "update markers") 
			 send_command(AwProcessCommand::UpdateMarkers, args);
    }
    else { // no command field
        mexErrMsgTxt("no command field found (See help).");
        return;
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{   
    if (nrhs >= 1) {        // check first parameter to be a struct.
        if (mxGetClassID(prhs[0]) != mxSTRUCT_CLASS) {
            mexErrMsgTxt("the fisrt parameter must be a struct (See help).");
            return;
        }
    } 
    if (nrhs == 0) {
         mexErrMsgTxt("input parameter required (See help).");
         return;
    }
    
    parse_command(prhs[0]);
}