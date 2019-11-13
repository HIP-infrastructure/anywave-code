function [res] = test_getpluginio()
%TEST_GETPLUGINIO Summary of this function goes here
%   Detailed explanation goes here
AwSendMessage('aw_getpluginio tests...');
global plugin_io;
plugin_io = aw_getpluginio;
save('d:\pluginio.mat', 'plugin_io');
assert(~isempty(plugin_io), 'aw_getpluginio: returns empty. failed.');
assert(isfile(plugin_io.file), 'aw_getpluginio: file property is incorrect. failed.');
assert(plugin_io.max_sr == 256, 'aw_getpluginio: max_sr property is incorrect. failed.');
assert(plugin_io.total_duration == 10, 'aw_getpluginio: total_duration property is incorrect. failed.');
% rebuild file using data_dir
assert(isfile(fullfile(plugin_io.data_dir, 'test.ades')), 'aw_getpluginio: data_dir property is incorrect. failed.');
expected = {'A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'A9', 'A10', 'ECG', 'Trigger' };
assert(size(expected, 2) == size(plugin_io.labels, 2), 'aw_getpluginio: labels size mismatched');
assert(all(cellfun(@isequal, expected, plugin_io.labels)) == true, 'aw_getpluginio: labels size mismatched');
% markers should be filled if the plugin as the input_flags = GetAllMarkers
assert(~isempty(plugin_io.markers), 'aw_getpluginio: markers should not be empty (check input_flags in desc.txt) failed');
assert(size(plugin_io.markers, 2) == 5, 'aw_getpluginio: markers size is invalid. failed');
% test labels
expected = { 'm1', 'm2', 'm3', 'm4', 'm5' };
labels = { plugin_io.markers(:).label };
assert(all(ismember(expected, labels)) == true, 'aw_getpluginio: marker labels mismatched');
% test values
expected = [ -1, 1, 5.5, 10, 22];
values =  [ plugin_io.markers(:).value ];
assert(all(ismember(expected, values)) == true, 'aw_getpluginio: marker values mismatched');
% test durations
expected_durations = [ 0, 8, 0, 2, 0];
durations = [ plugin_io.markers(:).duration ];
assert(all(ismember(expected_durations, durations)) == true, 'aw_getpluginio: marker durations mismatched');
% test positions
expected_positions = single([ 1., 1.5, 1.6, 2., 5. ]);
positions = [ plugin_io.markers(:).position ];
assert(all(ismember(expected_positions, positions)) == true, 'aw_getpluginio: marker positions mismatched');

AwSendMessage('aw_getpluginio tests Passed.');
res = true;
end

