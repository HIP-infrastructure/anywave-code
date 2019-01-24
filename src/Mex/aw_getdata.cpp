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
#include <AwChannel.h>

static mxArray *request_data();
static mxArray *parse_cfg(const mxArray *);

// parameters
int filterFlags = 0; // 0 => AnyWave current filters, 1 => specified filters 2=> raw data
QHash<int, QVector<float>> filterSettings;
QString montage, file;
float start = 0., duration = 0.;
QStringList labels, types;
int decimate = 0;

mxArray *request_data()
{
    QTcpSocket *socket = connect();
    mxArray *output = NULL;
    if (socket == NULL)  {
        mexErrMsgTxt("Could not connect to AnyWave.");
        return output;
    }
    int request = AwRequest::GetData3;
    QByteArray data;
    QDataStream stream_data(&data, QIODevice::WriteOnly);
	stream_data.setVersion(QDataStream::Qt_4_4);
    stream_data << request;
	stream_data << file;
	stream_data << montage;
	stream_data << start;
	stream_data << duration;  
    stream_data << decimate;   // decimation factor for downsampling of data (if value is 1 => no downsampling)
    stream_data << labels;
	stream_data << types;
	stream_data << filterFlags;
	
	if (filterFlags == 1)	{
		auto eegFilters = filterSettings[AwChannel::EEG];
		auto megFilters = filterSettings[AwChannel::MEG];
		auto emgFilters = filterSettings[AwChannel::EMG];
        stream_data << eegFilters[0];
		stream_data << eegFilters[1];
		stream_data << megFilters[0];
		stream_data << megFilters[1];
		stream_data << emgFilters[0];
		stream_data << emgFilters[1];
	}  
    writeToHost(socket, getPid(), data);
    // waiting for response
	int dataSize = waitForResponse(socket);  
   	if (dataSize == -1)	{
        mexErrMsgTxt("Bad status received from AnyWave.");
		delete socket;
		return output;
	}  
    // Handle response from AnyWave
    
    // Get response
	QDataStream in(socket);
	in.setVersion(QDataStream::Qt_4_4);
    int nChannels = 0;
    
    // create MATLAB data struct for output
    int nfields = 8;
    const char *fields[] = { "name", "type", "ref", "samplingRate", "data", "hpf", "lpf", "selected" };
    
    // get response from AnyWave
    in >> nChannels; 

    if (nChannels == 0) { // no channels returned (means that the requested labels did not match existing channels in AnyWave.
        output = mxCreateStructMatrix(1, 1, nfields, fields);
        delete socket;
        return output;
    }
    else {
        output = mxCreateStructMatrix(1, nChannels, nfields, fields);
    }
    
    // get channels 
    for (int i = 0; i < nChannels; i++) {
        dataSize = waitForResponse(socket);
        if (dataSize == -1) {
             mexErrMsgTxt("Bad status received from AnyWave.");
            delete socket;
            return output;
        }
        
        QString name, ref, type;
        float samplingRate, hpf, lpf;
        qint64 nSamples;
		int selected;
        
        in >> name >> type >> ref >> samplingRate >> hpf >> lpf >> nSamples >> selected;

        mxArray *f_name = mxCreateString(name.toStdString().c_str());
        mxSetField(output, i, "name", f_name);
        
        mxArray *f_type = mxCreateString(type.toStdString().c_str());
        mxSetField(output, i, "type", f_type);
        
        mxArray *f_ref =  mxCreateString(ref.toStdString().c_str());
        mxSetField(output, i, "ref", f_ref);
        
        mxArray *f_sr = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
        float *tmp = (float *)mxGetData(f_sr);
        *tmp = samplingRate;
        mxSetField(output, i, "samplingRate", f_sr);
        
        mxArray *f_hpf = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
        tmp = (float *)mxGetData(f_hpf);
        *tmp = hpf;
        mxSetField(output, i, "hpf", f_hpf);
        
        mxArray *f_lpf = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
        tmp = (float *)mxGetData(f_lpf);
        *tmp = lpf;
        mxSetField(output, i, "lpf", f_lpf);

	    mxArray *f_selected = mxCreateLogicalMatrix(1, 1);
		*mxGetLogicals(f_selected) = selected;
		mxSetField(output, i, "selected", f_selected);
        
        // read data for the current channel
        if (nSamples == 0) {  // End of data
            mxArray *f_data = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
            mxSetField(output, i, "data", f_data);
        }
        else {
            mxArray *f_data = mxCreateNumericMatrix(1, nSamples, mxSINGLE_CLASS, mxREAL);
            mxSetField(output, i, "data", f_data);
            float *data = (float *)mxGetData(f_data);
            // reading chuncks of data
            bool finished = false;
            qint64 nSamplesRead = 0;
            do {
                // waiting for chunk of data
                dataSize = waitForResponse(socket);
                if (dataSize == -1) {
                     mexErrMsgTxt("Error while receiving data.");
                     delete socket;
                     return output;
                }
                qint64 chunkSize;
                in >> chunkSize;
               if (chunkSize == 0) { // finished receiving data
                    finished = true;
                }
                else {
                    for (qint64 j = 0; j < chunkSize; j++) {
                        in >> data[j + nSamplesRead];
                    }
                    nSamplesRead += chunkSize;
                }
            }
            while (!finished);   
        }
    }
    return output;
}

// parse_cfg
// parse the input cfg structure
mxArray *parse_cfg(const mxArray *cfg)
{
    // check for start
    // default value    
    int f_index = mxGetFieldNumber(cfg, "start");
    if (f_index != -1) { // field found
        mxArray *f_start = mxGetField(cfg, 0, "start");
        if (!mxIsDouble(f_start)) {
            mexErrMsgTxt("start must be a double value.");
            return NULL;
        }
        if (f_start) {
            double s = *(double *)mxGetData(f_start);
            if (s > 0.)
                start = (float)s;
        }
    }
        
    // check for duration
    f_index = mxGetFieldNumber(cfg, "duration");
    if (f_index != -1) { // field found
        mxArray *f_dur = mxGetField(cfg, 0, "duration");
        if (!mxIsDouble(f_dur)) {
            mexErrMsgTxt("duration must be a double value.");
            return NULL;
        }
        if (f_dur) {
            double d = *(double *)mxGetData(f_dur);
            if (d > 0.)
                duration = (float)d;
        }
    }

	// check for file
	f_index =  mxGetFieldNumber(cfg, "file");
    if (f_index != -1) { // field found
		mxArray *f_file = mxGetField(cfg, 0, "file");
		if (mxIsChar(f_file))
			file = QString(mxArrayToString(f_file));
	}

	// check for montage
	f_index =  mxGetFieldNumber(cfg, "montage");
    if (f_index != -1) { // field found
		mxArray *f_montage = mxGetField(cfg, 0, "montage");
		if (mxIsChar(f_montage))
			montage = QString(mxArrayToString(f_montage));
	}

    // check for labels
    f_index = mxGetFieldNumber(cfg, "labels");
    if (f_index != -1) { // field found
        mxArray *f_labels = mxGetField(cfg, 0, "labels");
        if (!mxIsCell(f_labels)) {
            mexErrMsgTxt("labels must be a cell array (See help).");
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

    // check for types
    f_index = mxGetFieldNumber(cfg, "types");
    if (f_index != -1) { // field found
        mxArray *f_types = mxGetField(cfg, 0, "types");
        if (!mxIsCell(f_types)) {
            mexErrMsgTxt("types must be a cell array (See help).");
            return NULL;
        }
        for (int i = 0; i < mxGetNumberOfElements(f_types); i++) {
            mxArray *m_type = mxGetCell(f_types, i);
            size_t strlen = mxGetN(m_type) + 1;
            char *str = (char *)mxMalloc(strlen);
            if (mxGetString(m_type, str, (mwSize)strlen) == 0)
                types << QString(str);
            mxFree(str);
        }
    }

    // check for decimate option
    f_index = mxGetFieldNumber(cfg, "decimate");
    if (f_index != -1) {
         mxArray *f_decimate = mxGetField(cfg, 0, "decimate");
         if (!mxIsDouble(f_decimate)) {
             mexErrMsgTxt("decimate must be a value.");
             return NULL;
         }
         if (f_decimate) {
             double d = *(double *)mxGetData(f_decimate);
             decimate = (int)qFloor(d);
             if (d <= 0) 
                  decimate = 1; // 1 means no decimation
         }
    }
    
    // check for filtering options
    f_index = mxGetFieldNumber(cfg, "filtering");
	if (f_index == -1) {  // No filtering specified => filter flags => 0
		filterFlags = 0;
	}
	else {
		mxArray *f_filtering = mxGetField(cfg, 0, "filtering");
		QString s_filtering = QString(mxArrayToString(f_filtering)).toLower();
		if (s_filtering == "no") {
			filterFlags = 2;	// RAW DATA
		}
		if (s_filtering == "yes") {
			filterFlags = 1;
			int eegLP, eegHP, megLP, megHP, emgLP, emgHP;
			eegLP = mxGetFieldNumber(cfg, "eeg_lp");
			eegHP = mxGetFieldNumber(cfg, "eeg_hp");
			megLP = mxGetFieldNumber(cfg, "meg_lp");
			megHP = mxGetFieldNumber(cfg, "meg_hp");
			emgLP = mxGetFieldNumber(cfg, "emg_lp");
			emgHP = mxGetFieldNumber(cfg, "emg_hp");
			QVector<float> eegFilters = { 0., 0. };
			QVector<float> megFilters = { 0., 0. };
			QVector<float> emgFilters = { 0., 0. };
			if (eegLP != -1) {
				auto field = mxGetField(cfg, 0, "eeg_lp");
				if (!mxIsDouble(field)) {
					mexErrMsgTxt("eeg_lp must be a double value.");
					return NULL;
				}
				double v = *(double *)mxGetData(field);
				eegFilters[1] = (float)v;
			}
			if (eegHP != -1) {
				auto field = mxGetField(cfg, 0, "eeg_hp");
				if (!mxIsDouble(field)) {
					mexErrMsgTxt("eeg_hp must be a double value.");
					return NULL;
				}
				double v = *(double *)mxGetData(field);
				eegFilters[0] = (float)v;
			}
			if (megLP != -1) {
				auto field = mxGetField(cfg, 0, "meg_lp");
				if (!mxIsDouble(field)) {
					mexErrMsgTxt("meg_lp must be a double value.");
					return NULL;
				}
				double v = *(double *)mxGetData(field);
				megFilters[1] = (float)v;
			}
			if (megHP != -1) {
				auto field = mxGetField(cfg, 0, "meg_hp");
				if (!mxIsDouble(field)) {
					mexErrMsgTxt("meg_hp must be a double value.");
					return NULL;
				}
				double v = *(double *)mxGetData(field);
				megFilters[0] = (float)v;
			}
			if (emgLP != -1) {
				auto field = mxGetField(cfg, 0, "emg_lp");
				if (!mxIsDouble(field)) {
					mexErrMsgTxt("emg_lp must be a double value.");
					return NULL;
				}
				double v = *(double *)mxGetData(field);
				emgFilters[1] = (float)v;
			}
			if (emgHP != -1) {
				auto field = mxGetField(cfg, 0, "emg_hp");
				if (!mxIsDouble(field)) {
					mexErrMsgTxt("emg_hp must be a double value.");
					return NULL;
				}
				double v = *(double *)mxGetData(field);
				megFilters[0] = (float)v;
			}
			filterSettings.insert(AwChannel::EEG, eegFilters);
			filterSettings.insert(AwChannel::MEG, megFilters);
			filterSettings.insert(AwChannel::EMG, emgFilters);
		}
	}
    return request_data();
}


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{    
    if (nrhs != 1) {
        // one parameter only is required.
        mexErrMsgTxt("the function required one parameter (See help).");
        return;
    }
    
    if (mxGetClassID(prhs[0]) != mxSTRUCT_CLASS) {
        mexErrMsgTxt("the parameter must be a struct (See help).");
        return;
    }
    
    if (nlhs == 0) {
         mexErrMsgTxt("output parameter is required (See help).");
         return;
    }
    
    mxArray *out = parse_cfg(prhs[0]);
    if (out)
        plhs[0] = out;
}