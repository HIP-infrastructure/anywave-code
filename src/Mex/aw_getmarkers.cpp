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
static mxArray *request_markers(const QString& file, const QString& extractTriggers,
	QVector<float>& values, QStringList& labels, QStringList& channels);


mxArray *request_markers(const QString& file, const QString& extractTriggers, QVector<float>& values, QStringList& labels, QStringList& channels)
{
    QTcpSocket *socket = connect();
    mxArray *output = NULL;
    if (socket == NULL) {
        mexErrMsgTxt("Could not connect to AnyWave.");
        return output;
    }
    int request = AwRequest::GetMarkers2;
    QByteArray size;
    QDataStream stream_size(&size, QIODevice::WriteOnly);
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
    stream_size.setVersion(QDataStream::Qt_4_4);
    stream_data.setVersion(QDataStream::Qt_4_4);
    stream_size << getPid(); 
    stream_data << request;
	stream_data << file << extractTriggers << values << labels << channels;
       
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
            mxArray *output_label =  mxCreateString(label.toStdString().c_str());
            mxSetField(output, i, "label", output_label);
            
            // start
            mxArray *output_start = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
            float *s = (float *)mxGetData(output_start);
            s[0] = start;
            mxSetField(output, i, "position", output_start);
            
            // duration
            mxArray *output_duration = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
            float *d = (float *)mxGetData(output_duration);
            d[0] = duration;
            mxSetField(output, i, "duration", output_duration);
            
            // value
		   mxArray *output_value = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
		   float *v = (float *)mxGetData(output_value);
		   v[0] = value;
		   mxSetField(output, i, "value", output_value);
            
            // channels
            mxArray *output_channels = NULL;
            if (!channels.isEmpty()) {
                output_channels = mxCreateCellMatrix(1, channels.size());
                for (mwSize j = 0; j < channels.size(); j++) {
                    mxArray *target = mxCreateString(channels.at(j).toLatin1().data());
                    mxSetCell(output_channels, j, target);
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
    QVector<float> values;
    QStringList labels;
	QStringList channels;
	QString file, extractTriggers;
    
    if (cfg) {  // parse structure if a structure is passed as parameter
        // check for values
        int f_index = mxGetFieldNumber(cfg, "values");
        if (f_index != -1) {
            mxArray *f_values = mxGetField(cfg, 0, "values");
            if (!mxIsDouble(f_values)) {
                mexErrMsgTxt("values must be an array of double values (See help).");
                return NULL;
            }
            size_t nValues = mxGetNumberOfElements(f_values);
            values.reserve(nValues);
            for (int i = 0; i < nValues; i++) {
                double *v = (double *)mxGetData(f_values);
                values << (float)std::floor(v[i]);
            }
        }

		// check for file
		f_index = mxGetFieldNumber(cfg, "file");
        if (f_index != -1) {
            mxArray *f_file = mxGetField(cfg, 0, "file");
            if (mxIsChar(f_file)) 
                file = QString(mxArrayToString(f_file));
        }

		// check for extract_triggers
		f_index = mxGetFieldNumber(cfg, "extract_triggers");
        if (f_index != -1) {
            mxArray *f_extract = mxGetField(cfg, 0, "extract_triggers");
            if (mxIsChar(f_extract)) 
                extractTriggers = QString(mxArrayToString(f_extract));
        }
        
        
        // check for labels
        f_index = mxGetFieldNumber(cfg, "labels");
        if (f_index != -1) {
            mxArray *f_labels = mxGetField(cfg, 0, "labels");
            if (!mxIsCell(f_labels)) {
                mexErrMsgTxt("labels must be a cell array of strings (See help).");
                return NULL;
            }
            for (int i = 0; i < mxGetNumberOfElements(f_labels); i++) {
                mxArray *m_label = mxGetCell(f_labels, i);
                size_t strlen = mxGetN(m_label) + 1;
                char *str = (char *)mxMalloc(strlen);
                if (mxGetString(m_label, str, (mwSize)strlen) == 0)
                    labels << QString(str);
                mxFree(str);
            }
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
	return request_markers(file, extractTriggers, values, labels, channels);
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
    if (nrhs == 0)  {  // no parameter => get all markers.
        out = parse_cfg(NULL);
    }
    else {
        out = parse_cfg(prhs[0]);
    }
    if (out)
        plhs[0] = out;
}