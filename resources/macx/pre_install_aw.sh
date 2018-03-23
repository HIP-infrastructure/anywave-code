#!/bin/sh

#  pres_install_aw
#  
#
#  Created by bruno colombet on 11/04/2016.
#
#  remove AnyWave dirs and AwCore before the installer overwrites them
if [[ -d /Applications/AnyWave.app ]]; then
   rm -rf /Applications/AnyWave.app
fi
if [[ -d /Applications/Anywave_Plugins ]]; then
   rm -rf /Applications/Anywave_Plugins
fi
if [[ -d /Library/Frameworks/AwCore.framework ]] then
   rm -rf /Library/Frameworks/AwCore.framework
fi
