function [mixing,unmixing] = cca(data, ncomps)
% function co compute cca mixing and unmixing matrices
%
% input :   - data : N*t matrix of data( N chan, t samples)
%
% output :  - unmixing : such that unmixing'*data = sources
%           - mixing : such that mixing*sources = data
[m, ~] = size(data);
eigenvectors = [];
pca_applied = false;
if m > ncomps
    pca_applied = true;
    [pca_data, eigenvectors] = pca(data, ncomps);
    data = pca_data;
end

gems=mean(data,2);% zero mean
data=data-gems*ones(1,size(data,2));

[~,w,~] = ccaqr(data,1);

unmixing = double(w');
mixing = double(pinv(w'));
if pca_applied
    unmixing = unmixing * eigenvectors(:,1:ncomps);
end