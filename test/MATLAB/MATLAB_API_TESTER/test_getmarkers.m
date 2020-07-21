function [res] = test_getmarkers()
%TEST_GETMARKERS Summary of this function goes here
%   Detailed explanation goes here
AwSendMessage('aw_getmarkers tests...');
% expecting 5 markers (m1, m2, m3, m4, m5)
markers = aw_getmarkers();
assert(~isempty(markers), 'aw_getmarkers: returns empty. failed.');
assert(numel(markers) == 5, 'aw_getmarkers: expecting 5 markers. failed.');
expected_labels = { 'm1', 'm2', 'm3', 'm4', 'm5' };
labels = { markers(:).label };
assert(size(labels, 2) == size(expected_labels, 2), 'aw_getmarkers: labels number mismatch. failed.');
assert(all(ismember(labels, expected_labels)) == true, 'aw_getmarkers: labels mismatch. failed.');
expected_values = single([ -1, 1, 5.5, 10, 22]);
values = [ markers(:).value ];
assert(size(expected_values, 2) == size(values, 2) , 'aw_getmarkers: values number mismatch. failed.');
assert(all(ismember(expected_values, values)) == true, 'aw_getmarkers: values mismatch. failed.');
% check for durations of each markers
expected_durations = single([0, 8, 0, 2, 0]);
durations = [ markers(:).duration ];
assert(all(ismember(expected_durations, durations)) == true, 'aw_getmarkers: duration values mismatch. failed.');
expected_targets = { 'A2'};
% m5 must target A2
assert(size(markers(5).channels, 2) == size(expected_targets, 2), 'aw_getmarkers: m5 must target channel A2. failed.');
assert(all(cellfun(@isequal, markers(5).channels, expected_targets)) == true, 'aw_getmarkers: m5 must target channel A2. failed.');
% ask for a specific marker label
cfg = [];
cfg.labels = { 'm1', 'm5' };
markers = aw_getmarkers(cfg);
result = { markers(:).label };
assert(size(result, 2) == size(cfg.labels, 2), 'aw_getmarkers: requested m1 and m5, got wrong result. failed.');
assert(all(cellfun(@isequal, cfg.labels, result)) == true, 'aw_getmarkers: requested m1 and m5, got wrong result. failed.');
% ask for specific marker values
cfg = [];
cfg.values = [-1, 22];
expected = { 'm1', 'm5' };
markers = aw_getmarkers(cfg);
result = { markers(:).label };
assert(size(result, 2) == size(expected, 2), 'aw_getmarkers: requested markers for values -1 and 22 got wrong result. failed.');
assert(all(cellfun(@isequal, expected, result)) == true, 'aw_getmarkers: requested markers for values -1 and 22 got wrong result. failed.');
% testing extract triggers from a file
global plugin_io;
file = fullfile(plugin_io.plugin_dir, 'trigger_test.ades');
AwSendMessage(sprintf('opening file %s and extracting trigger values...', file));
cfg = [];
cfg.extract_triggers = 'yes';
cfg.file = file;
markers = aw_getmarkers(cfg);
assert(~isempty(markers), 'aw_getmarkers: requested to extract triggers, got empty result. failed.');
expected_values = single([150, 512]);
values = [ markers(:).value ];
assert(size(expected_values, 2) == size(values, 2), 'aw_getmarkers: extracted number of values mismatch. failed.');
assert(isequal(expected_values, values), 'aw_getmarkers: extracted triggers values mismatch. failed.');
res = true;
AwSendMessage('aw_getmarkers: Passed.');
end

