[Setup]
AppName=AnyWave
AppVersion=1.0
DefaultDirName=AnyWave
DefaultGroupName=AnyWave
UninstallDisplayIcon={app}\Anywave.exe   
OutputDir="D:\Dev\AnyWaveQt58_vs2015\setup"
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
ChangesAssociations=yes

[Registry]
Root: HKCR; Subkey: ".trc"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".vhdr"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".edf"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".bdf"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ades"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ah5"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: "AnyWave"; ValueType: string; ValueName: ""; ValueData: "AnyWave"; Flags: uninsdeletekey
Root: HKCR; Subkey: "AnyWave\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\ANYWAVE.EXE,0"
Root: HKCR; Subkey: "AnyWave\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\ANYWAVE.EXE"" ""%1"""


[Files]
Source: ".\bin\x64\Release\*"; DestDir: "{app}"
Source: ".\bin\x64\Release\Python\*"; DestDir: "{app}\Python"
Source: ".\bin\x64\Release\Plugins\MATLAB\*.*"; DestDir: "{app}\Plugins\MATLAB"; Flags: recursesubdirs
Source: ".\bin\x64\Release\Plugins\4DNI_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\ANT_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\AppendFiles.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\AutoMarking.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\BiosemiReader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\BrainVisionIO.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\Deltamed_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\EEGInto4D.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\Rewrite4DNI.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\Correlation.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\CTF_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\FileConverter.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\H2.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\Micromed_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\MultiICA.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\TFWavelet.dll"; DestDir: "{app}\Plugins";
Source: ".\bin\x64\Release\Plugins\FIFF_Reader.dll"; DestDir: "{app}\Plugins";
Source: ".\setup\vcredist.exe"; DestDir: "{app}"; Flags: deleteafterinstall
Source: ".\setup\vc_redist_2015.x64.exe"; DestDir: "{app}"; Flags: deleteafterinstall
Source: ".\bin\x64\Release\Python\*.*"; DestDir: "{app}\Python\"; Flags: recursesubdirs
Source: ".\bin\x64\Release\platforms\*.*"; DestDir: "{app}\platforms\"; Flags: recursesubdirs

[Icons]
Name: "{commondesktop}\AnyWave"; Filename: "{app}\Anywave.exe"; Tasks: desktopicon
Name: "{commonprograms}\AnyWave.exe"; Filename: "{app}\Anywave.exe"; Tasks: startmenu

[Tasks]
Name: desktopicon; Description: "Create a desktop icon"; GroupDescription: "Additional icons"; 
Name: startmenu; Description: "Add shortcut to start menu"; GroupDescription: "Shortcuts"; 

[Run]
Filename: "{app}\vcredist.exe"; Description: "Visual C++ runtime"; Parameters: "/q"; StatusMsg: "Installing Visual C++ runtime..."; 
Filename: "{app}\vc_redist_2015.x64.exe"; Description: "Visual Sutdio 2015 C++ runtime"; Parameters: "/q"; StatusMsg: "Installing Visual 2015 C++ runtime...";
