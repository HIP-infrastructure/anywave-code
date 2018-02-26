function [ markers ] = aw_getmarkers( cfg )
%aw_getmarkers retrieves markers from AnyWave
%   markers = aw_getmarkers(cfg) 
%   markers = aw_getmarkers()
%   returns markers which match the settings defined in cfg structure or
%   all available markers if no cfg structure is passed as paramater.
%
%   cfg may contain the following fields: 
%   
%   cgf.file = 'datafile.eeg';
%       specify the file to be used.
%   cfg.extract_triggers = 'yes';
%       must be specify with the file field.
%       indicates that we want values extracted from trigger channels.
%   cfg.values = [1 10 12];
%       get markers depending on their value. Several values can be
%       specified.
%   cfg.labels = {'Spike1', 'Spike2'};
%       get markers depending on their labels.
%   cfg.channels = {'A1', 'A2'};
%       get markers which target specific channels.
%
% Note: if a marker matches several conditions (values, labels or channels) only one
% instance of the marker is returned.
%
%Output:
%   markers is an array of strucrures with the following fields:
%   .label 
%       a string representing the marker's label.
%   .position
%       the position in seconds from beginning of data.
%   .duration
%       duration in seconds. Can be zero if the marker is just a time position.
%   .value
%       a numerical value associated with the marker. -1 indicates that no value is set. 
%   .channels  
%       a cell array of channel labels. Can be empty if the marker is
%       global to all channels.
%
end

