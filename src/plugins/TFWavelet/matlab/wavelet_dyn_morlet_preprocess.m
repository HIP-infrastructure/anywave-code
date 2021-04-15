function data_preproc=dyn_morlet_preprocess(data,srate,dodiff,xi,freqmin,pad_fact)
% preprocess data before continuous wavelet transform
% srate: sampling rate
% dodiff: 1=diff transform
% xi: oscilation parameter of morlet wavelet
% freqmin: lowest frequency of TF plane
% pad_fact: duration of padding in number of wavelet width (based on lowfreq and xi)

% do_col = 0;
% s=size(data);
% if(s(1)<s(2)) % data in line vector
%     do_col=1; % transpse to column
% end

% if(do_col)
%     data=data';
% end

wt_len=xi/(2*pi*freqmin);
pad_len=pad_fact*ceil(wt_len*srate);

data_preproc=[data(1)*ones(pad_len,1); data ;data(end)*ones(pad_len,1)];


if(dodiff)
    data_preproc=diff(data_preproc);
    data_preproc=[data_preproc(1) data_preproc]; % to get same number of points, duplicate first value
end

data_preproc=data_preproc(pad_len+1:pad_len+length(data));

% if(do_col)
%     data_preproc=data_preproc';
% end





