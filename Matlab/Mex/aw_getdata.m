function [ channels ] = aw_getdata(cfg )
%aw_getdata request data from AnyWave
%   channels = aw_getdata(cfg) 
%   returns channels' data according to the settings defined in cfg structure.
%
%   cfg may contain the following fields: 
%   
%   cfg.start = 10.;
%       starting position in seconds of requested data.
%       if this field is not specified, AnyWave will return data starting
%       at position 0s.
%   cfg.duration = 20.;
%       duration of requested data in seconds.
%       if this field is not specified, AnyWave will return ALL the
%       available data.
%   cfg.labels = {'A1', 'A2'};
%       cell array of strings to identify the required channel labels.
%       if no labels are specified, AnyWave will return the current selected
%       channels set as input for the plugin.
%   cfg.types = {'EEG','MEG'};
%       cell array of strings to identify the type of required channels.
%       can be empty.
%
%   cfg.filtering = 'no';
%       specifies that we don't want AnyWave to filter the data.
%   cfg.filtering = 'yes';
%       specifies that we want data to be filtered.
%   Note:
%       if the filtering field is not specified, the data will be filtered by
%       AnyWave using the current user defined settings.
%
%   cfg.eeg_lp = 10.;  requires filtering = 'yes'.
%       specifies that we want EEG data with low pass filter of 10Hz.
%   cfg.eeg_hp = 1.;  requires filtering = 'yes'.
%       specifies that we want EEG data with high pass filter of 1Hz.
%   cfg.meg_lp = 10.;  requires filtering = 'yes'.
%       specifies that we want MEG data with low pass filter of 10Hz.
%   cfg.meg_hp = 1.;  requires filtering = 'yes'.
%       specifies that we want MEG data with high pass filter of 1Hz.
%
%   cfg.decimate = 8;
%       specifies that we only take 1 sample of data every 8 samples.
%
%Output:
%   channels is an array of strucrures with the following fields:
%   .label; 
%       a string representing the channel's label.
%   .ref;
%       a string representing the reference channel. Can be empty.
%   .data;
%       a data vector containing the samples.
%   .sr;
%       the sampling rate of data. 
%   .hpf;  
%       the High Pass filter set.
%   .lpf;
%       the Low Pass filter set.
end

