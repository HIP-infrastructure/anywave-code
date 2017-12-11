#!/bin/bash
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
echo $SCRIPTPATH

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu:$SCRIPTPATH/lib:$SCRIPTPATH/Plugins

if [ -f $HOME/AnyWave/matlab.sh ];
then
	echo Executing matlab.sh
	. $HOME/AnyWave/matlab.sh
fi


export LD_LIBRARY_PATH

$SCRIPTPATH/AnyWaveLinux
exit
