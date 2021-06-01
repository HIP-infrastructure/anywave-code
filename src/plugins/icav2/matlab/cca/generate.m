cfg = coder.config('lib');
cfg.CustomBLASCallback = 'mklcallback';
mat = coder.newtype('double', [Inf Inf], [1 1]);
codegen -c -lang:c++ cca -args {mat} -config cfg -report