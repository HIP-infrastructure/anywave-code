function build_linux( )
output = '/home/bruno/dev/aw-gitlab/build/AnyWave/Plugins/Matlab/AnyWave';
% option_file = './mex_C++_glnxa64.xml';
includes = '-I/usr/include/X86_64-linux-gnu/qt5 -I/usr/include/X86_64-linux-gnu/qt5/QtCore -I/usr/include/X86_64-linux-gnu/qt5/QtNetwork';

disp('Compiling common functions...');
mex('-largeArrayDims', '-v', includes, '-c',  'common.cpp');

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
        mex('-largeArrayDims', '-f', option_file, includes, file, 'common.o', '-outdir', output);
    end


end

