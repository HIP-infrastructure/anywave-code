[Setup]
AppName=AnyWave
AppVersion=2.4.2
DefaultDirName={sd}\AnyWave
DefaultGroupName=AnyWave
UninstallDisplayIcon={app}\Anywave.exe   
OutputDir="c:\dev\tmp"
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
ChangesAssociations=yes
UsePreviousAppDir=no

[InstallDelete]
Type: filesandordirs; Name: "{app}"

[Registry]
Root: HKCR; Subkey: ".trc"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".vhdr"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".edf"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".bdf"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ades"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".cnt"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".fif"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".mff"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ds"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".set"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: "AnyWave"; ValueType: string; ValueName: ""; ValueData: "AnyWave"; Flags: uninsdeletekey
Root: HKCR; Subkey: "AnyWave\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\ANYWAVE.EXE,0"
Root: HKCR; Subkey: "AnyWave\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\ANYWAVE.EXE"" ""%1"""


[Files]
Source: ".\bin\x64\Release\*.dll"; DestDir: "{app}"
Source: ".\bin\x64\Release\*.exe"; DestDir: "{app}"
Source: ".\bin\x64\Release\*.html"; DestDir: "{app}"
Source: ".\bin\x64\Release\*.pl"; DestDir: "{app}"
Source: ".\bin\x64\Release\qt.conf"; DestDir: "{app}"
Source: ".\lib\*.lib"; DestDir: "{app}\lib";
Source: ".\include\*"; DestDir: "{app}\include"; Flags: recursesubdirs
Source: ".\bin\x64\Release\Python\*"; DestDir: "{app}\Python"; Flags: recursesubdirs
Source: ".\bin\x64\Release\Plugins\MATLAB\AnyWave\*.mexw64"; DestDir: "{app}\Plugins\MATLAB\AnyWave"; Flags: recursesubdirs
Source: ".\bin\x64\Release\Plugins\4DNI_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\4DNIExporter.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EEGInto4D.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\AppendFilesv2.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\AutoMarking.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EDFIO.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EEPIO.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EGIReader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\BrainVisionIO.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\DeltamedBinTxtReader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\CTF_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EEPIO.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\FileConverter.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\H2.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\MFV.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\H2Graphs.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\Micromed.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\icav2.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\TFWavelet2.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\FIFFIO.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\MergeFiles.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\SPMReader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EEGLABReader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\CompumedicsPF.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\Spectral.dll"; DestDir: "{app}\Plugins";
Source: ".\redist\vc_redist.x64.exe"; DestDir: "{app}"; Flags: deleteafterinstall
Source: ".\bin\x64\Release\QtPlugins\*.*"; DestDir: "{app}\QtPlugins\"; Flags: recursesubdirs

[Icons]
Name: "{commondesktop}\AnyWave"; Filename: "{app}\Anywave.exe"; Tasks: desktopicon
Name: "{commonprograms}\AnyWave.exe"; Filename: "{app}\Anywave.exe"; Tasks: startmenu

[Tasks]
Name: desktopicon; Description: "Create a desktop icon"; GroupDescription: "Additional icons"; 
Name: startmenu; Description: "Add shortcut to start menu"; GroupDescription: "Shortcuts"; 

[Run]
Filename: "{app}\vc_redist.x64.exe"; Description: "Visual C++ runtime"; Parameters: "/q /norestart"; StatusMsg: "Installing Visual C++ runtime..."; 