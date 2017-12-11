#!/bin/sh

#  pres_install_qt5.sh
#  
#
#  Created by bruno colombet on 11/04/2016.
#
if [[ -d /Applications/AnyWave.app ]]; then
   rm -rf /Applications/AnyWave.app
fi
if [[ -d /Applications/Anywave_Plugins ]]; then
   rm -rf /Applications/Anywave_Plugins
fi