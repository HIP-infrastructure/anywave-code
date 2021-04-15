cfg = coder.config('lib');
cfg.CustomBLASCallback = 'mklcallback';
cfg.CustomFFTCallback = 'mklFFTW';

vec = coder.newtype('double', [1 Inf], [0 1]);
d = coder.newtype('double', [1 1], [0 0]);
b = coder.newtype('logical', [1 1], [0 0]);
single_vec = coder.newtype('single', [1 Inf], [0 0]);

%codegen -c -lang:c++ DoG -args {vec,vec,d, d, d, d, b} -config cfg -report
%codegen -c -lang:c++ DoG_zH0 -args {vec,vec,d, d, d, d, b} -config cfg -report
%codegen -c -lang:c++ compute -args {single_vec, vec, d, d, d, d, d, d, d, d, d, d, d} -config cfg -report
codegen -c -lang:c++ wavelet_dyn_morlet_freqlist -args {vec, d, vec, d} -config cfg -report