python_exe = 'C:\dev\anywave-vs2019\bin\x64\Debug\Python\venv\Scripts\python.exe';
package = 'C:\dev\anywave-vs2019\src\Python';
pe =  pyenv('Version', python_exe);
% Ensure python-matlab integration code is on matlab path.
%pyFolder = fullfile(matlabroot, 'toolbox', 'matlab', 'external', 'interfaces', 'python');
% Add folders to python system path.
insert(py.sys.path, int64(0), package);
aw_debug_connect(59000);
channels = aw_getdata_ex();
disp(channels);
