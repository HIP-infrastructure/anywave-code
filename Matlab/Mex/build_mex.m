function build_mex(sdk, version)
% get the computer arch
arc = computer();

if isempty(sdk)
    error('Please, specify the SDK path');
end

if isempty(version)
    version = 56;
end

setenv('AWSDK', sdk);

if strcmp(arc, 'PCWIN64')
    output = sprintf('%s\\bin\\x64\\debug\\Plugins\\Matlab\\AnyWave', sdk);
    os = 0;
elseif  strcmp(arc, 'MACI64')
    output = sprintf('%s/bin/dist/Anywave_Plugins/MATLAB/AnyWave', sdk);
    os = 1;
elseif strcmp(arc, 'GLNXA64')
    output = sprintf('%s/bin/Plugins/Matlab/AnyWave', sdk);
    os = 2;
end

disp(sprintf('Mex files will go to %s', output));

% checking for qt version to use
mexoptions = '';
if os == 0
    % WIN64
    if version == 56
        mexoptions = 'mexopts_qt5.bat';
    else
        LIBPATH = sprintf('-L%s\\lib -LD:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\lib',sdk);
        INCLUDEPATH = sprintf('-I%s\\include -ID:\\Qt\\Qt5.8.0\\5.8\\msvc2015_64\\include',sdk);
        mexoptions = [LIBPATH INCLUDEPATH '-lQtCore -lQtNetwork -lAwCoreLib']; 
    end
else
    mexoptions = './mexopts.sh';
end


disp('Compiling common functions...');
if ~isempty(mexoptions)
    disp(['Mex options file is ' mexoptions]);
    if version == 56
        mex('-largeArrayDims', '-f', mexoptions, '-c',  'common.cpp');
    else
         mex('-largeArrayDims', mexoptions, '-c',  'common.cpp');
    end
else
    disp('No mex options file specified.');
     mex('-largeArrayDims', '-c',  'common.cpp');
end

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

% subfunctions()
    function buildCpp(file)
        disp(['Compiling ' file]);
        common = 'common.obj';
        if os == 1
            common = 'common.o';
            if ~isempty(mexoptions)
                if version == 56
                    mex('-largeArrayDims', '-f', mexoptions, file, common, '-outdir', output);
                else
                    mex('-largeArrayDims', mexoptions, file, common, '-outdir', output);
                end
            else
                mex('-largeArrayDims', file, common, '-outdir', output);
            end
        end
        if os == 0
            if ~isempty(mexoptions)
                mex('-largeArrayDims', '-f', mexoptions, file, common, '-outdir', output);
            else
                mex('-largeArrayDims', file, common, '-outdir', output);
            end
        end
        if os == 2
            mex('-largeArrayDims',  file, '-lQt5Core -lQt5Network',  'common.o', '-outdir', output);
        end
    end
end