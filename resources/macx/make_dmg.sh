#!/bin/sh

#  make_dmg.sh
#  
#
#  Created by bruno colombet on 22/03/2016.
#

hdiutil create -volname AnyWave -srcfolder build/AnyWave.mpkg -ov -format UDZO AnyWave.dmg