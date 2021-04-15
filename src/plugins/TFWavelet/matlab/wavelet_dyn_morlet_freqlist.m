function [wt] = dyn_morlet_freqlist(x,Fs,freqlist,xi)
%   awt_freqlist  analytical wavelet transform, where one can specify the list of desired frequencies 
%   
%   [wt,freqlist,psi_array] = awt_freqlist(x,Fs,freqlist,type,xi)
%
%   Inputs:
%       x           the signal to be analyzed
%       Fs          the sampling frequency
%       freqlist    list of frequencies at which to compute wt (optional)
%                   (or set to [] for automatic definition)
%       xi          the number of oscillations parameter

%   Outputs:
%       wt: time-frequency image (npts x nfreqs)
%       freqlist: useful in case of automatic definition of frequencies
%       psi_array : array of analysis functions (complex values)
%
%  modified from awt_freqlist 
% (Maureen Clerc, Christian Benar, october 2007 modified from awt from
% wavelab toolbox)

% History of changes
% 1/11/2007: (cgb) psi_array: output in complex 
% 3/06/2008: (cgb) init of psi_array to size of wt
% 15/04/2021: (cgb) converted from axt_freqlist

n = length(x);
sigma2 = 1;
x = x(:);
omega = [(0:n/2) (-ceil(n/2)+1:-1)].*Fs/n; % CGB added ceil: TO BE CHECKED
omega = omega(:);

fftx = fft(x);

% to compute min and max center frequency values:
%tolerance = 1.5; % such that an acceptable "zero" is exp(-tolerance^2/2)
tolerance = 0.5; % cgb
%tolerance = 1; % cgb

% compute min and max valid center frequencies, according to wavelet type
mincenterfreq = 2*tolerance*sqrt(sigma2)*Fs*xi./n;
maxcenterfreq = Fs*xi/(xi+tolerance/sqrt(sigma2));

if isempty(freqlist) 
    nvoice = 12;
    freqlist= 2.^(log2(mincenterfreq):1/nvoice:log2(maxcenterfreq));
end


s_array = xi./freqlist;
minscale = xi./maxcenterfreq;
maxscale = xi./mincenterfreq;

nscale = length(freqlist);
wt = zeros(n,nscale);
scaleindices=find(s_array(:)'>=minscale & s_array(:)'<=maxscale);
%psi_array=zeros(n,length(scaleindices));
%psi_array=zeros(n,nscale);

%for kscale=scaleindices  % not matlab coder compatible
for i=1:numel(scaleindices)
    kscale = scaleindices(i);
    s=s_array(kscale);
    
    freq =  (s .* omega  - xi);
    Psi = realpow(4.*pi.*sigma2,1/4)*sqrt(s) *exp(-sigma2/2*freq.*freq);
    
    wt(1:n,kscale) = abs(ifft(fftx.*Psi)).^2;
 %   psi_array(:,kscale)=ifft(Psi);
end
