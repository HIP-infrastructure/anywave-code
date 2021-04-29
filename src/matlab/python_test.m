python_exe = 'C:\dev\anywave-vs2019\src\Python\anywave\venv\Scripts\python.exe';
package = 'C:\dev\anywave-vs2019\src\Python';
pe = pyenv('Version', python_exe);
py.importlib.import_module('anywave');
aw_debug_connect(59000);
channels = aw_getdata_ex();
disp(channels);