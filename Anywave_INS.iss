[Setup]
AppName=AnyWave
AppVersion=2.7.3
DefaultDirName={sd}\AnyWave
DefaultGroupName=AnyWave
UninstallDisplayIcon={app}\Anywave.exe   
OutputDir="c:\dev\tmp"
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
ChangesAssociations=yes
SetupIconFile=C:\dev\anywave-vs2019\resources\images\AnyWave_icon.ico
AppPublisher= Dynamap team (INS - Aix Marseille Université INSERM UMR1106)
AppPublisherURL=https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave
AppSupportURL=https://gitlab-dynamap.timone.univ-amu.fr/anywave/anywave/-/wikis/home
AppContact=bruno.colombet@univ-amu.fr

[InstallDelete]
Type: files; Name: "{app}\*.*"
Type: files; Name: "{app}\Plugins\*.dll"
Type: files; Name: "{app}\include\*"
Type: files; Name: "{app}\Python\*"

[Registry]
Root: HKCR; Subkey: ".trc"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".vhdr"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".eeg"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".edf"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".bdf"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".ades"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".cnt"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".fif"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".mff"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: ".set"; ValueType: string; ValueName: ""; ValueData: "AnyWave";    Flags: uninsdeletevalue
Root: HKCR; Subkey: "AnyWave"; ValueType: string; ValueName: ""; ValueData: "AnyWave"; Flags: uninsdeletekey
Root: HKCR; Subkey: "AnyWave\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\ANYWAVE.EXE,0"
Root: HKCR; Subkey: "AnyWave\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\ANYWAVE.EXE"" ""%1"""


[Files]
Source: ".\bin\x64\Release\*.dll"; DestDir: "{app}"
Source: ".\bin\x64\Release\update.json"; DestDir: "{app}"
Source: ".\bin\x64\Release\*.exe"; DestDir: "{app}"
Source: ".\bin\x64\Release\*.html"; DestDir: "{app}"
Source: ".\bin\x64\Release\ins.txt"; DestDir: "{app}"
Source: ".\bin\x64\Release\qt.conf"; DestDir: "{app}"
Source: ".\lib\*.lib"; DestDir: "{app}\lib"
Source: ".\include\*"; DestDir: "{app}\include"; Flags: recursesubdirs
; Source: ".\bin\x64\Release\Python\*"; DestDir: "{app}\Python"; Flags: recursesubdirs
Source: ".\bin\x64\Release\Plugins\MATLAB\AnyWave\*.mexw64"; DestDir: "{app}\Plugins\MATLAB\AnyWave\"; Flags: recursesubdirs
Source: ".\bin\x64\Release\Plugins\4DNI_Reader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\4DNIExporter.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\AppendFilesv2.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\AutoMarking.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\MergeFiles.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\EDFIO.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\EEPIO.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\EGIReader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\DeltamedBinTxtReader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\BrainVisionIO.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\Deltamed_Reader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\EEGInto4D.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\RegisterInject.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\CTF_Reader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\FileConverter.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\H2.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\MFV.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\H2Graphs.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\Micromed.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\icav2.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\TFWavelet2.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\FIFFIO.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\SPMReader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\EEGLABReader.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\CompumedicsPF.dll"; DestDir: "{app}\Plugins"
;Source: ".\bin\x64\Release\Plugins\Spectral.dll"; DestDir: "{app}\Plugins"
Source: ".\bin\x64\Release\Plugins\MATLAB\PSDMATLAB\*.*"; DestDir: "{app}\Plugins\MATLAB\PSD"; Flags: recursesubdirs;

Source: ".\redist\vc_redist.x64.exe"; DestDir: "{app}"; Flags: deleteafterinstall
Source: ".\bin\x64\Release\QtPlugins\*.*"; DestDir: "{app}\QtPlugins\"; Flags: recursesubdirs
Source: "resources\windows\firewall.ps1"; DestDir: "{tmp}"

[Icons]
Name: "{commondesktop}\AnyWave"; Filename: "{app}\Anywave.exe"; Tasks: desktopicon
Name: "{commonprograms}\AnyWave.exe"; Filename: "{app}\Anywave.exe"; Tasks: startmenu

[Tasks]
Name: desktopicon; Description: "Create a desktop icon"; GroupDescription: "Additional icons"; 
Name: startmenu; Description: "Add shortcut to start menu"; GroupDescription: "Shortcuts"; 

[Run]
Filename: "{app}\vc_redist.x64.exe"; Parameters: "/quiet /norestart"; Description: "Visual C++ runtime"; StatusMsg: "Installing Visual C++ runtime..."

; Filename: "powershell.exe"; Parameters: "-Command ""{tmp}/firewall.ps1 {app}/anywave.exe"""; WorkingDir: "{app}"; Flags: postinstall waituntilterminated runhidden; Description: "Open firewall access"; StatusMsg: "Creating firewall rules..."

