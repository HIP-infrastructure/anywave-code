function test_getdata()
%TEST_GETDATA Summary of this function goes here
AwSendMessage('aw_getdata...');
cfg = [];
cfg.start = 0;
channels = aw_getdata(cfg);
assert(numel(channels) == 12, sprintf('expected 12 channels got %d. Failed', numel(channels)));
labels = { channels(:).name };
expected = {'A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'A9', 'A10', 'ECG', 'Trigger' };
assert(size(expected, 2) == size(labels, 2), sprintf('expected %d channels, got %d. Failed', numel(expected), numel(channels)));
assert(all(ismember(expected, labels) == true), 'channels labels mismatch. Failed.');
types = { channels(:).type };
expected = { 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'SEEG', 'ECG', 'TRIGGER' };
assert(all(ismember(expected, types) == true), 'channels types mismatch. Failed.');
expected = single( [ 256, 256,  256, 256,  256, 256,  256, 256,  256, 256,  256, 256 ]);
sr = [ channels(:).samplingRate ];
assert(all(ismember(expected, sr)) == true, 'wrong sampling rates. Failed.');
assert(numel(channels(1).data) == 10 * 256, 'wrong number of samples read.');
% get only 5s for first channel
cfg.start = 0;
cfg.duration = 5;
cfg.labels = { 'A1' };
AwSendMessage('Requesting 5s of data only for channel A1...');
A1 = aw_getdata(cfg);
assert(numel(A1) == 1, sprintf('expected 1 channel, got %d. Failed', numel(A1)));
assert(numel(A1(1).data) == 5 * 256, 'wrong number of samples read.');
expected = single(zeros(1, 5 * 256));
expected(:) = 150;
assert(all(ismember(expected, A1(1).data)) == true, 'samples values are wrong. Failed');
AwSendMessage('OK.');
AwSendMessage('Requesting 5s of data only for channel A1 but starting at position 5s...');
cfg.start = 5;
A1 = aw_getdata(cfg);
expected(:) = 300;
assert(all(ismember(expected, A1(1).data)) == true, 'samples values are wrong. Failed');
AwSendMessage('OK.');
AwSendMessage('Rewind to start position and requesting 5s of data only for channel A1...');
cfg.start = 0;
A1 = aw_getdata(cfg);
expected(:) = 150;
assert(all(ismember(expected, A1(1).data)) == true, 'samples values are wrong. Failed');
AwSendMessage('OK.');
cfg = [];
cfg.start = 0;
cfg.types = { 'SEEG' };
channels = aw_getdata(cfg);
assert(numel(channels) == 10, sprintf('expected 10 SEEG channels, got %d. Failed', numel(channels)));
cfg.types = { 'ECG' };
channels = aw_getdata(cfg);
assert(numel(channels) == 1, sprintf('expected 1 ECG channel, got %d. Failed', numel(channels)));
cfg.types = { 'TRIGGER' };
channels = aw_getdata(cfg);
assert(numel(channels) == 1, sprintf('expected 1 Trigger channel, got %d. Failed', numel(channels)));
AwSendMessage('aw_getdata Passed.');


end

