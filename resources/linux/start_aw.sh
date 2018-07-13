#!/bin/bash
SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")

LD_LIBRARY_PATH=$SCRIPTPATH/lib:$LD_LIBRARY_PATH:/usr/lib/x86_64-linux-gnu:$SCRIPTPATH/Plugins

# PRELOAD AnyWave Qt libs for MATLAB plugins otherwise MALTAB Qt lib will be used and this is not going to work!!!
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libQt5Core.so:/usr/lib/x86_64-linux-gnu/libQt5Network.so:/usr/lib/x86_64-linux-gnu/libstdc++.so.6

export LD_PRELOAD
export LD_LIBRARY_PATH

if [ -f $HOME/AnyWave/matlab.sh ];
then
	chmod +x $HOME/AnyWave/matlab.sh
	echo Executing matlab.sh
	. $HOME/AnyWave/matlab.sh
fi
anywave-binary
exit
