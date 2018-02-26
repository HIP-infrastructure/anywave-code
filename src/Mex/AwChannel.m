function [ output] = AwChannel(name, ref, samplingRate, data)
%AWCHANNEL Summary of this function goes here
%   Creates a structure for a channel
output.name = name;                 % sensor name of the channel
output.ref = ref;                   % reference name for the channel, can be empty for monopolar
output.samplingRate = samplingRate; % Sampling rate in Hz
output.data = data;                 % data vector (float 32bit)

end

