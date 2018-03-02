#!/bin/bash
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

LD_LIBRARY_PATH=$SCRIPTPATH/lib:$LD_LIBRARY_PATH:/lib/x86_64-linux-gnu:$SCRIPTPATH/Plugins

# PRELOAD AnyWave Qt libs for MATLAB plugins otherwise MALTAB Qt lib will be used and this is not going to work!!!
LD_PRELOAD=$SCRIPTPATH/lib/libQt5Core.so:$SCRIPTPATH/lib/libQt5Network.so
export LD_PRELOAD
export LD_LIBRARY_PATH

if [ -f $HOME/AnyWave/matlab.sh ];
then
	echo Executing matlab.sh
	. $HOME/AnyWave/matlab.sh
fi

export QT_QPA_PLATFORM_PLUGIN_PATH=$SCRIPTPATH/lib/plugins/platforms
$SCRIPTPATH/AnyWaveLinux
exit
