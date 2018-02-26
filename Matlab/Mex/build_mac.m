function  build_mac()
output = '/Users/bruno/dev/aw-svn/build/bin/dist/Anywave_Plugins/MATLAB/AnyWave';
aw_include = '-I/Users/bruno/dev/aw-svn/build/include';
aw_lib = '-L/Users/bruno/dev/aw-svn/build/lib';
disp('Compiling common functions...');
mex_options = './mexopts.sh';
mex('-largeArrayDims', mex_options, aw_include,  '-c',  'common.cpp');

buildCpp('aw_sendcommand.cpp');
buildCpp('aw_getplugininfo.cpp');
buildCpp('aw_getfileinfo.cpp');
buildCpp('aw_getmarkers.cpp');
buildCpp('aw_getdata.cpp');
buildCpp('aw_addmarkers.cpp');
buildCpp('AwIsProcessTerminated.cpp');
buildCpp('AwSendMessage.cpp');
buildCpp('aw_getcapture.cpp');
buildCpp('aw_captureicapanel.cpp');
buildCpp('aw_setbf.cpp');

    function buildCpp(file)
        disp(['Compiling ' file]);
        mex('-largeArrayDims', '-f', mex_options, aw_lib, '-lAwCoreLib', aw_include, file, 'common.o', '-outdir', output);
    end
end

