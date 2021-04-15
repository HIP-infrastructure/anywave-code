classdef mklFFTW < coder.fftw.StandaloneFFTW3Interface
     
    methods (Static)
        function th = getNumThreads
            coder.inline('always');
            th = int32(coder.const(1));
        end
                
        function updateBuildInfo(buildInfo, ctx)
            fftwLocation = '/usr/lib/fftw';
            includePath = fullfile(fftwLocation, 'include');
            buildInfo.addIncludePaths(includePath);
            libPath = fullfile(fftwLocation, 'lib');
            
            %Double
            libName1 = 'libfftw3-3';
            [~, libExt] = ctx.getStdLibInfo();
            libName1 = [libName1 libExt];
            addLinkObjects(buildInfo, libName1, libPath, 1000, true, true);
            
            %Single
             libName2 = 'libfftw3f-3';
            [~, libExt] = ctx.getStdLibInfo();
            libName2 = [libName2 libExt];
            addLinkObjects(buildInfo, libName2, libPath, 1000, true, true);
        end
    end           
end