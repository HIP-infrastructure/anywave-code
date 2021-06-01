function [mixing,unmixing] = cca(data)
% function co compute cca mixing and unmixing matrices
%
% input :   - data : N*t matrix of data( N chan, t samples)
%
% output :  - unmixing : such that unmixing'*data = sources
%           - mixing : such that mixing*sources = data
gems=mean(data,2);% zero mean
data=data-gems*ones(1,size(data,2));

[y,w,r] = ccaqr(data,1);
mixing = double(pinv(w'));
unmixing = double(w');