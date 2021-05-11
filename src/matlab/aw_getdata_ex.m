function [channels] = aw_getdata_ex(cfg)
res = [];
if nargin < 1
    res = py.anywave.get_data();
else
    args = py.dict(cfg);
    res = py.anywave.get_data(args);
end

tmp = cell(res);
channels = repmat(struct('name', 'toto', ...
    'type', 'eeg', 'ref', '...', ...
    'sr', 512, 'lp', 0, 'hp', 0, ...
    'notch', 0, 'data', single(zeros(1,10))), numel(tmp), 1);

for i=1:numel(tmp)
    channels(i).data = single(tmp{i}.data);
    channels(i).name = char(tmp{i}.name);
    channels(i).type = char(tmp{i}.type);
    channels(i).ref = char(tmp{i}.ref);
    channels(i).sr = double(tmp{i}.sr);
    channels(i).lp = double(tmp{i}.lp);
    channels(i).hp = double(tmp{i}.hp);
    channels(i).notch = double(tmp{i}.notch);
end

end

