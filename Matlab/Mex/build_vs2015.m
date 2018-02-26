function build_vs2015
output = 'd:\\dev\\AnyWaveQt58_VS2015\\bin\\x64\\debug\\Plugins\\Matlab\\AnyWave';
aw_include = '-ID:\\dev\\AnyWaveQt58_VS2015\\include';
qt_include = '-ID:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\include';
qt_include2 = '-ID:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\include\\QtCore';
aw_lib = '-LD:\\Dev\\AnyWaveQt58_VS2015\\lib';
qt_lib = '-LD:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\lib';

% build common file
mex('-largeArrayDims', qt_include, qt_lib,  '-lQt5Core',  '-lQt5Network', '-c',  'common.cpp');

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
        common = 'common.obj';
        mex('-largeArrayDims', aw_include, aw_lib, qt_include, qt_include2, qt_lib,  '-lQt5Core',  '-lQt5Network', '-lAwCoreLib', file, common, '-outdir', output);
    end
end