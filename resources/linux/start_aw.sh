#!/bin/bash
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

if [ -f $HOME/AnyWave/matlab.sh ];
then
	echo Executing matlab.sh
	. $HOME/AnyWave/matlab.sh
fi

export QT_QPA_PLATFORM_PLUGIN_PATH=$SCRIPTPATH/lib/plugins/platforms

if [ -f $HOME/AnyWave/matlab.sh ];
then
	chmod +x $HOME/AnyWave/matlab.sh
	echo Executing matlab.sh
	. $HOME/AnyWave/matlab.sh
fi
$SCRIPTPATH/AnyWaveLinux
exit
