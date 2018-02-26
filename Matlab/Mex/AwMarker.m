function [ marker ] = AwMarker( label, position, duration, value, targets)
% defines the structure for a marker
% 
%   marker.label
%       the label for the marker.
%   marker.position
%       the position in seconds from the beginning of data.
%   marker.duration
%       the duration in seconds. Can be 0 if the marker is just a position
%       in time.
%   marker.value
%       the value associated to the marker. -1 indicates that no value is
%       set.
%   marker.targets
%       a cell array containing the targeted channels. Empty if the marker
%       is global.
marker.label = label;                 
marker.position = position;
marker.duration = duration;           
marker.value = value;
marker.targets = targets;             

end

