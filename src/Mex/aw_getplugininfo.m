function [ infos ] = aw_getplugininfo()
%aw_getplugininfo returns information about the input set for the plugin.
%   infos = aw_getplugininfo();
%
%   returns a structure with the following fields:
%   .file
%       the file name open in AnyWave
%   .labels
%       labels of channels set as input for the plugin.
%   .refs
%       references of channels: Empty strings for monopolar channels.
%   .max_sr
%       maximum sampling rate in Hz.
%   .total_duration
%       the total duration in seconds of the data.
%   .temp_dir
%       the path to a temporary directory created by AnyWave for the
%       plugin.
%   .plugin_dir
%       The current plugin path. 
%
%   .ica_file
%       The path to the ica file currently loaded in AnyWave (empty string
%       if no ICA is loaded)
end

