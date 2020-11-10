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
#include <AwMarker.h>
#include <AwProcess.h>
#include <QtMath>

static void parse_markers(const mxArray*);
static void send_markers(AwMarkerList& markers);

void parse_markers(const mxArray* struct_markers)
{
    size_t nMarkers = mxGetNumberOfElements(struct_markers);
    mxArray* out = NULL;
    if (nMarkers == 0)
        return;

    AwMarkerList markers;
    for (int i = 0; i < nMarkers; i++) {
        QString s_label;
        float position, duration;
        float value;
        QStringList channels;
        QString s_color;

        // label
        int f_index = mxGetFieldNumber(struct_markers, "label");
        if (f_index == -1) {
            mexPrintf("Warning: field label is missing.\n");
            s_label = QString::fromLatin1("No Label");
        }
        else {
            mxArray* f_label = mxGetField(struct_markers, i, "label");
            s_label = QString(mxArrayToString(f_label));
            if (s_label.isEmpty())
                s_label = QString::fromLatin1("No Label");
        }
        // color (facultative)
        f_index = mxGetFieldNumber(struct_markers, "color");
        if (f_index != -1) { // found the field
            mxArray* f_color = mxGetField(struct_markers, i, "color");
            s_color = QString(mxArrayToString(f_color));
        }
        // position
        f_index = mxGetFieldNumber(struct_markers, "position");
        if (f_index == -1) {
            mexPrintf("Warning: field position is missing, skipping marker.\n");
            continue;
        }
        else {
            mxArray* f_pos = mxGetField(struct_markers, i, "position");
            if (mxIsDouble(f_pos)) {
                double* p = (double*)mxGetData(f_pos);
                position = (float)p[0];
            }
            else if (mxIsSingle(f_pos)) {
                float* p = (float*)mxGetData(f_pos);
                position = p[0];
            }
            else {
                mexPrintf("Warning: position is not a double value, skipping marker.\n");
                continue;
            }
        }
        // duration
        f_index = mxGetFieldNumber(struct_markers, "duration");
        if (f_index == -1)  //  no field, defaulting to 0
            duration = 0.;
        else {
            mxArray* f_dur = mxGetField(struct_markers, i, "duration");
            if (mxIsDouble(f_dur)) {
                double* d = (double*)mxGetData(f_dur);
                duration = (float)d[0];
            }
            else if (mxIsSingle(f_dur)) {
                float* d = (float*)mxGetData(f_dur);
                duration = d[0];
            }
            else
                duration = 0.;
        }
        // value
        f_index = mxGetFieldNumber(struct_markers, "value");
        if (f_index == -1) // no field, default to -1
            value = -1.;
        else {
            mxArray* f_value = mxGetField(struct_markers, i, "value");
            if (mxIsDouble(f_value)) {
                double* v = (double*)mxGetData(f_value);
                value = (float)v[0];
            }
            else if (mxIsSingle(f_value)) {
                float* v = (float*)mxGetData(f_value);
                value = v[0];
            }
            else
                value = -1.;
        }
        // channels
        f_index = mxGetFieldNumber(struct_markers, "channels");
        if (f_index == -1) // no field default to empty list
            channels = QStringList();
        else {
            mxArray* f_targets = mxGetField(struct_markers, i, "channels");
            if (!mxIsCell(f_targets))
                channels = QStringList();
            else {
                for (int i = 0; i < mxGetNumberOfElements(f_targets); i++) {
                    mxArray* m_target = mxGetCell(f_targets, i);
                    size_t strlen = mxGetN(m_target) + 1;
                    char* str = (char*)mxMalloc(strlen);
                    if (mxGetString(m_target, str, (mwSize)strlen) == 0)
                        channels << QString(str);
                    mxFree(str);
                }
            }
        }

        // create marker
        AwMarker* m = new AwMarker(s_label, position, duration);
        m->setValue(value);
        if (!s_color.isEmpty())
            m->setColor(s_color);
        if (!channels.isEmpty()) {
            m->setTargetChannels(channels);
        }

        markers << m;
    }

    send_markers(markers);

    while (!markers.isEmpty())
        delete markers.takeFirst();
}

void send_markers(AwMarkerList& markers)
{
    QTcpSocket* socket = connect();
    if (!socket) {
        mexErrMsgTxt("Unable to connect to host.");
        delete socket;
        return;
    }
    int request = AwRequest::AddMarkers;
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    stream_data.setVersion(QDataStream::Qt_4_4);

    stream_size << getPid();
    stream_data << request;
    stream_data << markers.size();
    for (int i = 0; i < markers.size(); i++)
    {
        AwMarker* marker = markers.at(i);
        // add marker
        stream_data << marker->label();
        stream_data << marker->start();
        stream_data << marker->duration();
        stream_data << marker->value();
        stream_data << marker->targetChannels();
        stream_data << marker->color();
    }

    stream_size << data.size();
    socket->write(size);
    socket->write(data);
    socket->flush();
    socket->waitForBytesWritten();
}

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
    if (nrhs >= 1) {        // check first parameter to be a struct.
        if (mxGetClassID(prhs[0]) != mxSTRUCT_CLASS) {
            mexErrMsgTxt("the first parameter must be a struct (See help).");
            return;
        }
    }
    if (nrhs == 0) {
        mexErrMsgTxt("input parameter required (See help).");
        return;
    }
    parse_markers(prhs[0]);
}