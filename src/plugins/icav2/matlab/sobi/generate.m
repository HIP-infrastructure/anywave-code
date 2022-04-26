cfg = coder.config('lib');
cfg.CustomBLASCallback = 'mklcallback';
mat = coder.newtype('double', [Inf Inf], [1 1]);
codegen -c -lang:c++ sobi_coder -args {mat,0} -config cfg -report